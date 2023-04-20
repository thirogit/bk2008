#include "stdafx.h"
#include "CurrencyCombo.h"
#include "global_arrays.h"

BEGIN_MESSAGE_MAP(CCurrencyCombo,CComboBoxSuper)
	
END_MESSAGE_MAP()


CCurrencyCombo::CCurrencyCombo() : CComboBoxSuper()
{
	
}


void CCurrencyCombo::InitCurrencyCombo(UINT idCountry/* = DEFAULTCOUNTRY*/)
{
	SetColumnCount(2);

	CDC *pDC = GetDC();
	TEXTMETRIC txtMetric;
	int cx = pDC->GetTextMetrics(&txtMetric);
	ReleaseDC(pDC);
	
	SetColumnWidth(0, 140);
	SetColumnWidth(1, txtMetric.tmAveCharWidth*3 + 6);
	
	SetImageList(&m_flagsRepo.GetFlagsImageList());

	int i,s,def = 0,item;
 	Country *cc;
	UINT flagImgIndex;
	CString sItemText;

	for(i=0,s=ary_countries.GetSize();i<s;i++)
	{
		cc = ary_countries[i];

		sItemText = cc->GetCountryCode3A();
		sItemText += " - ";
		sItemText += cc->GetCountryName();

		item = AddString(sItemText);
		if(cc->GetCountryISONumber() == idCountry) def = item;
		SetItemText(item,1,cc->GetCurrencyCode());
		
		SetItemData(item,(DWORD_PTR)cc);

		flagImgIndex = m_flagsRepo.GetFlagImageIndex(cc->GetCountryISONumber());
		SetItemImage(item, flagImgIndex);
		
	}

	SetCurSel(def);

}
Country* CCurrencyCombo::GetSelectedCountry()
{
	int selItem = GetCurSel();
	if(selItem >= 0)
		return (Country*)GetItemData(selItem);

	return NULL;
}
