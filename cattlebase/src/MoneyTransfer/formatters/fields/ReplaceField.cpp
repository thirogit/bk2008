#include "stdafx.h"
#include "ReplaceField.h"



ReplaceRule::ReplaceRule(char cWhat,char cWith) : m_cWhat(cWhat),m_cWith(cWith)
{
}

ReplaceRule::ReplaceRule(const ReplaceRule& src)
{
	CopyFrom(src);	
}

ReplaceRule& ReplaceRule::operator=(const ReplaceRule& right)
{
	CopyFrom(right);
	return *this;
}

void ReplaceRule::CopyFrom(const ReplaceRule& src)
{
	m_cWhat = src.m_cWhat;
	m_cWith = src.m_cWith;
}

char ReplaceRule::ApplyRule(char ch)
{
	if(ch == m_cWhat)
		return m_cWith;

	return ch;
}

ReplaceRules::ReplaceRules()
{
}

ReplaceRules::ReplaceRules(const ReplaceRules& src)
{
	CopyFrom(src);	
}

ReplaceRules& ReplaceRules::operator=(const ReplaceRules& right)
{
	CopyFrom(right);
	return *this;
}

void ReplaceRules::CopyFrom(const ReplaceRules& src)
{
	m_rules.clear();
	m_rules = src.m_rules;
	
}

ReplaceRules& ReplaceRules::Add(const ReplaceRule& replaceRule)
{
	m_rules.push_back(replaceRule);
	return *this;
}

char ReplaceRules::ApplyRules(char ch)
{
	char chAfterRules = ch;
	std::list<ReplaceRule>::iterator ruleIt = m_rules.begin();
	while(ruleIt != m_rules.end())
	{
		chAfterRules = ruleIt->ApplyRule(chAfterRules);
		if(chAfterRules == '\0')
			return chAfterRules;
		ruleIt++;
	}

	return chAfterRules;
}


ReplaceField::ReplaceField(FormatterField* pField,const ReplaceRules& rules) : WrapperField(pField),m_replaceRules(rules)
{
}

CString ReplaceField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	CString sValue = WrapperField::GetFieldValue(pContext,pMoneyTransfer);

	CString sResult;
	char ch;
	for(int i = 0;i < sValue.GetLength();i++)
	{
		ch = m_replaceRules.ApplyRules(sValue.GetAt(i));
		if(ch != '\0')
		{
			sResult.AppendChar(ch);
		}
	}

	return sResult;
}

