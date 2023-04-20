#include "stdafx.h"
#include "RepoCowsListCtrl.h"

GRIDHEADERS CRepoCowsListCtrl::LIST_HEADER_COLUMNS[] =
{
	{IDS_HEADEREAN,			REPOCOW_MEMBER_COWNO},
	{IDS_HEADERSEX,			REPOCOW_MEMBER_COWSEX},
	{IDS_HEADERSTOCK,		REPOCOW_MEMBER_STOCK},
	{IDS_HEADERWEIGHT,		REPOCOW_MEMBER_WEIGHT},
	{IDS_HEADERNETTOPRICE,	REPOCOW_MEMBER_PRICE},	
	{0,0}
};

BEGIN_MESSAGE_MAP(CRepoCowsListCtrl, CBaseListCtrl)
	ON_WM_CREATE()
	//ON_NOTIFY_REFLECT(NM_CLICK , OnClick)	
	ON_WM_SIZE()
END_MESSAGE_MAP()


CRepoCowsListCtrl::CRepoCowsListCtrl(int iConfigId/* = -1*/) : CBaseListCtrl(LIST_HEADER_COLUMNS,iConfigId)
{	
}

void CRepoCowsListCtrl::SortByCol(int Column,bool bAsc)
{
//   m_pHentArray->SortBy(GetColID(Column),bAsc);
}

void CRepoCowsListCtrl::BeforeUpdate()
{
	Lock();
}

void CRepoCowsListCtrl::UpdateListWith(RepoInvoice* pInvoice)
{
	Lock();
	m_pCowItems.RemoveAll();
	if(pInvoice)
	{
		for(int i = 0,s = pInvoice->GetCowCount();i < s;i++)
		{
			m_pCowItems.Add(pInvoice->GetCow(i));
		}
	}
	UpdateItemCount();
	Unlock();
}

void  CRepoCowsListCtrl::UpdateItemCount()
{
	SetItemCount(m_pCowItems.GetCount());
}
	
void CRepoCowsListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{	
	UINT columnId = GetColID(nSubItem);
	RepoCow* pItem = m_pCowItems.GetAt(nItem);
	CString sText = m_TextProvider.GetDataText(pItem,columnId);
	strcpy(pszItemText,(LPCSTR)sText);
}

void CRepoCowsListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CBaseListCtrl::OnSize(nType,cx,cy);
}

//bool CRepoCowsListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
//{
//	if(GetColID(nSubItem) == REPOSTOCK_DISCREPANCY)
//	{
//		StockEquivalenceItem* pItem = m_pStockItems.GetAt(nItem);
//		Discrepancy<RepoStock> discrepancy(*pItem);
//
//		switch(discrepancy.GetType())
//		{
//			case MISSING_REMOTE:
//				*cellBkg = COLOR_MISSING_REMOTE_SIDE;
//				return true;
//			case MISSING_LOCAL:
//				*cellBkg = COLOR_MISSING_LOCAL_SIDE;
//				return true;
//			case HAS_BOTH:
//				if(!pItem->IsEqual())
//				{
//					*cellBkg = COLOR_BOTH_SIDES_NOT_EQUAL;
//					return true;
//				}
//				break;			
//		}
//	}
//	return false;
//}

//int CRepoCowsListCtrl::GetItemImage(int iItem,int Column)
//{
//	UINT columnId = GetColID(Column);
//
//	if(REPOSTOCK_DISCREPANCY == columnId)
//	{
//		StockEquivalenceItem* pItem = m_pStockItems.GetAt(iItem);
//		return (pItem->IsDoUpdate() ? 1 : 0);
//	}
//	return -1;
//}
//
//void CRepoCowsListCtrl::PostCreate()
//{
//	CBaseListCtrl::PostCreate();
//	m_checkboxImageList.Create(16,16,ILC_COLORDDB | ILC_MASK,2,1);
//	CBitmap checkBoxes;
//	checkBoxes.LoadBitmap(IDB_CHECKBOX);
//	m_checkboxImageList.Add(&checkBoxes,RGB(255,0,255));
//	SetImageList(&m_checkboxImageList,LVSIL_SMALL);
//}

//void CRepoCowsListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	int nItem = pNMIA->iItem,nSubItem = pNMIA->iSubItem;
//	
//	if(GetColID(nSubItem) == REPOSTOCK_DISCREPANCY)
//	{
//		if(nItem < m_pStockItems.GetSize() && pNMIA->ptAction.x < 16)
//		{
//			StockEquivalenceItem* pItem = m_pStockItems.GetAt(nItem);
//			pItem->SetDoUpdate(!pItem->IsDoUpdate());			
//			Invalidate(FALSE);
//		}
//	}
//	*pResult = 0;
//}