#include "stdafx.h"
#include "CorrectVATInvoiceSession.h"
#include "../../exceptions/StrException.h"
#include "../../cattleSQL.h"
#include "../../dispacher/InvoiceDispatcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCorrectVATInvoiceSession::CCorrectVATInvoiceSession(VATInvoice *pInvoice,InvoiceContext* pInvoiceContext) : CowPriceInvoiceSession(pInvoiceContext)
{
	if(!pInvoice->CanEditThisInvoice())
	{
		m_Inv2Edit = pInvoice->GetLastGoodVATInvoice();
	}
	else
		throw new CStrException("Cant create correcting invoice for this VAT invoice.");
	

	SetVATRate(m_Inv2Edit->GetVATRate());
	SetPayDueDays(m_Inv2Edit->GetPayDueDays());	
	SetExtras(m_Inv2Edit->GetExtras());
	SetCustomNumber(m_Inv2Edit->GetCustomNumber());
	SetInvoiceDate(m_Inv2Edit->GetInvoiceDate());
	SetPayway(m_Inv2Edit->GetPayWay());
	SetInvoiceHent(m_Inv2Edit->GetInvoiceHent());
	SetTransactionDate(m_Inv2Edit->GetTransactionDate());
	SetTransactionPlace(m_Inv2Edit->GetTransactionPlace());
	
	CCowInvoiceEntryArray::ArrayIterator it = m_Inv2Edit->invEntries.iterator();

	while(it.hasNext())
	{	
		CowInvoiceEntry *pEntry = *(it++);
		CowPriceData *cowPrice = AddCow(pEntry->GetCow());
		cowPrice->SetCustomPriceNetto(pEntry->GetPrice());
		cowPrice->SetCustomStock(pEntry->GetStock());
		cowPrice->SetCustomWeight(pEntry->GetWeight());	
	}

	GetInvoiceDocs().Append(m_Inv2Edit->invoiceDocs);

}

CString CCorrectVATInvoiceSession::GetSessionTitle() const
{
	CString caption;
	caption.Format(IDS_INVCORRECTCAPTION,m_Inv2Edit->GetInvoiceNo());
	return caption;
}

void CCorrectVATInvoiceSession::AddItem()
{
	AfxMessageBox(IDS_CANTADDDOCSWHENCREATINGCORRECTINVOICE);	
}
void CCorrectVATInvoiceSession::DelItem()
{
	AfxMessageBox(IDS_CANTDELDOCSWHENCREATINGCORRECTINVOICE);
}

bool CCorrectVATInvoiceSession::CommitInvoice()
{
	VATInvoice *pInvoice = NULL;
	Cow *cowd = NULL;
	
	auto_ptr<VATInvoice> pInvoiceDelete;

	
	NullInvoice *pNullInvoice = CreateNullInvoice();
	pNullInvoice->SetInvoiceType(CORRCT_INV);
	pNullInvoice->SetCorrectFor(m_Inv2Edit);
	pInvoice = new VATInvoice(pNullInvoice);
	delete pNullInvoice;

	pInvoiceDelete.reset(pInvoice);
	pInvoice->SetInvoiceNetValue(GetTotalNetValue());	
					
	if(pInvoice->InsertThisInvoice(false))
	{			
		CCowInvoiceEntryArray cowInvEntries;
		CCowInvoiceEntryArray::AutoArray autoCowInvEntries(cowInvEntries);

		CCowPriceDataArray::ArrayIterator it = GetCowPriceIterator();
		while(it.hasNext())
		{
			cowInvEntries.Add(CreateNewInvoiceEntry(pInvoice,*(it++)));
		}

		if(!InsertCowInvoiceEntries(cowInvEntries))
		{
			SQLDB.Rollback();		
			return false;
		}
		pInvoice->invoiceDocs.Append(m_Inv2Edit->invoiceDocs);
		pInvoice->invEntries.Append(cowInvEntries);
		m_Inv2Edit->SetCorrect(pInvoice);
		SQLDB.Commit();			
		autoCowInvEntries.Release();
		pInvoiceDelete.release();
		InvoiceDispatcher::Dispatch(pInvoice);			
	}
	else
	{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGINVOICE);			
			SQLDB.Rollback();
			return false;
	}

	
	return true;

}