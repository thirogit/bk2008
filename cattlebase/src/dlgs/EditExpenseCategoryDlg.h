#ifndef __EDITEXPENSECATEGORYDLG_H__
#define __EDITEXPENSECATEGORYDLG_H__

#include "ExpenseCategoryDlg.h"
#include "../datatypes/ExpenseCategory.h"

class CEditExpenseCategoryDlg : public CExpenseCategoryDlg
{
public:
	CEditExpenseCategoryDlg(ExpenseCategory *pCategory,CWnd* pParent = NULL);
protected:
	virtual bool CommitExpenseCategory();
	virtual CString GetWindowCaption();
	virtual void InitDialogControls();
	
	ExpenseCategory *m_pEditCategory;


};

#endif