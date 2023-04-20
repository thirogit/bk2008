#include "stdafx.h"
#include "ExpensesListCtrl.h"
#include "../consts.h"

BEGIN_MESSAGE_MAP(CExpensesListCtrl, CBaseListCtrl)
	
	
END_MESSAGE_MAP()


CExpensesListCtrl::CExpensesListCtrl(CExpenseDataArray *pExpenseArray,
									 const GRIDHEADERS *ppHeaders,
									 int iConfigId/* = -1*/) : 
	CBaseListCtrl(ppHeaders,iConfigId) ,m_pExpenseArray(pExpenseArray),m_textProvider(*pExpenseArray)
{	
}

void CExpensesListCtrl::SortByCol(int Column,bool bAsc)
{
   m_pExpenseArray->SortBy(GetColID(Column),bAsc);
}


void CExpensesListCtrl::UpdateItemCount()
{
	SetItemCount(m_pExpenseArray->GetSize());
}
	
void CExpensesListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{
	m_textProvider.GetText(nItem,GetColID(nSubItem),pszItemText);
}



