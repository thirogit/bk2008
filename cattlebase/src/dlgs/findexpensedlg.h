#ifndef __FINDEXPENSEDLG_H__
#define __FINDEXPENSEDLG_H__


//#include "CattleSQL.h"
#include "findbasedlg.h"
#include "..\ctrls\NumFindCtrl.h"
#include "ctrls\DateSpan.h"
#include "..\combo\ExpenseTypeCombo.h"
#include "..\combo\ExpenseCategoryCombo.h"

class CFindExpenseDlg : public  CFindBaseDlg
{
public:
	CFindExpenseDlg();
	enum { IDD = IDD_FINDEXPENSE };


	CString m_byTitle;
	CFindExpenseTypeCombo m_ExpenseType;
	CString m_byInvoiceNo;
	BOOL	  m_bByExpenseDate;
	CDateSpan m_expenseDateSpan;
	CFloatFindCtrl  m_vatRate;
	CFloatFindCtrl  m_expenseGrosValue;
	CFindExpenseCategoryCombo		m_ExpenseCategory;

	int GoSearch(CFindSetup& setup);	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
protected:
	afx_msg void OnBtnByExpenseDate();
	
	DECLARE_MESSAGE_MAP()
};


#endif