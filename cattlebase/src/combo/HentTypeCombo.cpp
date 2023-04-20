#include "stdafx.h"
#include "HentTypeCombo.h"
#include "global_arrays.h"



CHentTypeCombo::CHentTypeCombo() : CBasePtrCombo<CComboBox,HentType>(ary_htypes.GetSize())
{
}

HentType *CHentTypeCombo::GetItemText(int item,CString &itemText)
{
	HentType *itemHType = ary_htypes[item];
	itemText = itemHType->henttypename;
	return itemHType;
}
