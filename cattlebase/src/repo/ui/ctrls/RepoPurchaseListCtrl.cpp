#include "stdafx.h"
#include "RepoPurchaseListCtrl.h"
//#include "../../Discrepancy.h"
#include "../RepoColors.h"

GRIDHEADERS CRepoPurchaseListCtrl::LIST_HEADER_COLUMNS[] =
{
	{IDS_HEADERAGENT,			REPOPURCHASE_MEMBER_AGENTCODE},
	{IDS_HEADERPLATENO,			REPOPURCHASE_MEMBER_PLATENO},
	{IDS_HEADERSTARTDATETIME,	REPOPURCHASE_MEMBER_STARTDATE},	
	{IDS_HEADERAMOUNT,			REPOPURCHASE_MEMBER_COWCOUNT},	
	{IDS_HEADERDOWNLOADED,		REPOPURCHASE_MEMBER_WASDOWNLOADED},
	{0,0}
};

BEGIN_MESSAGE_MAP(CRepoPurchaseListCtrl, CBaseListCtrl)
	ON_WM_CREATE()
	
	//ON_NOTIFY_REFLECT(NM_CLICK , OnClick)	
END_MESSAGE_MAP()


CRepoPurchaseListCtrl::CRepoPurchaseListCtrl(int iConfigId/* = -1*/) : CBaseListCtrl(LIST_HEADER_COLUMNS,iConfigId)
{	
}

void CRepoPurchaseListCtrl::SortByCol(int Column,bool bAsc)
{
//   m_pHentArray->SortBy(GetColID(Column),bAsc);
}

void CRepoPurchaseListCtrl::BeforeUpdate()
{
	Lock();
}

void CRepoPurchaseListCtrl::Select(Predicate<PurchaseItem*>& selectPredicate,bool bAddToSelected/* = false*/)
{
	PurchaseItem* pItem = NULL;
	PtrFlushArray<PurchaseItem>::ArrayIterator it = m_pPurchaseItems.iterator();
	while(it.hasNext())
	{
		pItem = *(it++);
		pItem->SetDoUpdate(selectPredicate.Evaluate(pItem) || (bAddToSelected && pItem->IsDoUpdate()));
	}
	Invalidate(FALSE);
}

void CRepoPurchaseListCtrl::UpdateListWith(PurachesCluster* pPurchasesCluster)
{
	Lock();
	m_pPurchaseItems.RemoveAll();
	if(pPurchasesCluster)
	{
		for(int i = 0,s = pPurchasesCluster->GetCount();i < s;i++)
		{
			m_pPurchaseItems.Add(pPurchasesCluster->GetItem(i));
		}
	}
	UpdateItemCount();
	Unlock();
}

void  CRepoPurchaseListCtrl::UpdateItemCount()
{
	SetItemCount(m_pPurchaseItems.GetCount());
}
	
void CRepoPurchaseListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{	
	UINT columnId = GetColID(nSubItem);
	CString sText = m_TextProvider.GetDataText(m_pPurchaseItems.GetAt(nItem)->GetPurchase(),columnId);
	strcpy(pszItemText,(LPCSTR)sText);
}

bool CRepoPurchaseListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{
	//if(GetColID(nSubItem) == REPOPURCHASE_MEMBER_AGENTCODE)
	//{
		PurchaseItem* pItem = m_pPurchaseItems.GetAt(nItem);
		if(pItem->GetPurchase()->GetWasDownloaded())
		{
			*cellBkg = COLOR_PURCHASE_DOWNLOADED;
			return true;
		}
	//}
	return false;
}
//int CRepoPurchaseListCtrl::GetItemImage(int iItem,int Column)
//{
//	UINT columnId = GetColID(Column);
//
//	if(REPOPURCHASE_MEMBER_AGENTCODE == columnId)
//	{
//		PurchaseItem* pItem = m_pPurchaseItems.GetAt(iItem);
//		return (pItem->IsDoUpdate() ? 1 : 0);
//	}
//	return -1;
//}

//void CRepoPurchaseListCtrl::PostCreate()
//{
//	CBaseListCtrl::PostCreate();
//	m_checkboxImageList.Create(16,16,ILC_COLORDDB | ILC_MASK,2,1);
//	CBitmap checkBoxes;
//	checkBoxes.LoadBitmap(IDB_CHECKBOX);
//	m_checkboxImageList.Add(&checkBoxes,RGB(255,0,255));
//	SetImageList(&m_checkboxImageList,LVSIL_SMALL);
//}

//
//void CRepoPurchaseListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	int nItem = pNMIA->iItem,nSubItem = pNMIA->iSubItem;
//	
//	if(GetColID(nSubItem) == REPOPURCHASE_MEMBER_AGENTCODE)
//	{
//		if(nItem < m_pPurchaseItems.GetSize() && pNMIA->ptAction.x < 16)
//		{
//			PurchaseItem* pItem = m_pPurchaseItems.GetAt(nItem);
//			pItem->SetDoUpdate(!pItem->IsDoUpdate());			
//			Invalidate(FALSE);
//		}
//	}
//	*pResult = 0;
//}