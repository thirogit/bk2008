#include "stdafx.h"
#include "SQLExpenseTypeDataMember.h"



SQLExpenseTypeDataMember::SQLExpenseTypeDataMember(Expense::ExpenseTypeDataMember* pExpenseTypeMember) :
		 SQLDataMember<Expense::ExpenseTypeDataMember>(pExpenseTypeMember)
{
}
		 
CString SQLExpenseTypeDataMember::GetSQLStringValue(Expense::ExpenseTypeDataMember* pExpenseTypeMember)
{
	Expense::EXPENSE_TYPE expenseType = pExpenseTypeMember->GetValue();
	if(expenseType != Expense::EXPENSE_TYPE_NULL)
	{
		CString expenseTypeChar;
		expenseTypeChar += (char)expenseType;
		return QuoteString(expenseTypeChar);
	}
	else
		return GetSQLNull();
}