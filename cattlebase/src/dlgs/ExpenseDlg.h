#ifndef __EXPENSEDLG_H__
#define __EXPENSEDLG_H__

#include "../datatypes/Expense.h"
#include "../ctrls/ExtrasMemo.h"
#include "../ctrls/curdateedit/curdateedit.h"
#include "../combo/VATCombo.h"
#include "../imgbutton/imagebutton.h"
#include "../combo/ExpenseTypeCombo.h"

class CExpenseDlg : public CDialog
{
public:
	enum { IDD = IDD_EXPENSE };
	CExpenseDlg(CWnd* pParent = NULL);
protected:
	CExpenseTypeCombo	m_expenseType;
	CString			m_invoiceNo;
	CCurDateEdit	m_invoiceDate;
	CVATCombo		m_vat;
	CExtrasMemo		m_extras;	
	CEnterEdit		m_expenseValueEdit;
	Money			m_expenseValue;
	VATRate			m_vatRate;
	CComboBox		m_expenseCategory;
	CString			m_expenseTitle;

	virtual bool CommitExpense() = 0;
	virtual CString GetWindowCaption() = 0;
	virtual void InitDialogControls();
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog() ;
	virtual void OnOK();
	void ReloadExpenseCategoryCombo(int expensecategoryid = -1);
	void ReloadExpenseTypeCombo(Expense::EXPENSE_TYPE expenseType = Expense::EXPENSE_TYPE_NULL);
	bool ValidateFields();
	Expense::EXPENSE_TYPE GetCurrentExpenseType();
	ExpenseCategoryPtr GetCurrentExpenseCategory();
	Expense* CreateExpense();


	CImageButton m_btncancel;
	CImageButton m_btnaction;

protected:
	afx_msg void OnExpenseTypeChange();
	afx_msg void OnExpenseValueComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExpenseVATComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExpenseCategoryChange();

	DECLARE_MESSAGE_MAP();

	

};

#endif