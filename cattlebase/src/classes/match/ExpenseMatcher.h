#ifndef _EXPENSEMATCHER_H_
#define _EXPENSEMATCHER_H_

#include "RawDataMatcher.h"
#include "../../datatypes/Expense.h"
#include "MatchNullInt.h"
//#include "MatchDate.h"

class CExpenseDataMatcher : public CRawDataMatcher<Expense>
{
public:
	void FindGrosValue(double grosValue,CMP_ACTION_TYPE cmpAction);
	double GetGrosValue(Expense *pExpense);

	void FindVATRate(double vatRate,CMP_ACTION_TYPE cmpAction);
	double GetVATRate(Expense *pExpense);

	void FindExpenseType(Expense::EXPENSE_TYPE expenseType);
	Expense::EXPENSE_TYPE GetExpenseType(Expense *pExpense);
	
	void FindInvoiceNo(const CString& invoiceNo);
	CString GetInvoiceNo(Expense *pExpense);

	void FindExpenseTitle(const CString& expenseTitle);
	CString GetExpenseTitle(Expense *pExpense);
	
	void FindExtras(const CString& extras);
	CString GetExtras(Expense *pExpense);
	
	void FindExpenseDate(const CDateTime& expenseDt1,const CDateTime& expenseDt2,TIMESPAN spanType);
	CDateTime GetExpenseDate(Expense *pExpense);

	void FindExpenseCategory(ExpenseCategory* expenseCategory);
	ExpenseCategory* GetExpenseCategory(Expense *pExpense);
};




#endif


	
	





