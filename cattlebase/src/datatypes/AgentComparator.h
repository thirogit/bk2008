#ifndef __AGENTCOMPARATOR_H__
#define __AGENTCOMPARATOR_H__

#include "RawData.h"
#include "Agent.h"
#include "DataComparator.h"

enum AGENT_MEMBERS
{
	AGENT_MEMBER_NAME = RAWDATA_MEMBER_BASE+1,
	AGENT_MEMBER_CODE,
	AGENT_MEMBER_PLATE
};


class AgentComparator : public DataComparator<Agent>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,Agent* DataLeft,Agent* DataRight);
};


#endif