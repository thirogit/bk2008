#include "stdafx.h"
#include "SimpleHentCombo.h"
#include "../global_arrays.h"
#include "../consts.h"

CSimpleHentCombo::CSimpleHentCombo(bool bIclMissingItem/* = false*/) : CBasePtrCombo<CTTComboBox,Hent>(ary_hents.GetSize() + (bIclMissingItem ? 1 : 0)),
															m_bIclMissingItem(bIclMissingItem)
{
}

Hent *CSimpleHentCombo::GetItemText(int item,CString &itemText)
{

	//LOG("item = %d, m_bIclMissingItem = %d",item,m_bIclMissingItem);
	int nItem = item;
	if(m_bIclMissingItem)
	{
		if(nItem == 0)
		{
			itemText = szMissing;
			return (Hent*)NULL;
		}
		nItem--;
	}

	Hent *itemHent = ary_hents[nItem];
	itemText = itemHent->GetComboItemText();

	return itemHent;
}

int CSimpleHentCombo::GetNewItemCount()
{
	return ary_hents.GetSize() + (m_bIclMissingItem ? 1 : 0);

}

