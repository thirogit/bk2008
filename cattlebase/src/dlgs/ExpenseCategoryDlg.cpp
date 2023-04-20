#include "stdafx.h"
#include "ExpenseCategoryDlg.h"
#include "../datalimits.h"

CExpenseCategoryDlg::CExpenseCategoryDlg(CWnd* pParent/* = NULL*/) : 
				CDialog(IDD_EXPENSECATEGORY,pParent)
{
}



void CExpenseCategoryDlg::InitDialogControls()
{
	//m_defaultVATRateCB.SetCurVATRate(0.0);
}

void CExpenseCategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_EXPENSECATEGORYNAME,m_expenseCategoryName);
	DDX_Text(pDX,IDC_EXPENSECATEGORYDESC,m_expenseCategoryDesc);
	DDX_Control(pDX,IDC_DEFAULTVATRATE,m_defaultVATRateCB);

}

BOOL CExpenseCategoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText((LPCSTR)GetWindowCaption());
	SendDlgItemMessage(IDC_EXPENSECATEGORYNAME,EM_LIMITTEXT,MAXEXPENSECATEGORYNAME);
	SendDlgItemMessage(IDC_EXPENSECATEGORYDESC,EM_LIMITTEXT,MAXEXPENSECATEGORYDESC);

	InitDialogControls();
	return TRUE;
}

bool CExpenseCategoryDlg::ValidateFields()
{
	if(m_expenseCategoryName.GetLength() == 0)
	{
		AfxMessageBox(IDS_EMPTYCATEGORYNAME);
		GetDlgItem(IDC_EXPENSECATEGORYNAME)->SetFocus();
		return false;
	}
	return true;
}

void CExpenseCategoryDlg::OnOK()
{
	UpdateData();

	if(!ValidateFields())
		return;

	m_defaultVATRate = m_defaultVATRateCB.GetVATRate();
	
	if(CommitExpenseCategory())
		EndDialog(IDOK);

}
