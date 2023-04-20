#ifndef __EXPENSE_H__
#define __EXPENSE_H__

#include "RawData.h"
#include "ExpenseCategory.h"
#include "ExpenseCategoryArray.h"

#include "DataMember.h"
#include "DataMembersDefs.h"
#include "DataMemberPtr.h"

#define NULL_EXPENSEID (0)

class Expense : public RawData
{
public:

	enum EXPENSE_TYPE
	{
		EXPENSE_TYPE_NULL = '\0',
		EXPENSE_TYPE_WITH_INVOICE = 'F',
		EXPENSE_TYPE_NON_INVOICE = '$'
	};

typedef DataMember<EXPENSE_TYPE,EXPENSE_TYPE> ExpenseTypeDataMember;
typedef DataMemberPtr<ExpenseCategoryPtr> ExpenseCategoryDataMember;
//class ExpenseCategoryDataMember : public DataMember<ExpenseCategoryPtr,ExpenseCategoryPtr>
//{
//public:
//	ExpenseCategoryDataMember(ExpenseCategoryPtr initValue = ExpenseCategoryPtr()) : DataMember<ExpenseCategoryPtr,ExpenseCategoryPtr>(initValue) {};
//	virtual void Rollback()
//	{
//		DataMember<ExpenseCategoryPtr,ExpenseCategoryPtr>::Rollback();
//		m_previousValue = ExpenseCategoryPtr();
//			
//	}
//	virtual void Commit()
//	{
//		DataMember<ExpenseCategoryPtr,ExpenseCategoryPtr>::Commit();
//		m_previousValue = ExpenseCategoryPtr();
//	}
//};

	Expense(UINT Id);
	~Expense();
	virtual void CopyFrom(const Expense& copy);
	

	UINT				GetId() const;
	Money				GetGrosValue() const;
	double				GetVATRate() const;
	EXPENSE_TYPE		GetExpenseType() const;
	ExpenseCategoryPtr	GetExpenseCategory() const;
	CString				GetInvoiceNo() const;
	CDateTime			GetExpenseDate() const;
	CString				GetExtras() const;
	CString				GetExpenseTitle() const;

	void				SetGrosValue(const Money& dGrosValue);
	void				SetVATRate(double dVATRate);
	void				SetExpenseType(EXPENSE_TYPE	tExpenseType);
	void				SetExpenseCategory(ExpenseCategoryPtr ptrExpenseCategory);
	void				SetInvoiceNo(const CString& sInvoiceNo);
	void				SetExpenseDate(const CDateTime& dtExpenseDate);
	void				SetExtras(const CString& sExtras);
	void				SetExpenseTitle(const CString& sExpenseTitle);

	void				ChangeGrosValue(const Money& dGrosValue);
	void				ChangeVATRate(double dVATRate);
	void				ChangeExpenseType(EXPENSE_TYPE	tExpenseType);
	void				ChangeExpenseCategory(ExpenseCategoryPtr ptrExpenseCategory);
	void				ChangeInvoiceNo(const CString& sInvoiceNo);
	void				ChangeExpenseDate(const CDateTime& dtExpenseDate);
	void				ChangeExtras(const CString& sExtras);
	void				ChangeExpenseTitle(const CString& sExpenseTitle);

	void CommitChanges();
	void RollbackChanges();
	bool IsUncommited();

	bool InsertThisExpense(bool bCommit = true);
	bool UpdateThisExpense(bool bCommit = true);
	bool DeleteThisExpense(bool bCommit = true);	

protected:	
	UINT						expenseid;
	MoneyDataMember				grosvalue;
	FloatDataMember				vatrate;
	ExpenseTypeDataMember		expensetype;
	ExpenseCategoryDataMember	expensecategory;
	StringDataMember			invoiceno;
	DateTimeDataMember			expensedate;
	StringDataMember			extras;
	StringDataMember			expensetitle;

	TransactedDataMemberArray transactedMembers;
	UpatableDataMembers updatebleMembers;
};


class ExpenseTypeStr
{
private:
	ExpenseTypeStr();
	bool m_bLoaded;
	CString m_expenseTypeInvoice;
	CString m_expenseTypeCash;
public:
	static CString GetExpenseTypeStr(Expense::EXPENSE_TYPE expenseType);
};


#endif