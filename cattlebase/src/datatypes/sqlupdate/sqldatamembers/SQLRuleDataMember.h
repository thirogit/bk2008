#ifndef __SQLRULEDATAMEMBER_H__
#define __SQLRULEDATAMEMBER_H__

#include "../../Stock.h"
#include "../SQLDataMember.h"

class SQLRuleDataMember : public SQLDataMember<RuleDataMember>
{
public:
	SQLRuleDataMember(RuleDataMember* pRuleMember);
	virtual CString GetSQLStringValue(RuleDataMember* pRuleMember);	
};


#endif