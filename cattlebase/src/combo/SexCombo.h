#ifndef __SEXCOMBO_H__
#define __SEXCOMBO_H__
#include "../classes/types/CowSex.h"
#include "BaseCombo.h"

class CSexCombo : public CComboBox
{
public:
	
	virtual void ReloadCombo(COWSEX select = SEX_UNK);
	COWSEX GetSelSex();
	void SelSex(COWSEX sex);
	
};


class CFindSexCombo : public CBaseFindCombo<CSexCombo> 
{
protected:
	virtual void PreSubclassWindow();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP();
};

#endif