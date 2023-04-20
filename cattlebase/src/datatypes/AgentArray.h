#ifndef __AGENTARRAY_H__
#define __AGENTARRAY_H__

//#include "../RefCountingPtrArray.h"
#include "Agent.h"
#include "AgentComparator.h"
#include "../PtrFlushArray.h"
//typedef RefCountingPtr<Agent> AgentPtr;

class CAgentArray : public PtrFlushArray<Agent> 
{
public:
	Agent* GetAgentByAgentId(int Id);
};

#endif