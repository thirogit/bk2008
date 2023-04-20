#ifndef __SQLEXPENSETYPEDATAMEMBER_H__
#define __SQLEXPENSETYPEDATAMEMBER_H__

#include "../../Expense.h"
#include "../SQLDataMember.h"

class SQLExpenseTypeDataMember : public SQLDataMember<Expense::ExpenseTypeDataMember>
{
public:
	SQLExpenseTypeDataMember(Expense::ExpenseTypeDataMember* pExpenseTypeMember);
	virtual CString GetSQLStringValue(Expense::ExpenseTypeDataMember* pExpenseTypeMember);	
};


#endif