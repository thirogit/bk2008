#ifndef __RULE_H__
#define __RULE_H__

#include "DataMember.h"
#include "DataMembersDefs.h"

#include "RulePart.h"

enum BuyOrSell
{
	BUY = 1,
	SELL = -1
};

#define NULL_RULEID (0)

class Rule 
{
public:
	Rule(UINT Id);
	~Rule();
	virtual void CopyFrom(const Rule& copy);

	UINT	 GetId() const;
	CString	 GetRuleName() const;
	CowSex	 GetNoSexValue() const;

	void SetRuleName(const CString& sRuleName);
	void SetNoSexValue(const CowSex& cowSex);

	void ChangeRuleName(const CString& sRuleName);
	void ChangeNoSexValue(const CowSex& cowSex);
		
	RulePart*	GetRulePart(BuyOrSell buy_sell);

	bool InsertThisRule(bool bCommit = true);
	bool DeleteThisRule(bool bCommit = true);
	bool UpdateThisRule(bool bCommit = true);

	void CommitChanges();
	void RollbackChanges();
	bool IsUncommited();

private:
	UINT				ruleid;
	StringDataMember	rulename;
	RulePart			buyrule;
	RulePart			sellrule;	
	CowSexDataMember	nosexvalue;

	TransactedDataMemberArray transactedMembers;
	UpatableDataMembers updatebleMembers;
};


#endif