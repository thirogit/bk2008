#include "stdafx.h"
#include "AgentComparator.h"

COMPARE_RESULT AgentComparator::CompareBy(UINT dataMember,Agent* DataLeft,Agent* DataRight)
{
	Agent *left = DataLeft,*right = DataRight;
	
	switch(dataMember)
	{		
		case AGENT_MEMBER_NAME:
			return PolishStringCompare(left->GetAgentName(),right->GetAgentName());
		case AGENT_MEMBER_CODE:
			return PolishStringCompare(left->GetAgentCode(),right->GetAgentCode());		
		case AGENT_MEMBER_PLATE:
			return PolishStringCompare(left->GetPlateNo(),right->GetPlateNo());
		default:
			ASSERT(0);
	}

	return COMPARE_LESS;
}
