#ifndef __EXPENSECATEGORYCOMBO_H__
#define __EXPENSECATEGORYCOMBO_H__

#include "BaseCombo.h"
#include "../datatypes/ExpenseCategory.h"

class CExpenseCategoryCombo : public CBasePtrCombo<CComboBox,ExpenseCategory>
{
public:
	CExpenseCategoryCombo();
	virtual ExpenseCategory* GetItemText(int item,CString &itemText);
};

typedef CBaseFindCombo<CExpenseCategoryCombo> CFindExpenseCategoryCombo;

#endif