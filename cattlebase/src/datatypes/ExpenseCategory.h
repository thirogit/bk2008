#ifndef __EXPENSECATEGORY_H__
#define __EXPENSECATEGORY_H__

#include "DataMember.h"
#include "DataMembersDefs.h"

#define NULL_EXPENSECATEGORYID (0)

class ExpenseCategory
{
public:
	ExpenseCategory(UINT Id);
	~ExpenseCategory();
	virtual void CopyFrom(const ExpenseCategory &copy);
	
	bool InsertThisExpenseCategory(bool bCommit = true);
	bool DeleteThisExpenseCategory(bool bCommit = true);
	bool UpdateThisExpenseCategory(bool bCommit = true);

	UINT	GetId() const;
	CString GetExpenseCategoryName() const;
	CString GetExpenseCategoryDesc() const;
	VATRate  GetDefaultVATRate() const;
	
	void SetExpenseCategoryName(const CString& sCategoryName);
	void SetExpenseCategoryDesc(const CString& sCategoryDesc);
	void SetDefaultVATRate(const VATRate& defaultVATRate);

	void ChangeExpenseCategoryName(const CString& sCategoryName);
	void ChangeExpenseCategoryDesc(const CString& sCategoryDesc);
	void ChangeDefaultVATRate(const VATRate& defaultVATRate);

	bool HasDefaultVATRate() const;

	void CommitChanges();
	void RollbackChanges();
	bool IsUncommited();
private:
	StringDataMember expensecategoryname;
	UINT expensecategoryid;
	StringDataMember expensecategorydesc;
	VATRateDataMember defaultvatrate;

	TransactedDataMemberArray transactedMembers;
	UpatableDataMembers updatebleMembers;
};

#endif