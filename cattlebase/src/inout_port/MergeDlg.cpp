#include "stdafx.h"
#include "MergeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMergeDlg dialog

IMPLEMENT_DYNAMIC(CMergeDlg, CDialog)
CMergeDlg::CMergeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMergeDlg::IDD, pParent)
{
	m_bDo4All = FALSE;	
}

CMergeDlg::~CMergeDlg()
{
}

void CMergeDlg::SetMergeText(const CString &mergeText)
{
	m_mergeText = mergeText;
}

void CMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX,IDC_USEFORNEXTALL,m_bDo4All);
	DDX_Radio(pDX,IDC_INSERTOPT,radioAction);
}


BEGIN_MESSAGE_MAP(CMergeDlg, CDialog)
	ON_BN_CLICKED(IDC_REPLACEOPT,OnReplaceOptRadioBtn)
END_MESSAGE_MAP()

void CMergeDlg::OnReplaceOptRadioBtn()
{
	UpdateData();
	GetDlgItem(IDC_REPLACEITEMSCOMBO)->EnableWindow(radioAction == 1);
}

BOOL CMergeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_MERGETEXT,(LPCSTR)m_mergeText);
	radioAction = 0;
	UpdateData(FALSE);
	return TRUE;
}
	
CMergeDlg::MERGE_ACTION CMergeDlg::GetMergeAction()
{	
	if(radioAction == 1) return (m_bDo4All ? MERGE_ACTION_REPLACE_4ALL : MERGE_ACTION_REPLACE);

	return (m_bDo4All ? MERGE_ACTION_INSERT_4ALL : MERGE_ACTION_INSERT);
}
