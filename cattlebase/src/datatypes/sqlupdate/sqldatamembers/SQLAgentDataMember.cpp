#include "stdafx.h"
#include "SQLAgentDataMember.h"

SQLAgentDataMember::SQLAgentDataMember(AgentDataMember* pAgentMember) : SQLDataMember(pAgentMember) 
{
}
 CString SQLAgentDataMember::GetSQLStringValue(AgentDataMember* pAgentMember)
 {
	Agent* pAgent = pAgentMember->GetValue();
	if(pAgent != NULL)
	{
		return FormatInt(pAgent->GetId());
	}
	else
		return GetSQLNull();

	
}