#include "stdafx.h"
#include "OutGrpCombo.h"
#include "DataTypes.h"

void COutGrpCombo::InitCombo(int grpSelected/* = NULL_INOUT_GRP*/)
{

	ResetContent();

	CString itemText;
	int item, nSel = 0;


	itemText.LoadString(IDS_NOINOUTGRP);
	item = AddString(itemText);
	SetItemData(item,NULL_INOUT_GRP);
	if(grpSelected == NULL_INOUT_GRP)
		nSel = item;

	for(int i = 0;i <= MAX_INOUT_GRP;i++)
	{
		itemText.Format("%d",i);
		item = AddString(itemText);
		SetItemData(item,i);
		if(item <= nSel) nSel++;
		if(i == grpSelected) nSel = item;
		
	}	
	SetCurSel(nSel);

}

int COutGrpCombo::GetSelOutGroup()
{
	return (int)GetItemData(GetCurSel());
}


