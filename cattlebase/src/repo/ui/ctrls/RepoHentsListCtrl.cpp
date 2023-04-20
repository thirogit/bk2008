#include "stdafx.h"
#include "RepoHentsListCtrl.h"
#include "../../Discrepancy.h"
#include "../RepoColors.h"

#define REPOHENT_DISCREPANCY (REPOHENT_MEMBER_MAX+1)

GRIDHEADERS CRepoHentsListCtrl::LIST_HEADER_COLUMNS[] =
{
	{IDS_HEADER_DISCREPANCY,REPOHENT_DISCREPANCY},
	{IDS_HEADERNAME,		REPOHENT_MEMBER_NAME},
	{IDS_HEADERARIMRNO,		REPOHENT_MEMBER_FARMNO},
	{IDS_HEADERALIAS,		REPOHENT_MEMBER_ALIAS},
	{IDS_HEADERSTREET,		REPOHENT_MEMBER_STREET},
	{IDS_HEADERPOBOX,		REPOHENT_MEMBER_POBOX},
	{IDS_HEADERCITY,		REPOHENT_MEMBER_CITY},
	{IDS_HEADERZIP,			REPOHENT_MEMBER_ZIP},
	{IDS_HEADERHENTTYPE,	REPOHENT_MEMBER_HENTTYPE},
	{0,0}
};

BEGIN_MESSAGE_MAP(CRepoHentsListCtrl, CBaseListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK , OnClick)	
END_MESSAGE_MAP()


CRepoHentsListCtrl::CRepoHentsListCtrl(int iConfigId/* = -1*/) : CBaseListCtrl(LIST_HEADER_COLUMNS,iConfigId)
{	
}

void CRepoHentsListCtrl::SortByCol(int Column,bool bAsc)
{
//   m_pHentArray->SortBy(GetColID(Column),bAsc);
}


void CRepoHentsListCtrl::UpdateListWith(HentsCluster* pHentsCluster)
{
	Lock();
	m_pHentItems.RemoveAll();

	if(pHentsCluster)
	{
		for(int i = 0,s = pHentsCluster->GetCount();i < s;i++)
		{
			m_pHentItems.Add(pHentsCluster->GetItem(i));
		}
	}
	UpdateItemCount();
	Unlock();
}

void CRepoHentsListCtrl::Select(Predicate<HentEquivalenceItem*>& selectPredicate,bool bAddToSelected/* = false*/)
{
	HentEquivalenceItem* pItem = NULL;
	PtrFlushArray<HentEquivalenceItem>::ArrayIterator it = m_pHentItems.iterator();
	while(it.hasNext())
	{
		pItem = *(it++);
		pItem->SetDoUpdate(selectPredicate.Evaluate(pItem) || (bAddToSelected && pItem->IsDoUpdate()));
	}
	Invalidate(FALSE);
}

void  CRepoHentsListCtrl::UpdateItemCount()
{
	SetItemCount(m_pHentItems.GetCount());
}
	
void CRepoHentsListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{	

	UINT columnId = GetColID(nSubItem);

	if(REPOHENT_DISCREPANCY != columnId)
	{
		HentEquivalenceItem* pItem = m_pHentItems.GetAt(nItem);
		Discrepancy<RepoHent> discrepancy(*pItem);
		CString sText = m_TextProvider.GetDataText(discrepancy.GetSide(),columnId);
		strcpy(pszItemText,(LPCSTR)sText);
	}

}

bool CRepoHentsListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{
	if(GetColID(nSubItem) == REPOHENT_DISCREPANCY)
	{
		HentEquivalenceItem* pItem = m_pHentItems.GetAt(nItem);
		Discrepancy<RepoHent> discrepancy(*pItem);

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

int CRepoHentsListCtrl::GetItemImage(int iItem,int Column)
{
	UINT columnId = GetColID(Column);

	if(REPOHENT_DISCREPANCY == columnId)
	{
		HentEquivalenceItem* pItem = m_pHentItems.GetAt(iItem);
		return (pItem->IsDoUpdate() ? 1 : 0);
	}
	return -1;
}

void CRepoHentsListCtrl::PostCreate()
{
	CBaseListCtrl::PostCreate();
	m_checkboxImageList.Create(16,16,ILC_COLORDDB | ILC_MASK,2,1);
	CBitmap checkBoxes;
	checkBoxes.LoadBitmap(IDB_CHECKBOX);
	m_checkboxImageList.Add(&checkBoxes,RGB(255,0,255));
	SetImageList(&m_checkboxImageList,LVSIL_SMALL);
}

void CRepoHentsListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMIA->iItem,nSubItem = pNMIA->iSubItem;
	
	if(GetColID(nSubItem) == REPOHENT_DISCREPANCY)
	{
		if(nItem < m_pHentItems.GetSize() && pNMIA->ptAction.x < 16)
		{
			HentEquivalenceItem* pItem = m_pHentItems.GetAt(nItem);
			pItem->SetDoUpdate(!pItem->IsDoUpdate());			
			Invalidate(FALSE);
		}
	}
	*pResult = 0;
}