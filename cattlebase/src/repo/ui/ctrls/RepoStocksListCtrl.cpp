#include "stdafx.h"
#include "RepoStocksListCtrl.h"
#include "../../Discrepancy.h"
#include "../RepoColors.h"

#define REPOSTOCK_DISCREPANCY (REPOSTOCK_MEMBER_MAX+1)

GRIDHEADERS CRepoStocksListCtrl::LIST_HEADER_COLUMNS[] =
{
	{IDS_HEADER_DISCREPANCY,REPOSTOCK_DISCREPANCY},
	{IDS_HEADERNAME,		REPOSTOCK_MEMBER_NAME},
	{IDS_HEADERSHORT,		REPOSTOCK_MEMBER_CODE},
	{IDS_HEADERMYPERKGPRICE,REPOSTOCK_MEMBER_MYPRICE},
	{IDS_HEADERPREDEFSEX,	REPOSTOCK_MEMBER_PREDEFSEX},	
	{0,0}
};

BEGIN_MESSAGE_MAP(CRepoStocksListCtrl, CBaseListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK , OnClick)	
END_MESSAGE_MAP()


CRepoStocksListCtrl::CRepoStocksListCtrl(int iConfigId/* = -1*/) : CBaseListCtrl(LIST_HEADER_COLUMNS,iConfigId)
{	
}

void CRepoStocksListCtrl::SortByCol(int Column,bool bAsc)
{
//   m_pHentArray->SortBy(GetColID(Column),bAsc);
}

void CRepoStocksListCtrl::Select(Predicate<StockEquivalenceItem*>& selectPredicate,bool bAddToSelected/* = false*/)
{
	StockEquivalenceItem* pItem = NULL;
	PtrFlushArray<StockEquivalenceItem>::ArrayIterator it = m_pStockItems.iterator();
	while(it.hasNext())
	{
		pItem = *(it++);
		pItem->SetDoUpdate(selectPredicate.Evaluate(pItem) || (bAddToSelected && pItem->IsDoUpdate()));
	}
	Invalidate(FALSE);
}

void CRepoStocksListCtrl::UpdateListWith(StocksCluster* pStocksCluster)
{
	Lock();
	m_pStockItems.RemoveAll();
	if(pStocksCluster)
	{
		for(int i = 0,s = pStocksCluster->GetCount();i < s;i++)
		{
			m_pStockItems.Add(pStocksCluster->GetItem(i));
		}
	}
	UpdateItemCount();
	Unlock();
}

void  CRepoStocksListCtrl::UpdateItemCount()
{
	SetItemCount(m_pStockItems.GetCount());
}
	
void CRepoStocksListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{	

	UINT columnId = GetColID(nSubItem);

	if(REPOSTOCK_DISCREPANCY != columnId)
	{
		StockEquivalenceItem* pItem = m_pStockItems.GetAt(nItem);
		Discrepancy<RepoStock> discrepancy(*pItem);
		CString sText = m_TextProvider.GetDataText(discrepancy.GetSide(),columnId);
		strcpy(pszItemText,(LPCSTR)sText);
	}

}

bool CRepoStocksListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{
	if(GetColID(nSubItem) == REPOSTOCK_DISCREPANCY)
	{
		StockEquivalenceItem* pItem = m_pStockItems.GetAt(nItem);
		Discrepancy<RepoStock> discrepancy(*pItem);

		switch(discrepancy.GetType())
		{
			case MISSING_REMOTE:
				*cellBkg = COLOR_MISSING_REMOTE_SIDE;
				return true;
			case MISSING_LOCAL:
				*cellBkg = COLOR_MISSING_LOCAL_SIDE;
				return true;
			case HAS_BOTH:
				if(!pItem->IsEqual())
				{
					*cellBkg = COLOR_BOTH_SIDES_NOT_EQUAL;
					return true;
				}
				break;			
		}
	}
	return false;
}

int CRepoStocksListCtrl::GetItemImage(int iItem,int Column)
{
	UINT columnId = GetColID(Column);

	if(REPOSTOCK_DISCREPANCY == columnId)
	{
		StockEquivalenceItem* pItem = m_pStockItems.GetAt(iItem);
		return (pItem->IsDoUpdate() ? 1 : 0);
	}
	return -1;
}

void CRepoStocksListCtrl::PostCreate()
{
	CBaseListCtrl::PostCreate();
	m_checkboxImageList.Create(16,16,ILC_COLORDDB | ILC_MASK,2,1);
	CBitmap checkBoxes;
	checkBoxes.LoadBitmap(IDB_CHECKBOX);
	m_checkboxImageList.Add(&checkBoxes,RGB(255,0,255));
	SetImageList(&m_checkboxImageList,LVSIL_SMALL);
}

void CRepoStocksListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMIA->iItem,nSubItem = pNMIA->iSubItem;
	
	if(GetColID(nSubItem) == REPOSTOCK_DISCREPANCY)
	{
		if(nItem < m_pStockItems.GetSize() && pNMIA->ptAction.x < 16)
		{
			StockEquivalenceItem* pItem = m_pStockItems.GetAt(nItem);
			pItem->SetDoUpdate(!pItem->IsDoUpdate());			
			Invalidate(FALSE);
		}
	}
	*pResult = 0;
}