#ifndef __SIMPLEHENTCOMBO_H__
#define __SIMPLEHENTCOMBO_H__

#include "../DataTypes.h"
#include "BaseCombo.h"
#include "TTComboBox.h"

template CBasePtrCombo<CTTComboBox,Hent>;

class CSimpleHentCombo : public CBasePtrCombo<CTTComboBox,Hent>
{
public:
	CSimpleHentCombo(bool bIclMissingItem = false);
protected:
	Hent *GetItemText(int item,CString &itemText);
	int GetNewItemCount();
	bool m_bIclMissingItem;
	
};

class CFindHentCombo : public CBaseFindCombo<CSimpleHentCombo>
{
public:
	CFindHentCombo(bool bIncMissingItem = true) : CBaseFindCombo<CSimpleHentCombo>()
	{
		m_bIclMissingItem = bIncMissingItem;	
	};

};


#endif