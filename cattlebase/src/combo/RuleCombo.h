#ifndef __RULECOMBO_H__
#define __RULECOMBO_H__

#include "DataTypes.h"
#include "BaseCombo.h"

/*
class CRuleCombo : public CComboBox
{
public:
	CRuleCombo(bool bHasNullItem = true);
	virtual void InitRuleCombo(Rule *curSel = NULL);
	Rule* GetSelRule();
	void Reload(Rule *curSel = NULL);
protected:
	bool m_bHasNullItem;
protected:
	DECLARE_MESSAGE_MAP();
};*/


class CRuleCombo : public CBasePtrCombo<CComboBox,Rule>
{
public:
	CRuleCombo(bool bHasNullItem = true);
	virtual void ReloadCombo(Rule *curSel = NULL);
protected:
	Rule *GetItemText(int item,CString &itemText);
	bool m_bHasNullItem;
};


#endif