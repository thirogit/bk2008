#include "stdafx.h"
#include "ExpenseCategoryCombo.h"
#include "../global_arrays.h"

CExpenseCategoryCombo::CExpenseCategoryCombo() : CBasePtrCombo<CComboBox,ExpenseCategory>(ary_expensecategories.GetSize())
{
}

ExpenseCategory* CExpenseCategoryCombo::GetItemText(int item,CString &itemText)
{
	ExpenseCategoryPtr expenseCategoryPtr = ary_expensecategories[item];
	itemText = expenseCategoryPtr->GetExpenseCategoryName();
	return expenseCategoryPtr.GetPtr();
}
