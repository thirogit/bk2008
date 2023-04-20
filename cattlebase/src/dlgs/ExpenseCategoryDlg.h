#ifndef __EXPENSECATEGORYDLG_H__
#define __EXPENSECATEGORYDLG_H__

#include "../datatypes/ExpenseCategory.h"
#include "../combo/VATCombo.h"

class CExpenseCategoryDlg : public CDialog
{
public:
	enum { IDD = IDD_EXPENSECATEGORY };
	CExpenseCategoryDlg(CWnd* pParent = NULL);
			
protected:
	
	CString m_expenseCategoryName;
	CString m_expenseCategoryDesc;
	VATRate	m_defaultVATRate;

	CVATCombo m_defaultVATRateCB;
	
	virtual bool	CommitExpenseCategory() = 0;
	virtual CString GetWindowCaption() = 0;
	virtual void InitDialogControls();
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog() ;
	virtual void OnOK();
	virtual bool ValidateFields();

	

};

#endif