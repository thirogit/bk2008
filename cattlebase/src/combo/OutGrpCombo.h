#ifndef __OUTGRPCOMBO_H__
#define __OUTGRPCOMBO_H__

#include "../datatypes/Cow.h"

class COutGrpCombo : public CComboBox
{
public:	
	virtual void InitCombo(int grpSelected = NULL_INOUT_GRP);
	int GetSelOutGroup();
	
};




#endif