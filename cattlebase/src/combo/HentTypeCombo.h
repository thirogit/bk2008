#ifndef __HENTTYPECOMBO_H__
#define __HENTTYPECOMBO_H__

#include "../DataTypes.h"
#include "BaseCombo.h"

class CHentTypeCombo : public  CBasePtrCombo<CComboBox,HentType>
{
public:
	CHentTypeCombo();	
protected:
	HentType *GetItemText(int item,CString &itemText);

};

typedef  CBaseFindCombo<CHentTypeCombo> CFindHentTypeCombo;

#endif