#include "stdafx.h"
#include "ExpenseTypeCombo.h"


CExpenseTypeCombo::CExpenseTypeCombo() : CBaseCombo<CComboBox,Expense::EXPENSE_TYPE>(2)
{
}
Expense::EXPENSE_TYPE CExpenseTypeCombo::GetItemText(int item,CString &itemText)
{
	if(item == 0)
	{
		itemText = ExpenseTypeStr::GetExpenseTypeStr(Expense::EXPENSE_TYPE_WITH_INVOICE);
		return Expense::EXPENSE_TYPE_WITH_INVOICE;
	}
	else
	{
		itemText = ExpenseTypeStr::GetExpenseTypeStr(Expense::EXPENSE_TYPE_NON_INVOICE);
		return Expense::EXPENSE_TYPE_NON_INVOICE;
	}

}

void CExpenseTypeCombo::ReloadCombo(Expense::EXPENSE_TYPE curSel /*= Expense::EXPENSE_TYPE_NULL*/)
{
	CBaseCombo<CComboBox,Expense::EXPENSE_TYPE>::ReloadCombo(curSel);
}