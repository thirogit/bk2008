#ifndef __AGENTCOMBO_H__
#define __AGENTCOMBO_H__

#include "BaseCombo.h"
#include "../datatypes/Agent.h"

class CAgentCombo : public CBasePtrCombo<CComboBox,Agent>
{
public:
	CAgentCombo();
	virtual Agent* GetItemText(int item,CString &itemText);
};

typedef CBaseFindCombo<CAgentCombo> CFindAgentCombo;

#endif