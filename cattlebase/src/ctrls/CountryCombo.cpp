#include "stdafx.h"
#include "CountryCombo.h"
#include "global_arrays.h"

BEGIN_MESSAGE_MAP(CCountryCombo,CComboBoxSuper)
	
END_MESSAGE_MAP()


CCountryCombo::CCountryCombo() : CComboBoxSuper()
{
	
}

void CCountryCombo::InitCountryCombo(int iCountryPresel /*= DEFAULTCOUNTRY*/)
{
	SetColumnCount(3);

	CDC *pDC = GetDC();
	TEXTMETRIC txtMetric;
	int cx = pDC->GetTextMetrics(&txtMetric);
	ReleaseDC(pDC);
	
	SetColumnWidth(0, 100);
	SetColumnWidth(1, txtMetric.tmAveCharWidth*3 + 6);
	SetColumnWidth(2, txtMetric.tmAveCharWidth*2 + 6);

	SetImageList(&m_flagsRepo.GetFlagsImageList());

	int i,s,def = 0,item;
 	Country *cc;
	UINT flagImgIndex;

	for(i=0,s=ary_countries.GetSize();i<s;i++)
	{
		cc = ary_countries[i];
		item = AddString(cc->GetCountryName());
		if(cc->GetCountryISONumber() == iCountryPresel) def = item;
		SetItemText(item,1,cc->GetCountryCode3A());
		SetItemText(item,2,cc->GetCountryCode2A());
		SetItemData(item,(DWORD_PTR)cc);

		//if(cc->IsCountryInEU()) SetItemImage(item, 0);

		flagImgIndex = m_flagsRepo.GetFlagImageIndex(cc->GetCountryISONumber());
		SetItemImage(item, flagImgIndex);
		
	}

	SetCurSel(def);
	
	
}
Country* CCountryCombo::GetSelectedCountry()
{
	int selItem = GetCurSel();
	if(selItem >= 0)
		return (Country*)GetItemData(selItem);

	return NULL;
}