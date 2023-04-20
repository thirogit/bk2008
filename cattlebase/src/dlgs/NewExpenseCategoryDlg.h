#ifndef __NEWEXPENSECATEGORYDLG_H__
#define __NEWEXPENSECATEGORYDLG_H__

#include "ExpenseCategoryDlg.h"

class CNewExpenseCategoryDlg : public CExpenseCategoryDlg
{
public:
	CNewExpenseCategoryDlg(CWnd* pParent = NULL);
	ExpenseCategory *GetNewExpenseCategory();
protected:
	virtual CString GetWindowCaption();
	virtual bool CommitExpenseCategory();
protected:
	ExpenseCategory *m_NewExpenseCategory;
};



#endif