#include "stdafx.h"
#include "VersionFilesListCtrl.h"
#include "../MainCfg.h"
#include "resource.h"

#define IDCOL_VERFILE 1001

const GRIDHEADERS CVersionFilesListCtrl::szColHeaders[] =
{
	{IDS_HEADERVERFILE,IDCOL_VERFILE},	
	{0,0}
};


CVersionFilesListCtrl::CVersionFilesListCtrl(CStringArray &filesArray) : 
	CBaseListCtrl(szColHeaders,CFGID_VERFILELIST) ,m_filesArray(filesArray)
{	
}

void CVersionFilesListCtrl::SortByCol(int Column,bool bAsc)
{
   
}


void CVersionFilesListCtrl::UpdateItemCount()
{
	SetItemCount(m_filesArray.GetSize());
}
	
void CVersionFilesListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{
	if(nSubItem == 0)
	{
		strcpy(pszItemText,m_filesArray[nItem]);
	}
	
}



