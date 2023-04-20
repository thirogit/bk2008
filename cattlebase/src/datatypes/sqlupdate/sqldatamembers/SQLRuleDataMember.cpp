#include "stdafx.h"
#include "SQLRuleDataMember.h"

SQLRuleDataMember::SQLRuleDataMember(RuleDataMember* pRuleMember) :
		SQLDataMember(pRuleMember) 
{
}
 CString SQLRuleDataMember::GetSQLStringValue(RuleDataMember* pRuleMember)
 {
	Rule* pRule = pRuleMember->GetValue();
	if(pRule)
	{
		return FormatInt(pRule->GetId());
	}
	else
		return GetSQLNull();
	
}