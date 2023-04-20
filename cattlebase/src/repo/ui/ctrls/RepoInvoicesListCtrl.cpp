#include "stdafx.h"
#include "RepoInvoicesListCtrl.h"


GRIDHEADERS CRepoInvoicesListCtrl::LIST_HEADER_COLUMNS[] =
{
	{IDS_HEADERCUSTNO,		REPOINVOICE_MEMBER_CUSTOMNUMBER},
	{IDS_HEADERDOCDATE,		REPOINVOICE_MEMBER_INVOICEDATE},
	{IDS_HEADERINVOICETYPE,	REPOINVOICE_MEMBER_INVOICETYPE},
	{IDS_HEADERPAYWAY,		REPOINVOICE_MEMBER_PAYWAY},
	{IDS_HEADERHENT,		REPOINVOICE_MEMBER_INVOICEHENTALIAS},
	{IDS_HEADERAMOUNT,		REPOINVOICE_MEMBER_COWCOUNT},
	{IDS_HEADERNETTO,		REPOINVOICE_MEMBER_TOTALNET},
	{0,0}
};

BEGIN_MESSAGE_MAP(CRepoInvoicesListCtrl, CBaseListCtrl)
	ON_WM_CREATE()
	//ON_NOTIFY_REFLECT(NM_CLICK , OnClick)	
END_MESSAGE_MAP()


CRepoInvoicesListCtrl::CRepoInvoicesListCtrl(int iConfigId/* = -1*/) : CBaseListCtrl(LIST_HEADER_COLUMNS,iConfigId)
{	
}

void CRepoInvoicesListCtrl::SortByCol(int Column,bool bAsc)
{
   //m_pInvoiceItems.SortBy(GetColID(Column),bAsc);
}

void CRepoInvoicesListCtrl::BeforeUpdate()
{
	Lock();
}

RepoInvoice* CRepoInvoicesListCtrl::GetItemAt(int iItemIndex)
{
	return m_pInvoiceItems.GetAt(iItemIndex);
}

void CRepoInvoicesListCtrl::UpdateListWith(RepoPurchase* pPurchase)
{
	Lock();
	m_pInvoiceItems.RemoveAll();
	if(pPurchase != NULL)
	{
		for(int i = 0,s = pPurchase->GetInvoiceCount();i < s;i++)
		{
			m_pInvoiceItems.Add(pPurchase->GetInvoice(i));
		}
	}
	UpdateItemCount();
	Unlock();
}

void  CRepoInvoicesListCtrl::UpdateItemCount()
{
	SetItemCount(m_pInvoiceItems.GetCount());
}
	
void CRepoInvoicesListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{	
	UINT columnId = GetColID(nSubItem);
	
	RepoInvoice* pItem = m_pInvoiceItems.GetAt(nItem);		
	CString sText = m_TextProvider.GetDataText(pItem,columnId);
	strcpy(pszItemText,(LPCSTR)sText);
}

//bool CRepoInvoicesListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
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

//int CRepoInvoicesListCtrl::GetItemImage(int iItem,int Column)
//{
//	UINT columnId = GetColID(Column);
//
//	//if(REPOSTOCK_DISCREPANCY == columnId)
//	//{
//	//	StockEquivalenceItem* pItem = m_pStockItems.GetAt(iItem);
//	//	return (pItem->IsDoUpdate() ? 1 : 0);
//	//}
//	return -1;
//}

//void CRepoInvoicesListCtrl::PostCreate()
//{
//	CBaseListCtrl::PostCreate();
//	m_checkboxImageList.Create(16,16,ILC_COLORDDB | ILC_MASK,2,1);
//	CBitmap checkBoxes;
//	checkBoxes.LoadBitmap(IDB_CHECKBOX);
//	m_checkboxImageList.Add(&checkBoxes,RGB(255,0,255));
//	SetImageList(&m_checkboxImageList,LVSIL_SMALL);
//}

void CRepoInvoicesListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMIA->iItem,nSubItem = pNMIA->iSubItem;
	
	/*if(GetColID(nSubItem) == REPOSTOCK_DISCREPANCY)
	{
		if(nItem < m_pStockItems.GetSize() && pNMIA->ptAction.x < 16)
		{
			StockEquivalenceItem* pItem = m_pStockItems.GetAt(nItem);
			pItem->SetDoUpdate(!pItem->IsDoUpdate());			
			Invalidate(FALSE);
		}
	}*/
	*pResult = 0;
}