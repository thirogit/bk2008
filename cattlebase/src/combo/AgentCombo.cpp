#include "stdafx.h"
#include "AgentCombo.h"
#include "../global_arrays.h"

CAgentCombo::CAgentCombo() : CBasePtrCombo<CComboBox,Agent>(ary_agents.GetSize())
{
}

Agent* CAgentCombo::GetItemText(int item,CString &itemText)
{
	Agent* pAgent = ary_agents[item];
	itemText = pAgent->GetAgentCode();
	return pAgent;
}
