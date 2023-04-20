#include "stdafx.h"
#include "SexCombo.h"
#include "../classes/types/CowSex.h"
#include "global_arrays.h"
/*
BEGIN_MESSAGE_MAP(CSexCombo,CComboBox)
	ON_WM_CREATE()
END_MESSAGE_MAP()
*/
/*
//CSexCombo::CSexCombo() : CComboBox()
//{
//}

void CSexCombo::PreSubclassWindow()
{
	ASSERT(!(GetStyle() & CBS_SORT));
	CComboBox::PreSubclassWindow();
}

int CSexCombo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ASSERT(!(lpCreateStruct->style & CBS_SORT));
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}*/

void CSexCombo::ReloadCombo(COWSEX select)
{

	ResetContent();

	//ModifyStyle(CBS_SORT,0,0);

	int item, nSel = -1;

	for(int i = 0;i < COWSEXES_SIZE;i++)
	{
		item = AddString((CowSexes)[i].szCowSexName);
		SetItemData(item,(CowSexes)[i].nSexCode);
		if((CowSexes)[i].nSexCode == select) nSel = item;
	}
	
	SetCurSel(nSel);

}

COWSEX CSexCombo::GetSelSex()
{
	return (COWSEX)GetItemData(GetCurSel());
}

void CSexCombo::SelSex(COWSEX sex)
{
	for(int i = 0,s = GetCount(); i < s;i++)
	{
		if(sex == (COWSEX)GetItemData(i))
		{
			SetCurSel(i);
			break;
		}
	}
}


BEGIN_MESSAGE_MAP(CFindSexCombo,CBaseFindCombo<CSexCombo>)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CFindSexCombo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if (CSexCombo::OnCreate(lpCreateStruct) == -1)	return -1;
	InitFindCombo();
	return 0;

}
void CFindSexCombo::PreSubclassWindow()
{
	CSexCombo::PreSubclassWindow();
	InitFindCombo();
}
