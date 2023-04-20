#include "stdafx.h"
#include "EditExpenseDlg.h"
#include "../classes/TempStringLoader.h"
#include "../CattleSQL.h"

CEditExpenseDlg::CEditExpenseDlg(Expense* pExpense,CWnd* pParent/* = NULL*/) : m_pEditExpense(pExpense)
{
	ASSERT(m_pEditExpense);
}
CString CEditExpenseDlg::GetWindowCaption()
{
	return CTempStringLoader(IDS_EDITEPANSECAPTION);
}
void CEditExpenseDlg::InitDialogControls()
{
	CExpenseDlg::InitDialogControls();
	ReloadExpenseCategoryCombo(m_pEditExpense->GetExpenseCategory()->GetId());
	ReloadExpenseTypeCombo(m_pEditExpense->GetExpenseType());
	m_extras.SetWindowText(m_pEditExpense->GetExtras());
	
	m_invoiceDate.SetDate(m_pEditExpense->GetExpenseDate());
	
	m_invoiceNo = m_pEditExpense->GetInvoiceNo();
	m_vat.SetVATRate(VATRate(m_pEditExpense->GetVATRate()));
	m_vatRate = m_pEditExpense->GetVATRate();
	m_expenseValueEdit.SetCommitedText(m_pEditExpense->GetGrosValue().ToString(NULL,4));
	m_expenseValue = m_pEditExpense->GetGrosValue();
	m_expenseTitle = m_pEditExpense->GetExpenseTitle();

	m_btnaction.SetWindowText((LPCSTR)CTempStringLoader(IDS_BTNEDIT));

	UpdateData(FALSE);

	OnExpenseTypeChange();
}

void CEditExpenseDlg::UpdateExpense(Expense *pExpense)
{
	Expense *pNewExpense = CreateExpense();

	pExpense->ChangeGrosValue(pNewExpense->GetGrosValue());
	pExpense->ChangeVATRate(pNewExpense->GetVATRate());
	pExpense->ChangeExpenseType(pNewExpense->GetExpenseType());
	pExpense->ChangeInvoiceNo(pNewExpense->GetInvoiceNo());
	pExpense->ChangeExpenseDate(pNewExpense->GetExpenseDate());
	pExpense->ChangeExtras(pNewExpense->GetExtras());	
	pExpense->ChangeExpenseTitle(pNewExpense->GetExpenseTitle());
	pExpense->ChangeExpenseCategory(pNewExpense->GetExpenseCategory());
	
	delete pNewExpense;
}


bool CEditExpenseDlg::CommitExpense()
{

	UpdateExpense(m_pEditExpense);

	if(m_pEditExpense->UpdateThisExpense())
	{
		m_pEditExpense->CommitChanges();
		return true;
	}
	else
	{
		m_pEditExpense->RollbackChanges();
		SQLDB.SQLMessageBox(IDS_ERRUPDATINGEXPENSE);
		return false;
	}
}
