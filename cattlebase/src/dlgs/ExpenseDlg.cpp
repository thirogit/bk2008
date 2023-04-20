#include "stdafx.h"
#include "ExpenseDlg.h"
#include "../classes/TempStringLoader.h"
#include "../consts.h"
#include "../missing/util.h"
#include "../global_arrays.h"

CExpenseDlg::CExpenseDlg(CWnd* pParent/* = NULL*/) : CDialog(IDD_EXPENSE,pParent),
													m_expenseValueEdit(g_szRegExUnsigned),m_invoiceDate(true)					
{
}

void CExpenseDlg::InitDialogControls()
{
	m_btncancel.SetButtonImage(IDB_CANCEL);	
	m_btnaction.SetButtonImage(IDB_DOLAR);
	ReloadExpenseCategoryCombo();
	ReloadExpenseTypeCombo();
	
}

BEGIN_MESSAGE_MAP(CExpenseDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_EXPENSETYPECOMBO,OnExpenseTypeChange)
	ON_CBN_SELCHANGE(IDC_EXPENSECATEGORY,OnExpenseCategoryChange)
	ON_NOTIFY(EN_EDITCOMPLETE,IDC_EXPENSEVALUE,OnExpenseValueComplete)
	ON_NOTIFY(EN_EDITCOMPLETE,IDC_EXPENSEVAT,OnExpenseVATComplete)
END_MESSAGE_MAP()


void CExpenseDlg::OnExpenseValueComplete(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString s;
	m_expenseValueEdit.GetWindowText(s);
	m_expenseValue = (Money(CUtil::myatof(s)));				
	*pResult = (LRESULT)0;
}

void CExpenseDlg::OnExpenseVATComplete(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_vatRate = m_vat.GetVATRate();
	*pResult = (LRESULT)0;
}

void CExpenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EXPENSETYPECOMBO,m_expenseType);
	DDX_Text(pDX,IDC_INVOICENO,m_invoiceNo);
	DDX_Control(pDX,IDC_INVOICEDATE,m_invoiceDate);
	DDX_Control(pDX,IDC_EXPENSEVAT,m_vat);
	DDX_Control(pDX,IDC_EXPENSEEXTRAS,m_extras);	
	DDX_Control(pDX,IDC_EXPENSEVALUE,m_expenseValueEdit);
	DDX_Control(pDX,IDCANCEL,m_btncancel);
	DDX_Control(pDX,IDOK,m_btnaction);
	DDX_Control(pDX,IDC_EXPENSECATEGORY,m_expenseCategory);
	DDX_Text(pDX,IDC_EXPENSETITLEEDIT,m_expenseTitle);

}

Expense::EXPENSE_TYPE CExpenseDlg::GetCurrentExpenseType()
{
	int nSel = m_expenseType.GetCurSel();
	if(nSel >= 0)
	{
		Expense::EXPENSE_TYPE expenseType = (Expense::EXPENSE_TYPE)m_expenseType.GetItemData(nSel);
		return expenseType;
	}
	return Expense::EXPENSE_TYPE_NULL;
}

void CExpenseDlg::OnExpenseTypeChange()
{
		Expense::EXPENSE_TYPE currentExpenseType = GetCurrentExpenseType();
		BOOL bEnableInvoice = (currentExpenseType == Expense::EXPENSE_TYPE_WITH_INVOICE);
		//m_invoiceDate.EnableWindow(bEnableInvoice);
		GetDlgItem(IDC_INVOICENO)->EnableWindow(bEnableInvoice);
}

void CExpenseDlg::OnExpenseCategoryChange()
{
	LRESULT lResult = 0;
	ExpenseCategoryPtr ptrExpenseCategory = GetCurrentExpenseCategory();
	if(!ptrExpenseCategory.IsNull())
	{
		m_vat.SetVATRate(ptrExpenseCategory->GetDefaultVATRate());
		OnExpenseVATComplete(NULL,&lResult);
	}
}

void CExpenseDlg::ReloadExpenseTypeCombo(Expense::EXPENSE_TYPE expenseType/* = Expense::EXPENSE_TYPE_NULL*/)
{
	m_expenseType.ReloadCombo(expenseType);
}

