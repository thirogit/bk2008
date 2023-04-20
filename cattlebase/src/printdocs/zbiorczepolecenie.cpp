#include "stdafx.h"
#include "zbiorczepolecenie.h"
#include "przelew.h"
#include "MainCfg.h"
#include "datatypes/CompanyInfo.h"
#include "../Utils/StringFormat.h"
#include "../classes/IntString.h"

IMPLEMENT_DYNCREATE(CZbiorczePolecenieDoc, CCowDrwDoc)


CZbiorczePolecenieDoc::CZbiorczePolecenieDoc() 
{
  m_bDiscountFee = AppCfg().GetBool(CFG_DISCOUNTTRANSFEE) == TRUE;
  m_transFee = AppCfg().GetFloat(CFG_TRANSFEE);
}

int CZbiorczePolecenieDoc::LoadInputData(CPtrArray &docInputData)
{
	m_dataArray.RemoveAll();
	m_dataArray.Append((CInvoiceDataArray&)docInputData);

	int ret = RELOAD_OK_CONTINUE;
	Invoice *pInv;
	bool bSkipped = false;

	invs.RemoveAll();

	for(int i = 0;i< m_dataArray.GetSize();i++)
	{
		pInv = m_dataArray[i];
		if(!pInv->HasCorrect() == NULL && 
			(pInv->GetRealInvoiceType() == IN_INV_RR || pInv->GetRealInvoiceType() == IN_INV_VAT))
			invs.Add(pInv);
		else
			bSkipped = true;
	}

		if(invs.GetSize() == 0)
			ret = RELOAD_NO_CONTINUE;
		else if(bSkipped) ret = RELOAD_CAN_CONTINUE;
	return ret;
}

int CZbiorczePolecenieDoc::GetPrintPages() const
{
	int pages = invs.GetSize()/ZBIORCZEPOLECENIE_PAGESIZE;
	if(invs.GetSize()%ZBIORCZEPOLECENIE_PAGESIZE) pages++;
	return pages;
}

void CZbiorczePolecenieDoc::PrintPage(int page)
{

	int startidx = (page-1)*ZBIORCZEPOLECENIE_PAGESIZE,
			endidx  = (startidx+ZBIORCZEPOLECENIE_PAGESIZE)-1,
			size = invs.GetSize(),i,row;
	Invoice *pInv;

	CString fld,Str;

	if (endidx >= size)	endidx = size-1;
	

	Money pageTotal(0.0);

	for(i=startidx,row=0;i<=endidx;i++,row++)	
	{
		pInv = invs[i];
				
		Money transTotal(pInv->GetInvoiceNetValue());
		transTotal += transTotal*pInv->GetVATRate();

		if(m_bDiscountFee)
			transTotal -= m_transFee;

		pageTotal += transTotal;

	
		CCowDrwDoc::DrawText(CStringFormat("LP%d",row),CIntString(i+1));

		CCowDrwDoc::DrawText(CStringFormat("TRANSTOTAL%d",row),transTotal.ToString());

		DrawTextLR(CStringFormat("TRANSHENT%d",row),pInv->GetInvoiceHent()->GetName());
	}
	
	
	CCowDrwDoc::DrawText("TOTAL",pageTotal.ToString());
	
	CCowDrwDoc::DrawText("DAY",CDateTime::now().GetDayDate());
	CCowDrwDoc::DrawText("QTY",CIntString(endidx-startidx+1));
	DrawTextLR("ACCOUNTNO",Company::GetCompany()->GetAccountNo());
	
}


void CZbiorczePolecenieDoc::DrawTextLR(const CString& sLabel,const CString &sText)
{
	CString LeftLabel("LEFT_"),RightLabel("RIGHT_");
	CCowDrwDoc::DrawText(LeftLabel+sLabel,sText);
	CCowDrwDoc::DrawText(RightLabel+sLabel,sText);
}
