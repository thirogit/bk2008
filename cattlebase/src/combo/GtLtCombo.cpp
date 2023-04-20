#include "stdafx.h"
#include "GtLtCombo.h"

void CGtLtCombo::InitCombo(UINT select/* = GTLT_NONE*/)
{
	const int gtltSize = 2;
	UINT gtltItems[gtltSize][2] = 
	{
		{IDS_GT,IT_IS_GT},
		{IDS_LT,IT_IS_LT}
	};

	ResetContent();

	int item, nSel = -1;
	CString itemText;

	for(int i = 0;i < gtltSize;i++)
	{
		itemText.LoadString(gtltItems[i][0]);
		item = AddString(itemText);
		SetItemData(item,gtltItems[i][1]);
		if(gtltItems[i][1] == select)
			nSel = item;
		else
		{
			if(item <= nSel) nSel++;
		}
	}	
	SetCurSel(nSel);
}
UINT CGtLtCombo::GetSelGtLt()
{
	int nSel = GetCurSel();
	if(nSel >= 0)
		return (UINT)GetItemData(nSel);

	return GTLT_NONE;
}
void CGtLtCombo::SelGtLt(UINT gtlt)
{
	for(int i = 0,s = GetCount(); i < s;i++)
	{
		if(gtlt == (UINT)GetItemData(i))
		{
			SetCurSel(i);
			break;
		}
	}
}


