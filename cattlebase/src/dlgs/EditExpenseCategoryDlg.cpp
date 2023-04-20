#include "stdafx.h"
#include "EditExpenseCategoryDlg.h"
#include "../classes/TempStringLoader.h"

CEditExpenseCategoryDlg::CEditExpenseCategoryDlg(ExpenseCategory *pCategory,CWnd* pParent/* = NULL*/) : CExpenseCategoryDlg(pParent)
{
	ASSERT(pCategory);
	m_pEditCategory = pCategory;
}

bool CEditExpenseCategoryDlg::CommitExpenseCategory()
{
	m_pEditCategory->ChangeExpenseCategoryName(m_expenseCategoryName);
	m_pEditCategory->ChangeExpenseCategoryDesc(m_expenseCategoryDesc);
	m_pEditCategory->ChangeDefaultVATRate(m_defaultVATRate);
	return true;
}

CString CEditExpenseCategoryDlg::GetWindowCaption()
{
	return CTempStringLoader(IDS_EDITEXPENSECATEGORYCAPTION);
}

void CEditExpenseCategoryDlg::InitDialogControls()
{
	m_expenseCategoryName = m_pEditCategory->GetExpenseCategoryName();
	m_expenseCategoryDesc = m_pEditCategory->GetExpenseCategoryDesc();
	if(m_pEditCategory->HasDefaultVATRate())
	{
		m_defaultVATRate = m_pEditCategory->GetDefaultVATRate();
		m_defaultVATRateCB.SetVATRate(m_defaultVATRate);
	}
	else
	{
		m_defaultVATRate.MakeNull();
	}
	UpdateData(FALSE);
}