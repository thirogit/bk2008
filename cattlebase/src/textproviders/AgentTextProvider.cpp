#include "stdafx.h"
#include "AgentTextProvider.h"

#include "../datatypes/AgentComparator.h"

	
CString AgentTextProvider::GetDataText(Agent *pData,UINT memberID)
{
	CString retStr;
   	switch(memberID)
	{
		case AGENT_MEMBER_NAME:
			retStr = pData->GetAgentName();
			break;
		case AGENT_MEMBER_CODE:
			retStr = pData->GetAgentCode();
			break;
		case AGENT_MEMBER_PLATE:
			retStr = pData->GetPlateNo();
			break;		
		default:
			ASSERT(0);
			break;
	}	
	return retStr;
}
