#ifndef __EXPENSECATEGORIESDLG_H__
#define __EXPENSECATEGORIESDLG_H__

#include "../ctrls/BaseListCtrl.h"
#include "../datatypes/ExpenseCategoryArray.h"
#include "..\imgbutton\ImageButton.h"

class CExpenseCategoryListCtrl : public CBaseListCtrl
{
public:
	CExpenseCategoryListCtrl(CExpenseCategoryArray &expenseCategories);	
	void UpdateItemCount();
	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	
private:
	static const GRIDHEADERS m_expenseCategoriesListColumns[4];
	CExpenseCategoryArray &m_expenseCategories;	
};


class CExpenseCategoriesDlg : public CDialog
{
public:
	enum { IDD = IDD_LISTEDITDLG };
	CExpenseCategoriesDlg(CWnd* pParent = NULL);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog() ;
	
	CExpenseCategoryListCtrl m_expenseCategories;

	CImageButton  m_OKBtn;
	CImageButton  m_EditBtn;
	CImageButton  m_AddBtn;
	CImageButton  m_DeleteBtn;

	afx_msg void OnNewExpenseCategroy();
	afx_msg void OnDelExpenseCategory();
	afx_msg void OnEditExpenseCategory();
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP();
	

};

#endif