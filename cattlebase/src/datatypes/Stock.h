#ifndef __STOCKDATA_H__
#define __STOCKDATA_H__

#include "Rule.h"
#include "DataMembersDefs.h"

typedef DataMember<Rule*,Rule*> RuleDataMember;

#define NULL_STOCKID (0)

class Stock
{
public:
	Stock(UINT Id = NULL_STOCKID);
	~Stock();
	void CopyFrom(const Stock &copy);

	bool InsertThisStock(bool bCommit = true);
	bool DeleteThisStock(bool bCommit = true);
	bool UpdateThisStock(bool bCommit = true);

	int	GetId() const;
	void ResetId();
	bool IsNew() const;

	CString			GetStockName() const;
	void			SetStockName(const CString& stockName);
	void			ChangeStockName(const CString& stockName);

	CString			GetStockCode() const;
	void			SetStockCode(const CString& stockCode);
	void			ChangeStockCode(const CString& stockCode);

	Money			GetMyPerKgPrice() const;
	void			SetMyPerKgPrice(const Money& dMyPerKgPrice);
	void			ChangeMyPerKgPrice(const Money& dMyPerKgPrice);

	CowSex			GetPredefSex() const;
	void			SetPredefSex(const CowSex& cowsex);
	void			ChangePredefSex(const CowSex& cowsex);

	CString			GetOfficialCode() const;
	void			SetOfficialCode(const CString& sOfficialCode);
	void			ChangeOfficialCode(const CString& sOfficialCode);

	Rule*			GetRule() const;
	void			SetRule(Rule* pRule);
	void			ChangeRule(Rule* pRule);

	void CommitChanges();
	void RollbackChanges();
	bool IsUncommited();

protected:
	StringDataMember	stockname;
	StringDataMember	stockcode;
	UINT				stockid;
	RuleDataMember		rule;
	MoneyDataMember		myperkgprice;
	CowSexDataMember	predefsex;
	StringDataMember	officialcode;

	TransactedDataMemberArray transactedMembers;
	UpatableDataMembers updatebleMembers;

};


#endif