#include "stdafx.h"
#include "oswiadczenieFPMW.h"
#include "datatypes/CompanyInfo.h"
#include "MainCfg.h"
#include "../logo/Logo.h"
#include "../Utils/StringFormat.h"

IMPLEMENT_DYNCREATE(COswiadczenieFPWMDoc, CCowDrwDoc)


COswiadczenieFPWMDoc::COswiadczenieFPWMDoc() : 	m_currInv(NULL)
{
    
}

int COswiadczenieFPWMDoc::GetPrintPages() const
{
	return m_invs.GetCount();
}

int COswiadczenieFPWMDoc::LoadInputData(CPtrArray &docInputData)
{
	m_invs.RemoveAll();
	m_invs.Append((CInvoiceDataArray&)docInputData);

	return RELOAD_OK_CONTINUE;
}

void COswiadczenieFPWMDoc::PreparePage(int nPage)
{
	m_currInv = m_invs[nPage-1];

	COleStreamFile logoStream;
	if(CLogoImage::GetLogo()->GetOleLogoImage(logoStream))
		DrawImage("LOGO",logoStream);
}
void COswiadczenieFPWMDoc::PrintPage(int nPage)
{
	const char* moneyFloatValueFmt = "%.2f zl";
	if(m_currInv->GetBuyOrSell() == BUY)
	{
		PrintBuyer(Company::GetCompany()->GetAsHent());
		PrintSeller(m_currInv->GetInvoiceHent());
	}
	else
	{
		PrintSeller(Company::GetCompany()->GetAsHent());
		PrintBuyer(m_currInv->GetInvoiceHent());
	}

	Money invGrosValue = m_currInv->GetInvoiceGrossValue();
	Money fpwmValue(100.0);//invGrosValue*AppCfg().GetFloat(CFGID_FPMWRATE);
	DrawText("BASE_VALUE",CStringFormat(moneyFloatValueFmt,invGrosValue.GetDouble()));
	DrawText("FPWM_VALUE",CStringFormat(moneyFloatValueFmt,fpwmValue.GetDouble()));


	CString invoiceNo;

	CString sCustomNumber = m_currInv->GetCustomNumber();

	if(sCustomNumber.IsEmpty())
		invoiceNo = m_currInv->GetInvoiceNoStr();
	else
	{
		invoiceNo = sCustomNumber;
		DrawText("SMALL_INVOICENO",m_currInv->GetInvoiceNoStr());
	}

	DrawText("INVOICENO",invoiceNo);
	DrawText("INVOICE_DATE",(LPCSTR)m_currInv->GetInvoiceDate().GetDayDate());

}

void COswiadczenieFPWMDoc::PrintBuyer(const Hent *buyer)
{
	CString Str;
	
	DrawText("BUYER_NAME",buyer->GetName());
	Str.Format("%s %s\n%s %s",(LPCSTR)buyer->GetStreet(),(LPCSTR)buyer->GetPOBox(),(LPCSTR)buyer->GetZip(),(LPCSTR)buyer->GetCity());
    DrawText("BUYER_ADDRESS",Str);
	DrawText("BUYER_FARMNO",buyer->GetFarmNo());
	DrawText("BUYER_WETNO",buyer->GetWetNo());

	Str.Empty();
	if(buyer->HasNIP())
		Str += buyer->GetNIP();
	if(buyer->HasREGON())
		Str.AppendFormat("(%s)",(LPCSTR)buyer->GetREGON());
	DrawText("BUYER_NIP_REGON",Str);

	DrawText("BUYER_ACCOUNTNO",buyer->GetAccountNo());
	DrawText("BUYER_BANKNAME",buyer->GetBankName());
}

void COswiadczenieFPWMDoc::PrintSeller(const Hent *seller)
{
	CString Str;

	DrawText("SELLER_NAME",seller->GetName());
	Str.Format("%s %s\n%s %s",(LPCSTR)seller->GetStreet(),(LPCSTR)seller->GetPOBox(),(LPCSTR)seller->GetZip(),(LPCSTR)seller->GetCity());
    DrawText("SELLER_ADDRESS",Str);
	DrawText("SELLER_FARMNO",seller->GetFarmNo());
	DrawText("SELLER_WETNO",seller->GetWetNo());

	/*Str.Empty();
	if(!seller->pesel.IsEmpty())
		Str = seller->pesel;
	if(seller->HasNIP())
		Str.AppendFormat("(%s)",(LPCSTR)seller->GetNIP());*/

	DrawText("SELLER_ACCOUNTNO",seller->GetAccountNo());
	DrawText("SELLER_BANKNAME",seller->GetBankName());
}