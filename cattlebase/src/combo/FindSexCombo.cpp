#include "stdafx.h"
#include "FindSexCombo.h"
#include "../classes/types/CowSex.h"

#if 0
BEGIN_MESSAGE_MAP(CFindSexCombo,CComboBox)
	ON_WM_CREATE()
END_MESSAGE_MAP()

CFindSexCombo::CFindSexCombo() : CSexCombo() ,m_itemneutral(-1)
{
}

int CFindSexCombo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if (CSexCombo::OnCreate(lpCreateStruct) == -1)
		return -1;
	InitSexCombo(SEX_UNK/*this parametr isn't used*/);
	return 0;

}
void CFindSexCombo::PreSubclassWindow()
{
	CSexCombo::PreSubclassWindow();
	InitSexCombo(SEX_UNK/*this parametr isn't used*/);
}

void CFindSexCombo::InitSexCombo(COWSEX select)
{
	CSexCombo::InitSexCombo();
	m_itemneutral = InsertString(0,"*");
	SetCurSel(m_itemneutral);
}

bool CFindSexCombo::IsNeutral()
{
	int nSel = GetCurSel();
	if(nSel < 0) return false;
	return nSel == m_itemneutral;
}
#endif