BOOL CExpenseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText((LPCSTR)GetWindowCaption());

	
	InitDialogControls();
	return TRUE;
}


bool CExpenseDlg::ValidateFields()
{
	if(m_expenseTitle.GetLength() == 0)
	{
		AfxMessageBox(IDS_NOEXPENSETITLE);
		GetDlgItem(IDC_EXPENSETITLEEDIT)->SetFocus();
		return false;
	}
	
	if(!m_invoiceDate.IsDateValid() && GetCurrentExpenseType() == Expense::EXPENSE_TYPE_WITH_INVOICE)
	{
		AfxMessageBox(IDS_NOEXPENSEINVOICEDATE);
		m_invoiceDate.SetFocus();
		return false;
	}

	if(m_invoiceNo.GetLength() == 0 && GetCurrentExpenseType() == Expense::EXPENSE_TYPE_WITH_INVOICE)
	{
		AfxMessageBox(IDS_NOEXPENSEINVOICNO);
		GetDlgItem(IDC_INVOICENO)->SetFocus();
		return false;
	}

	if(m_expenseCategory.GetCurSel() < 0)
	{
		AfxMessageBox(IDS_NOEXPENSECATEGORYSELECTED);
		m_expenseCategory.SetFocus();
		return false;
	}

	if(m_expenseValueEdit.IsUncommited())
	{
		AfxMessageBox(IDS_EXPENSEVALUENOTCOMMITED);
		m_expenseValueEdit.SetFocus();
		return false;
	}

	if(m_vat.IsUncommited())
	{
		AfxMessageBox(IDS_EXPENSEVATNOTCOMMITED);
		m_vat.SetFocus();
		return false;
	}

	if(m_vatRate.IsNull())
	{
		AfxMessageBox(IDS_EXPENSEVATMISSING);
		m_vat.SetFocus();
		return false;
	}

	if(m_expenseValue < 0.01)
	{
		AfxMessageBox(IDS_EXPENSEVALUETOOSMALL);
		m_expenseValueEdit.SetFocus();
		return false;
	}

	return true;
}

void CExpenseDlg::OnOK()
{
	UpdateData();

	if(!ValidateFields())
		return;

	if(CommitExpense())
		EndDialog(IDOK);

	return;
}

void CExpenseDlg::ReloadExpenseCategoryCombo(int expensecategoryid/* = -1*/)
{
	int nItem = -1,defaultItem = -1;
	ExpenseCategoryPtr expenseCategoryPtr;
	for(int i = 0;i < ary_expensecategories.GetSize();i++)
	{
		expenseCategoryPtr = ary_expensecategories[i];
		nItem = m_expenseCategory.AddString(expenseCategoryPtr->GetExpenseCategoryName());
		m_expenseCategory.SetItemData(nItem,(DWORD)expenseCategoryPtr->GetId());

		if(nItem <= defaultItem) defaultItem++;

		if(expenseCategoryPtr->GetId() == expensecategoryid)
			defaultItem = nItem;
	}
	m_expenseCategory.SetCurSel(defaultItem);
}

ExpenseCategoryPtr CExpenseDlg::GetCurrentExpenseCategory()
{
	int expenseCategoryId = m_expenseCategory.GetItemData(m_expenseCategory.GetCurSel());
	return ary_expensecategories.GetExpenseCategory(expenseCategoryId);
}


Expense* CExpenseDlg::CreateExpense()
{
	Expense *pExpense = new Expense(NULL_EXPENSEID);
	pExpense->SetExpenseType(GetCurrentExpenseType());
	pExpense->SetExpenseCategory(GetCurrentExpenseCategory());

	CString sExtras;
	m_extras.GetWindowText(sExtras);
	pExpense->SetExtras(sExtras);

	CDateTime dtExpenseDate;
	m_invoiceDate.GetDate(dtExpenseDate);
	pExpense->SetExpenseDate(dtExpenseDate);

	pExpense->SetInvoiceNo(m_invoiceNo);
	pExpense->SetVATRate(m_vatRate.GetDouble());
	pExpense->SetGrosValue(m_expenseValue);
	pExpense->SetExpenseTitle(m_expenseTitle);
	return pExpense;

}