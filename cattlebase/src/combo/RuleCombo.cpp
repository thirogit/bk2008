#include "stdafx.h"
#include "RuleCombo.h"
#include "global_arrays.h"



CRuleCombo::CRuleCombo(bool bHasNullItem/* = true*/) : m_bHasNullItem(bHasNullItem), CBasePtrCombo<CComboBox,Rule>(ary_rules.GetSize())
{
}
void CRuleCombo::ReloadCombo(Rule *curSel/* = NULL*/)
{
	CBasePtrCombo<CComboBox,Rule>::ReloadCombo(curSel);
	int item = -1;
	if(m_bHasNullItem)
	{
		item = AddString("-");
		SetItemData(item,(DWORD_PTR)NULL);
		if(!curSel) 
			SetCurSel(item);
		else 
			SelectString(-1,curSel->GetRuleName()); //restore curSel it couldd have been changed by adding "-"
	}

}
Rule *CRuleCombo::GetItemText(int item,CString &itemText)
{
	Rule *itemRule = ary_rules[item];
	itemText = itemRule->GetRuleName();
	return itemRule;
}