#include "stdafx.h"
#include "RepoPurchaseHentsListCtrl.h"

GRIDHEADERS CRepoPurchaseHentsListCtrl::LIST_HEADER_COLUMNS[] =
{
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

BEGIN_MESSAGE_MAP(CRepoPurchaseHentsListCtrl, CBaseListCtrl)
	
END_MESSAGE_MAP()


CRepoPurchaseHentsListCtrl::CRepoPurchaseHentsListCtrl(int iConfigId/* = -1*/) : CBaseListCtrl(LIST_HEADER_COLUMNS,iConfigId)
{	
}

void CRepoPurchaseHentsListCtrl::SortByCol(int Column,bool bAsc)
{
//   m_pHentArray->SortBy(GetColID(Column),bAsc);
}

void CRepoPurchaseHentsListCtrl::BeforeUpdate()
{
	Lock();
}

void CRepoPurchaseHentsListCtrl::UpdateListWith(RepoPurchase* pPurchase)
{
	Lock();
	m_pHentItems.RemoveAll();

	if(pPurchase)
	{
		for(int i = 0,s = pPurchase->GetHentCount();i < s;i++)
		{
			m_pHentItems.Add(pPurchase->GetHent(i));
		}
	}
	UpdateItemCount();
	Unlock();
}


void  CRepoPurchaseHentsListCtrl::UpdateItemCount()
{
	SetItemCount(m_pHentItems.GetCount());
}
	
void CRepoPurchaseHentsListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{	
	UINT columnId = GetColID(nSubItem);

	RepoHent* pHent = m_pHentItems.GetAt(nItem);
	CString sText = m_TextProvider.GetDataText(pHent,columnId);
	strcpy(pszItemText,(LPCSTR)sText);
	

}



