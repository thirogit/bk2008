#include "stdafx.h"
#include "InvoiceDriver.h"
#include "../BaseInvoiceDlg.h"
#include "../../CattleSQL.h"

InvoiceDriver::InvoiceDriver(InvoiceContext* pInvoiceContext,CWnd* pParent/* = NULL*/)
{
	pInvoiceDialog = new CBaseInvoiceDlg(pInvoiceContext,this,pParent);
}

void InvoiceDriver::Do()
{
}

InvoiceDriver::~InvoiceDriver()
{
	delete pInvoiceDialog;
}



bool InvoiceDriver::InsertCowInvoiceEntries(CCowInvoiceEntryArray &cowInvEntries)
{
	CCowInvoiceEntryArray::ArrayIterator it = cowInvEntries.iterator();
	while(it.hasNext())
	{
		if(!((*(it++))->InsertThis(false)))
		{
			SQLDB.SQLMessageBox(IDS_ERRUPDATINGCOWPRICE);		
			return false;
		}
	}
	return true;
}


bool InvoiceDriver::DeleteInvoiceEntries(CCowInvoiceEntryArray& invoiceEntries)
{
	CCowInvoiceEntryArray::ArrayIterator it = invoiceEntries.iterator();
	while(it.hasNext())
	{
		if(!(*(it++))->DeleteThis(false))
		{
			SQLDB.SQLMessageBox(IDS_ERRDELETEINGINVOICEENTRIES);
			return false;
		}
	}
	return true;
}

#define UpInv(upname,val) pInvoice->SQLUpdate.Set(InvoiceData::upname,val)

void InvoiceDriver::UpdateInvoiceValues(InvoiceData* pInvoice)
{
	ASSERT(pInvoice);
	bool up_extras, up_invoicedate,up_vatrate, up_payway, up_customnumber,up_payduedays;

	InvoiceData::PAYWAYS setPayWay = m_payway.GetPayway();
	pInvoice->SQLUpdate.Reset();

	up_extras = UpInv(update_extras,m_extras.GetModify() == TRUE);
	up_invoicedate = UpInv(update_invoicedate,m_invoicedate.GetModify() == TRUE);
	up_vatrate = UpInv(update_VATrate,pInvoice->VATrate != m_InvoiceSession.GetVATRate().GetDouble());
	up_payway = UpInv(update_payway,pInvoice->payway != setPayWay);
	up_customnumber = UpInv(update_customnumber,m_customNumber.Compare(pInvoice->customnumber) != 0);
	up_payduedays = UpInv(update_payduedays,m_spnPayDueDays.GetPos() != pInvoice->payduedays);

	if(up_extras) m_extras.GetWindowText(pInvoice->extras);

	if(up_invoicedate) m_invoicedate.GetDate(pInvoice->invoicedate);
		
	if(up_vatrate)	pInvoice->VATrate = m_InvoiceSession.GetVATRate().GetDouble();

	if(up_payway) pInvoice->payway = (InvoiceData::PAYWAYS)setPayWay;
		
	if(up_customnumber) pInvoice->customnumber = m_customNumber;

	if(up_payduedays) pInvoice->payduedays = m_spnPayDueDays.GetPos(); 
}