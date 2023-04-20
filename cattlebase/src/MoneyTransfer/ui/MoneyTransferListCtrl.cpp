#include "stdafx.h"
#include "MoneyTransferListCtrl.h"
#include "MainCfg.h"
#include "MoneyTransferEntryComparator.h"
#include "../../classes/IntString.h"

#define MONEYTRANSFERENTRY_LP (MONEYTRANSFERENTRY_MAX+1)


const GRIDHEADERS CMoneyTransferListCtrl::szColHeaders[] =
{
	{IDS_HEADERINDX,		MONEYTRANSFERENTRY_LP},
	{IDS_HEADERHENT,		MONEYTRANSFERENTRY_HENT},
	{IDS_HEADERDOCNO,		MONEYTRANSFERENTRY_INVOICENO},
	{IDS_HEADERCUSTNO,		MONEYTRANSFERENTRY_INVOICECUSTNO},	
	{IDS_HEADERINVVALUE,	MONEYTRANSFERENTRY_INVOICEVALUE},
	{IDS_HEADERTRANSVALUE,	MONEYTRANSFERENTRY_TRANSFERVALUE},
	{IDS_HEADERERROR,		MONEYTRANSFERENTRY_INVALIDCAUSE},
	{0,0}
};

BEGIN_MESSAGE_MAP(CMoneyTransferListCtrl, CBaseListCtrl)

END_MESSAGE_MAP()

CMoneyTransferListCtrl::CMoneyTransferListCtrl(PtrFlushArray<MoneyTransferEntry>& moneyTransferEntries) : 
	CBaseListCtrl(szColHeaders,CFGID_MONEYTRANSLIST),m_moneyTransferEntries(moneyTransferEntries)
{

}
void CMoneyTransferListCtrl::UpdateItemCount()
{
	SetItemCount(m_moneyTransferEntries.GetCount());
}

void CMoneyTransferListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	CString sCellTxt;
	UINT nColId = GetColID(nSubItem);
	
	if(MONEYTRANSFERENTRY_LP == nColId)
		sCellTxt = (CString)CIntString(nItem+1);
	else
		sCellTxt = m_textProvider.GetDataText(m_moneyTransferEntries[nItem],nColId);

	strcpy(pszItemText,(LPCSTR)sCellTxt);
}

bool CMoneyTransferListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{
	if( GetColID(nSubItem) == MONEYTRANSFERENTRY_INVALIDCAUSE && m_moneyTransferEntries[nItem]->IsInvalid())
	{
		*cellBkg = RGB(255,0,0);
		return true;
	}
	return false;
}

void CMoneyTransferListCtrl::SortByCol(int Column,bool bAsc)
{
	//MonenyTansferEntryComparator comparator;
	
	//ary_stocks.SortBy(GetColID(Column),bAsc);
}
