#include "stdafx.h"
#include "DataDiffList.h"
#include "../consts.h"
#include "../MainCfg.h"

const GRIDHEADERS CDataDiffList::DataDiffListCols[] = 
{
	{IDS_HEADER_VALUENAME,	COL_MEMBER_NAME},
	{IDS_HEADER_COPYTO,		COL_MEMBER_TO},
	{IDS_HEADER_COPYFROM,	COL_MEMBER_FROM},
	{0,						0}
};

BEGIN_MESSAGE_MAP(CDataDiffList,CBaseListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK , OnClick)
END_MESSAGE_MAP()

CDataDiffList::CDataDiffList(DiffMemberArray &diffList) : 
	CBaseListCtrl(DataDiffListCols,CFGID_DATADIFFLIST),m_diffList(diffList)		
{	
	//m_checkboxImageList.Create(IDB_CHECKBOX,16,1,RGB(255,0,255));	
	m_checkboxImageList.Create(16,16,ILC_COLORDDB | ILC_MASK,2,1);
	CBitmap checkBoxes;
	checkBoxes.LoadBitmap(IDB_CHECKBOX);
	m_checkboxImageList.Add(&checkBoxes,RGB(255,0,255));
	

}
void CDataDiffList::UpdateItemCount()
{
	SetItemCount(m_diffList.GetSize());
}

void CDataDiffList::PostCreate()
{
	CBaseListCtrl::PostCreate();
	SetImageList(&m_checkboxImageList,LVSIL_SMALL);
}

void CDataDiffList::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	CBaseDataDiffMerge *diff = m_diffList[nItem];
	
	CString temp;
	
	switch(GetColID(nSubItem))
	{				
		case COL_MEMBER_NAME:			
			temp = diff->GetMemberName();
		break;
		case COL_MEMBER_TO:
			temp = diff->GetMemeber_To_TextValue();			
		break;
		case COL_MEMBER_FROM:
			temp = diff->GetMemeber_From_TextValue();			
			break;
	}
	strcpy(pszItemText,(LPCSTR)temp);
}

int CDataDiffList::GetItemImage(int iItem,int Column)
{
	if(GetColID(Column) == COL_MEMBER_NAME)
	{
		return ((m_diffList[iItem])->GetDoCopy() ? 1 : 0);
	}
	return -1;
}

void CDataDiffList::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMIA->iItem,nSubItem = pNMIA->iSubItem;
	
	if(GetColID(nSubItem) == COL_MEMBER_NAME)
	{
		if(nItem < m_diffList.GetSize() && pNMIA->ptAction.x < 16)
		{
			CBaseDataDiffMerge *diff = m_diffList[nItem];
			diff->SetDoCopy(!diff->GetDoCopy());
			Invalidate();
		}
	}
	
	
	*pResult = 0;

}

