#ifndef __REPLACEFIELD_H__
#define __REPLACEFIELD_H__

#include "WrapperField.h"
#include "../../../PtrFlushArray.h"
#include <list>
class ReplaceRule
{
public:
	ReplaceRule(char cWhat,char cWith);
	ReplaceRule(const ReplaceRule& src);
	ReplaceRule& operator=(const ReplaceRule& right);
	void CopyFrom(const ReplaceRule& src);
	char ApplyRule(char ch);
private:
	char m_cWhat;
	char m_cWith;
};

class ReplaceRules
{
public:
	ReplaceRules();
	ReplaceRules(const ReplaceRules& src);
	ReplaceRules& operator=(const ReplaceRules& right);
	void CopyFrom(const ReplaceRules& src);
	ReplaceRules& Add(const ReplaceRule& replaceRule);
	char ApplyRules(char ch);
private:
	std::list<ReplaceRule> m_rules;
};


class ReplaceField : public WrapperField
{
public:
	ReplaceField(FormatterField* pField,const ReplaceRules& rules);
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	ReplaceRules m_replaceRules;
};


#endif