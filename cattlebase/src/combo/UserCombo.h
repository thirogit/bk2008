#ifndef __USERCOMBO_H__
#define __USERCOMBO_H__

#include "../DataTypes.h"
#include "BaseCombo.h"

class CUserCombo : public CBasePtrCombo<CComboBox,UserData>
{
public:
	CUserCombo();
protected:
	UserData *GetItemText(int item,CString &itemText);

};

typedef CBaseFindCombo<CUserCombo> CFindUserCombo;

#endif