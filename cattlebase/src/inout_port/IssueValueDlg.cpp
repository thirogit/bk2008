#include "stdafx.h"

#include "IssueValueDlg.h"


// CIssueValueDlg dialog

IMPLEMENT_DYNAMIC(CIssueValueDlg, CDialog)
CIssueValueDlg::CIssueValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIssueValueDlg::IDD, pParent)
{
	m_ValueRegEx = ".*";
}

CIssueValueDlg::~CIssueValueDlg()
{
}

void CIssueValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_ISSUEVALUEEDIT,m_valueEdit);
}


BEGIN_MESSAGE_MAP(CIssueValueDlg, CDialog)
END_MESSAGE_MAP()


void CIssueValueDlg::SetCaption(UINT nCaption)
{
	m_caption.LoadString(nCaption);
	
}
void CIssueValueDlg::SetCaption(LPCSTR lpszCaption)
{
	m_caption = lpszCaption;
}
void CIssueValueDlg::SetIssueText(UINT nIssueText)
{
	m_IssueText.LoadString(nIssueText);
}
void CIssueValueDlg::SetIssueText(LPCSTR lpszIssueText)
{
	m_IssueText = lpszIssueText;
}
void CIssueValueDlg::SetIssueValueName(UINT nValueName)
{
	m_ValueName.LoadString(nValueName);
}
void CIssueValueDlg::SetIssueValueName(LPCSTR lpszValueName)
{
	m_ValueName = lpszValueName;
}
void CIssueValueDlg::SetRegEx(LPCSTR lpszRegEx)
{
	m_ValueRegEx = lpszRegEx;
}
CString CIssueValueDlg::GetValue()
{
	return m_Value;
}
void CIssueValueDlg::SetValue(LPCSTR lpszValue)
{	
	m_Value = lpszValue;
}


BOOL CIssueValueDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_caption);
	SetDlgItemText(IDC_ISSUEVALUENAME,m_ValueName);
	SetDlgItemText(IDC_ISSUETEXT,m_IssueText);

	m_valueEdit.SetRegEx(m_ValueRegEx);
	m_valueEdit.SetWindowText(m_Value);

	return TRUE;
}
void CIssueValueDlg::OnOK()
{
	UpdateData();
	m_valueEdit.GetWindowText(m_Value);
	EndDialog(IDOK);

}