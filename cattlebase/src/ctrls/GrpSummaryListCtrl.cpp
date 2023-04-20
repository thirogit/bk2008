#include "stdafx.h"
#include "GrpSummaryListCtrl.h"


CGrpSummaryListCtrl::CGrpSummaryListCtrl(CowAccessor* pAccessor,CCowArray &summaryCows) : 
									CBaseListCtrl(szInOutGrpSummaryListCols,MAX_INOUT_GRP+1+1,-1),
									m_summaryCows(summaryCows),m_pAccessor(pAccessor)

{
	int i = 0;
	szInOutGrpSummaryListCols[i].nResID = IDS_NOGRPHEADER;
	szInOutGrpSummaryListCols[i].nColID = -1;

	i++;

	for(i = 1;i <= 1+(MAX_INOUT_GRP);i++)
	{
		szInOutGrpSummaryListCols[i].nResID = IDS_OUTGRPSUMMARYHEADERCOL;
		szInOutGrpSummaryListCols[i].nColID = i-1;
	}
	szInOutGrpSummaryListCols[i].nResID = 0;
	szInOutGrpSummaryListCols[i].nColID = 0;
}


void CGrpSummaryListCtrl::UpdateItemCount()
{
		SetItemCount(1);
}


void CGrpSummaryListCtrl::UpdateSummary()
{
	::ZeroMemory(m_inoutgrpSummaryTable,sizeof(m_inoutgrpSummaryTable));
	m_emptyGrpCount = 0;

	int grp;
	Cow *pCow = NULL;
	CCowArray::ArrayIterator it = m_summaryCows.iterator();
	NullInt cowgrp;
	while(it.hasNext())
	{
		pCow = *(it++);
		cowgrp = GetCowGroup(pCow);	
		if(cowgrp.IsNull())
			m_emptyGrpCount++;
		else
		{
			grp = cowgrp.ToInt();
			if(grp >= 0 && grp <= MAX_INOUT_GRP)
				m_inoutgrpSummaryTable[grp]++;
			else
				m_emptyGrpCount++;
		}
	}
	Invalidate();
}

void CGrpSummaryListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	UINT singleOutGrp;
	int colId = GetColID(nSubItem);
	if(colId == -1)
		singleOutGrp = m_emptyGrpCount;
	else
		singleOutGrp = m_inoutgrpSummaryTable[colId];

	sprintf(pszItemText,"%d",singleOutGrp);	
}

void CGrpSummaryListCtrl::SetListCols()
{
	CBaseListCtrl::SetListCols();
	
	CHeaderCtrl *pHeadCtrl = GetHeaderCtrl();
	int colCount = pHeadCtrl->GetItemCount();
	HDITEM hdItem;
	char colName[20];

	for(int i = 1;i < colCount;i++)
	{
		sprintf(colName,"%d",i-1);
		hdItem.mask = HDI_TEXT;
		hdItem.pszText = colName;
		pHeadCtrl->SetItem(i,&hdItem);
	}
	UpdateItemCount();

	SetExtendedStyle( LVS_EX_GRIDLINES);
	
}

NullInt CGrpSummaryListCtrl::GetCowGroup(Cow* pCow)
{
	return m_pAccessor->GetGroup(pCow);
}

