#ifndef __SQLEXPENSECATEGORYDATAMEMBER_H__
#define __SQLEXPENSECATEGORYDATAMEMBER_H__

#include "../../Expense.h"
#include "../SQLDataMember.h"

class SQLExpenseCategoryDataMember : public SQLDataMember<Expense::ExpenseCategoryDataMember>
{
public:
	SQLExpenseCategoryDataMember(Expense::ExpenseCategoryDataMember* pExpenseCategoryMember);
	virtual CString GetSQLStringValue(Expense::ExpenseCategoryDataMember* pExpenseCategoryMember);
};


#endif