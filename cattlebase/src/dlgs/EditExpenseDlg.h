#ifndef __EDITEXPENSEDLG_H__
#define __EDITEXPENSEDLG_H__

#include "ExpenseDlg.h"

class CEditExpenseDlg : public CExpenseDlg
{
public:
	CEditExpenseDlg(Expense* pExpense,CWnd* pParent = NULL);
protected:
	virtual CString GetWindowCaption();
	virtual void InitDialogControls();
	virtual bool CommitExpense();
	void UpdateExpense(Expense *pExpense);
protected:
	Expense *m_pEditExpense;
};


#endif