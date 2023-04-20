#include "stdafx.h"
#include "NewExpenseCategoryDlg.h"
#include "../classes/TempStringLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CNewExpenseCategoryDlg::CNewExpenseCategoryDlg(CWnd* pParent/* = NULL*/) : CExpenseCategoryDlg(pParent),m_NewExpenseCategory(NULL)
{
}

CString CNewExpenseCategoryDlg::GetWindowCaption()
{
	return CTempStringLoader(IDS_NEWEPANSECATEGORY);
}

ExpenseCategory *CNewExpenseCategoryDlg::GetNewExpenseCategory()
{
	return m_NewExpenseCategory;
}

bool CNewExpenseCategoryDlg::CommitExpenseCategory()
{
	ExpenseCategory *newCategory = new ExpenseCategory(NULL_EXPENSECATEGORYID);
	newCategory->SetExpenseCategoryName(m_expenseCategoryName);
	newCategory->SetExpenseCategoryDesc(m_expenseCategoryDesc);
	newCategory->SetDefaultVATRate(m_defaultVATRate);
	m_NewExpenseCategory = newCategory;
	return true;
}