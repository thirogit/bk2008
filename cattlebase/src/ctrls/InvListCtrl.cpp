#include "stdafx.h"
#include "InvListCtrl.h"
#include "../consts.h"

BEGIN_MESSAGE_MAP(CInvListCtrl, CBaseListCtrl)
	
	
END_MESSAGE_MAP()


CInvListCtrl::CInvListCtrl(CInvoiceDataArray *invArray,const GRIDHEADERS *ppHeaders,int iConfigId/* = -1*/) : 
	CBaseListCtrl(ppHeaders,iConfigId) ,m_invArray(invArray),m_textProvider(*invArray)
{	
}

void CInvListCtrl::SortByCol(int Column,bool bAsc)
{
   m_invArray->SortBy(GetColID(Column),bAsc);
}


void CInvListCtrl::UpdateItemCount()
{
	SetItemCount(m_invArray->GetSize());
}
	
void CInvListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{
	m_textProvider.GetText(nItem,GetColID(nSubItem),pszItemText);
}



