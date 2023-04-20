#ifndef __EXPENSETYPECOMBO_H__
#define __EXPENSETYPECOMBO_H__

#include "BaseCombo.h"
#include "../datatypes/Expense.h"

class CExpenseTypeCombo : public CBaseCombo<CComboBox,Expense::EXPENSE_TYPE>
{
public:
	CExpenseTypeCombo();
	virtual Expense::EXPENSE_TYPE GetItemText(int item,CString &itemText);
	void ReloadCombo(Expense::EXPENSE_TYPE curSel = Expense::EXPENSE_TYPE_NULL);

};

typedef CBaseFindCombo<CExpenseTypeCombo> CFindExpenseTypeCombo;

#endif