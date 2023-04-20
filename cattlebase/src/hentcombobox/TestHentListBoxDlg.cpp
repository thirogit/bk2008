#include "stdafx.h"
#include "TestHentListBoxDlg.h"

CTestHentListBoxDlg::CTestHentListBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestHentListBoxDlg::IDD, pParent)
{

}

CTestHentListBoxDlg::~CTestHentListBoxDlg()
{
}


void CTestHentListBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CTestHentListBoxDlg, CDialog)
END_MESSAGE_MAP()

BOOL CTestHentListBoxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	return TRUE;

}

void CTestHentListBoxDlg::OnOK()
{
	UpdateData();
	EndDialog(IDOK);

}
