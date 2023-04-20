#ifndef __NEWEXPENSEDLG_H__
#define __NEWEXPENSEDLG_H__

#include "ExpenseDlg.h"

class CNewExpenseDlg : public CExpenseDlg
{
public:
	CNewExpenseDlg(CWnd* pParent = NULL);
protected:
	virtual CString GetWindowCaption();
	virtual void InitDialogControls();
	virtual bool CommitExpense();
};


#endif