#include "stdafx.h"
#include "zbiorowkafv.h"
#include "../consts.h"
#include "../Utils/StringFormat.h"
#include "../classes/IntString.h"

CZbiorowkaFVDoc::CZbiorowkaFVDoc(BuyOrSell buy_sell) : 
	starti(0),endi(0),m_nPages(0),buyOrsell(buy_sell),totalNetto(0.0),totalVAT(0.0),totalCows(0)
{
}

int CZbiorowkaFVDoc::GetPrintPages() const
{
	return m_nPages;
}

int CZbiorowkaFVDoc::LoadInputData(CPtrArray &docInputData)
{
	m_invs2print.RemoveAll();
	m_invs2print.Append((CInvoiceDataArray&)docInputData);

	int pages = 0,size;
	size = m_invs2print.GetSize();
	Invoice *inv2accept;
	bool bSkipped = false;

	invs.RemoveAll();
	totalNetto = 0.0;
	totalVAT = 0.0;
	totalCows = 0;

	for(int i = 0;i < size;i++)
	{
		inv2accept = m_invs2print[i];
		if(inv2accept->HasCorrect()) 
		{
			bSkipped = true;
			continue;
		}
					
		if(inv2accept->GetBuyOrSell() == buyOrsell)
		{
			invs.Add(inv2accept);
			totalNetto += inv2accept->GetInvoiceNetValue();
			totalVAT += inv2accept->GetInvoiceNetValue()*inv2accept->GetVATRate();
			totalCows += inv2accept->GetCowQty();
		}


		else bSkipped = true;
	}

	size = invs.GetSize();
	if(!size) return RELOAD_NO_CONTINUE;

	invs.SortBy(INVS_SORT_BY_INVDATE,true);

	date_from = invs[0]->GetInvoiceDate();
	date_to = invs[size-1]->GetInvoiceDate();

	pages = (size/ZBIOROWKAFVPAGESIZE);
	if(size%ZBIOROWKAFVPAGESIZE) pages++;

    m_nPages = pages;

	return (bSkipped ? RELOAD_CAN_CONTINUE : RELOAD_OK_CONTINUE);
}

void CZbiorowkaFVDoc::PreparePage(int nPage)
{
    
	ResetGroupsVisibility(false);
	CString grpname;
		
	starti = ZBIOROWKAFVPAGESIZE*(nPage-1);
	endi = starti + ZBIOROWKAFVPAGESIZE - 1;
	int size = invs.GetSize(),grp = 0,i;    
						
	if(endi >= size) endi = size-1;
	
	for(i=starti;i<=endi;i++,grp++)
	{
		grpname.Format("%d",grp);
		SetGroupVisible(grpname,true);
	}
	if(nPage == m_nPages) SetGroupVisible("BOTTOM",true);				

}
void CZbiorowkaFVDoc::PrintPage(int page)
{
	CString fld,str;
	int i,row;

	char *invTypeName = NULL,
		szInvTypeNames[][10] = {
		"SPRZEDA¯Y",
		"KUPNA"
		};//,NoFmt[] = "%s%04d";

	invTypeName = szInvTypeNames[int(buyOrsell == BUY)];
	
	
	DrawText("PAGES",CStringFormat("%d/%d",page,m_nPages));
	DrawText("HEADER",CStringFormat("ZESTAWIENIE FAKTUR VAT %s Z OKRESU OD %s DO %s",invTypeName,date_from.GetDayDate(),date_to.GetDayDate()));

	if(page == m_nPages)
	{
		DrawText("TOTALNETTO",totalNetto.ToString());
		DrawText("TOTALVAT",totalVAT.ToString());
		DrawText("TOTALBRUTTO",(totalVAT+totalNetto).ToString());
		DrawText("TOTALCOWS",CIntString(totalCows));
	}

    
	Invoice *pInv;
	char *szDestLabel = NULL;
	row = 0;
	for(i = starti;i <= endi;i++,row++)
	{
		pInv = invs[i];
	        
		CString sCustomNumber = pInv->GetCustomNumber();
		if(sCustomNumber.IsEmpty())
			szDestLabel = "INVNO%d";
		else
		{
			szDestLabel = "OTHER_INVNO%d";
			fld.Format("INVNO%d",row);
			DrawText(fld,sCustomNumber);
			
		}
		str = pInv->GetInvoiceNoStr();
		if(pInv->GetThisInvoiceType() == CORRCT_INV)
			str += '*';

		DrawText(CStringFormat(szDestLabel,row),str);
		DrawText(CStringFormat("INVDATE%d",row),pInv->GetInvoiceDate().GetDayDate());
		DrawText(CStringFormat("HENT%d",row),(LPCSTR)pInv->GetInvoiceHent()->GetName());
		DrawText(CStringFormat("QTY%d",row),CIntString(pInv->GetCowQty()));
		DrawText(CStringFormat("VATRATE%d",row),CStringFormat("%.2f",pInv->GetVATRate()*100.0));
	
		const char *szPayWay = NULL;
		switch(pInv->GetPayWay())
		{
			case Invoice::PAY_CASH:
				szPayWay = PayWayNames[1];
			break;
			case Invoice::PAY_TRANS:
				szPayWay = PayWayNames[0];
			break;
		}
		DrawText(CStringFormat("PAYWAY%d",row),szPayWay);

		fld.Format("PAID%d",row);
		if(!pInv->IsPaid())
			DrawText(fld,"NIE");
		else DrawText(fld,pInv->GetPaidDate().GetDayDate());

		Money inv_totalVAT = pInv->GetVATRate()*pInv->GetInvoiceNetValue();

		fld.Format("TOTALVAT%d",row);
		DrawText(fld,inv_totalVAT.ToString());

		fld.Format("TOTALNETTO%d",row);
		DrawText(fld,pInv->GetInvoiceNetValue().ToString());

		fld.Format("TOTALBRUTTO%d",row);
		DrawText(fld,(inv_totalVAT+pInv->GetInvoiceNetValue()).ToString());
		
	}
		
}

//#######################################################################################
IMPLEMENT_DYNCREATE(CZbiorowkaFVBuyDoc, CZbiorowkaFVDoc)
CZbiorowkaFVBuyDoc::CZbiorowkaFVBuyDoc() : CZbiorowkaFVDoc(BUY)
{
}

//#######################################################################################
IMPLEMENT_DYNCREATE(CZbiorowkaFVSellDoc, CZbiorowkaFVDoc)
CZbiorowkaFVSellDoc::CZbiorowkaFVSellDoc()  : CZbiorowkaFVDoc(SELL)
{
}


