#include "stdafx.h"
#include "ExpenseDispatcher.h"

void ExpenseDispatcher::Dispatch(Expense *pExpense)
{

	ary_expenses.Add(pExpense);
	UpdateGrid(EXPENSESGRID);
}