#ifndef __SQLAGENTDATAMEMBER_H__
#define __SQLAGENTDATAMEMBER_H__

#include "../../Doc.h"
#include "../SQLDataMember.h"

class SQLAgentDataMember : public SQLDataMember<AgentDataMember>
{
public:
	SQLAgentDataMember(AgentDataMember* pAgentMember);
	virtual CString GetSQLStringValue(AgentDataMember* pAgentMember);	
};


#endif