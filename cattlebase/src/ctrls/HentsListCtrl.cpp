#include "stdafx.h"
#include "HentsListCtrl.h"
#include "../consts.h"

BEGIN_MESSAGE_MAP(CHentsListCtrl, CBaseListCtrl)
	
	
END_MESSAGE_MAP()


CHentsListCtrl::CHentsListCtrl(CHentArray *pHentArray,
									 const GRIDHEADERS *ppHeaders,
									 int iConfigId/* = -1*/) : 
	CBaseListCtrl(ppHeaders,iConfigId) ,m_pHentArray(pHentArray),m_textProvider(*pHentArray)
{	
}

void CHentsListCtrl::SortByCol(int Column,bool bAsc)
{
   m_pHentArray->SortBy(GetColID(Column),bAsc);
}


void CHentsListCtrl::UpdateItemCount()
{
	SetItemCount(m_pHentArray->GetSize());
}
	
void CHentsListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{
	m_textProvider.GetText(nItem,GetColID(nSubItem),pszItemText);
}



