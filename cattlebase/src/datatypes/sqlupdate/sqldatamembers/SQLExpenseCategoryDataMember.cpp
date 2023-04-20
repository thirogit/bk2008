#include "stdafx.h"
#include "SQLExpenseCategoryDataMember.h"

SQLExpenseCategoryDataMember::SQLExpenseCategoryDataMember(Expense::ExpenseCategoryDataMember* pExpenseCategoryMember) :
			SQLDataMember<Expense::ExpenseCategoryDataMember>(pExpenseCategoryMember)
{
}
CString SQLExpenseCategoryDataMember::GetSQLStringValue(Expense::ExpenseCategoryDataMember* pExpenseCategoryMember)
{
	ExpenseCategoryPtr ptrExpenseCategory = pExpenseCategoryMember->GetValue();
	if(!ptrExpenseCategory.IsNull())
	{
		return FormatInt(ptrExpenseCategory->GetId());
	}
	else
		return GetSQLNull();

}