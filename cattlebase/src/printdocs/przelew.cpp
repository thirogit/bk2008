#include "stdafx.h"
#include "przelew.h"
#include "MainCfg.h"
#include "../consts.h"
#include "datatypes/CompanyInfo.h"
#include "../Utils/StringFormat.h"

IMPLEMENT_DYNCREATE(CPrzelewDoc, CCowDrwDoc)


CPrzelewDoc::CPrzelewDoc() 
{
  m_bDiscountFee = AppCfg().GetBool(CFG_DISCOUNTTRANSFEE);
  m_transFee = AppCfg().GetFloat(CFG_TRANSFEE);

}

int CPrzelewDoc::LoadInputData(CPtrArray &docInputData)
{
	dataArray.RemoveAll();
	dataArray.Append((CInvoiceDataArray&)docInputData);

	int ret = RELOAD_OK_CONTINUE;
	Invoice *pInv;
	bool bSkipped = false;
	invs.RemoveAll();
	for(int i = 0;i< dataArray.GetSize();i++)
	{
		pInv = (Invoice*)dataArray[i];
		if(!pInv->HasCorrect() && pInv->GetBuyOrSell() == BUY)
			invs.Add(pInv);
		else
			bSkipped = true;
	}

		if(invs.GetSize() == 0)
			ret = RELOAD_NO_CONTINUE;
		else if(bSkipped) ret = RELOAD_CAN_CONTINUE;
	return ret;
}

int CPrzelewDoc::GetPrintPages() const
{
	return invs.GetSize();
}

void CPrzelewDoc::PrintPage(int page)
{
	
	Invoice *pInv = invs[page-1];
	Hent *pHent = pInv->GetInvoiceHent();
	PrintHent("RECV",pHent);

	const Company *pCompany = Company::GetCompany(); 
	PrintHent("SENDNAME",pCompany->GetAsHent());
	PrintAccountNo("RECVACCOUNTNO",pHent->GetAccountNo());
	PrintAccountNo("SENDACCOUNTNO",pCompany->GetAccountNo());

	Money transTotal(pInv->GetInvoiceNetValue());
	transTotal += transTotal*pInv->GetVATRate();

	if(m_bDiscountFee)
		transTotal -= m_transFee;

	PrintTransValue(transTotal);

	CString strFor,sInvoiceNo,sCustonNumber = pInv->GetCustomNumber();

	if(!sCustonNumber.IsEmpty())
		sInvoiceNo = sCustonNumber;
	else
		sInvoiceNo = pInv->GetInvoiceNoStr();

	strFor.Format("Faktura %s z dnia %s",(LPCSTR)sInvoiceNo,pInv->GetInvoiceDate().GetDayDate());

	if(strFor.GetLength() > GRIDROWSIZE)
		DrawText("SENDFOR_1_TENSE",strFor);
	else
		FillGridWithText("SENDFOR_1",strFor);

	
	
}


void CPrzelewDoc::PrintAccountNo(LPCSTR labelPrefix,const CString &sAccountNo)
{
	FillGridWithText(labelPrefix,sAccountNo);
}

void CPrzelewDoc::PrintTransValue(Money &Value)
{
	CString valueStr = Value.ToString();
	if(valueStr.GetLength() > GRIDTOTALSIZE)
		DrawText("TOTAL_TENSE",valueStr);
	else
		FillGridWithText("TOTAL",valueStr);
}


void  CPrzelewDoc::PrintHent(LPCSTR destLabel,const Hent *hent)
{
	CString label,szStr;
	CString hentName = hent->GetName();
	if(hentName.GetLength() > GRIDROWSIZE)
	{
		label.Format("%s_1_TENSE",destLabel);
		DrawText(label,hentName);
	}
	else
	{
		label.Format("%s_1",destLabel);
		FillGridWithText(label,hentName);
	}

	szStr.Format("%s %s %s %s",(
		LPCSTR)hent->GetStreet(),
		(LPCSTR)hent->GetPOBox(),
		(LPCSTR)hent->GetZip(),
		(LPCSTR)hent->GetCity());

	if(szStr.GetLength() > GRIDROWSIZE)
	{
		label.Format("%s_2_TENSE",destLabel);
		DrawText(label,szStr);
	}
	else 
	{
		label.Format("%s_2",destLabel);
		FillGridWithText(label,szStr);
	}
		
}

void CPrzelewDoc::FillGridWithText(LPCSTR labelPrefix,const CString &sText)
{
	CString label,text;
	for(int i = 0;i< sText.GetLength();i++)
	{
		label.Format("%s_%d",labelPrefix,i);
		text = sText.GetAt(i);
		DrawText(label,text);
	}
}

void CPrzelewDoc::DrawText(const char *szLabel,CString &strText)
{
	CString PinkLabel("PINK_"),GrayLabel("GRAY_"),upperText;
	upperText = strText.MakeUpper();

	CCowDrwDoc::DrawText(PinkLabel+szLabel,upperText);
	CCowDrwDoc::DrawText(GrayLabel+szLabel,upperText);
}
