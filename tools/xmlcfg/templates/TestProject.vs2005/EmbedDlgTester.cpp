// EmbedDlgTester.cpp : implementation file
//

#include "stdafx.h"
#include "TestProject.h"
#include "EmbedDlgTester.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmbedDlgTester dialog


CEmbedDlgTester::CEmbedDlgTester(CWnd* pParent /*=NULL*/)
	: CDialog(CEmbedDlgTester::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEmbedDlgTester)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEmbedDlgTester::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmbedDlgTester)
	DDX_Control(pDX, IDC_COMBO1, cboDialogList_);
	DDX_Control(pDX, IDOK, btnClose_);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmbedDlgTester, CDialog)
	//{{AFX_MSG_MAP(CEmbedDlgTester)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmbedDlgTester message handlers

void CEmbedDlgTester::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CEmbedDlgTester::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
