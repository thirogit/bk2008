#include "stdafx.h"
#include "ExpenseCategoryArray.h"


ExpenseCategoryPtr CExpenseCategoryArray::GetExpenseCategory(int expenseCategoryId)
{
	for(int i = 0;i < GetSize();i++)
	{
		ExpenseCategoryPtr expenseCategory = GetAt(i);
		if(expenseCategory->GetId() == expenseCategoryId)
			return expenseCategory;
	}

	return ExpenseCategoryPtr();
}