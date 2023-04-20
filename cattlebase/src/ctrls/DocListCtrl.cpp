#include "stdafx.h"
#include "DocListCtrl.h"
#include "../consts.h"
#include "../datatypes/DocDataComparator.h"

BEGIN_MESSAGE_MAP(CDocListCtrl, CBaseListCtrl)
	
	
END_MESSAGE_MAP()


GRIDHEADERS CDocListCtrl::DocHeader[] =
{
	{IDS_HEADERDOCNO,DOCS_SORT_BY_NUMBER},
	{IDS_HEADERDOCDATE,DOCS_SORT_BY_DOCDATE},
	{IDS_HEADERHENT,DOCS_SORT_BY_HENT},
	{IDS_HEADERAGENT,DOCS_MEMBER_AGENT},
	{IDS_HEADERINOUTDATE,DOCS_SORT_BY_LOADDATE},
	{IDS_HEADERPLATENO,DOCS_SORT_BY_PLATENO},
	{IDS_HEADERAMOUNT,DOCS_SORT_BY_COWCOUNT},
	{0,0}
};

CDocListCtrl::CDocListCtrl(CDocDataArray *docArray,int iConfigId/* = -1*/) :
CBaseListCtrl(DocHeader,iConfigId) ,m_docArray(docArray), m_textProvider(*m_docArray)
{
}

CDocListCtrl::CDocListCtrl(CDocDataArray *docArray,const GRIDHEADERS *ppHeaders,int iConfigId/* = -1*/) : 
CBaseListCtrl(ppHeaders,iConfigId) ,m_docArray(docArray), m_textProvider(*m_docArray)
{	
}

void CDocListCtrl::SortByCol(int Column,bool bAsc)
{
   m_docArray->SortBy(GetColID(Column),bAsc);
}


void CDocListCtrl::UpdateItemCount()
{
	SetItemCount(m_docArray->GetSize());
}
	
void CDocListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{
	m_textProvider.GetText(nItem,GetColID(nSubItem),pszItemText);
}



