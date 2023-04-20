#include "stdafx.h"
#include "InfoTextBoxDlg.h"
// CInfoTextBoxDlg dialog

void CInfoTextBoxDlg::ShowTextInfoBox(const CString & text,CWnd *pParent/* = NULL*/)
{
	CInfoTextBoxDlg textInfoBoxDlg(pParent);
	textInfoBoxDlg.SetText(text);
	textInfoBoxDlg.DoModal();
}

CInfoTextBoxDlg::CInfoTextBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoTextBoxDlg::IDD, pParent)
{
}

CInfoTextBoxDlg::~CInfoTextBoxDlg()
{
}


void CInfoTextBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_INFOTEXT,m_textBox);
}


BEGIN_MESSAGE_MAP(CInfoTextBoxDlg, CDialog)
END_MESSAGE_MAP()


BOOL CInfoTextBoxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_textBox.AddText((LPCSTR)m_text);
	return TRUE;
}

void CInfoTextBoxDlg::SetText(const CString &text)
{
	m_text = text;
}