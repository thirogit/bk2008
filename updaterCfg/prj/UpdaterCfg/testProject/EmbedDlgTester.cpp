// EmbedDlgTester.cpp : implementation file
//

#include "stdafx.h"
#include "TestProject.h"
#include "EmbedDlgTester.h"
#include "UpdaterCfgUpdaterCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmbedDlgTester dialog

CEmbedDlgTester::CEmbedDlgTester(CWnd* pParent /*=NULL*/)
    	: CDialog(CEmbedDlgTester::IDD, pParent), child_(NULL), initialized_(false)
{
    //{{AFX_DATA_INIT(CEmbedDlgTester)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CEmbedDlgTester::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEmbedDlgTester)
        DDX_Control(pDX, IDOK, btnClose_);
        DDX_Control(pDX, IDC_COMBO1, cboDialogList_);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmbedDlgTester, CDialog)
    //{{AFX_MSG_MAP(CEmbedDlgTester)
    ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmbedDlgTester message handlers
void CEmbedDlgTester::OnSelchangeCombo1() 
{
    if (child_) {
        child_->DestroyWindow();
        delete child_;
    }

    switch (cboDialogList_.GetCurSel()) {
        case 0:
            child_ = new UpdaterCfgUpdaterCfgDlg;
            ((UpdaterCfgUpdaterCfgDlg*)child_)->CreateAndEmbed(this, CRect(0, 0, 0, 0));
            ((UpdaterCfgUpdaterCfgDlg*)child_)->SetData(&updaterCfg_);
            break;

    }

    UpdateWindowSize();

}

BOOL CEmbedDlgTester::OnInitDialog() 
{
    CDialog::OnInitDialog();

    cboDialogList_.AddString(_T("UpdaterCfgUpdaterCfg"));

    initialized_ = true;
    UpdateWindowSize();

    return TRUE;
}

void CEmbedDlgTester::UpdateWindowSize() 
{
    if (!initialized_)
        return;

    CRect rc;
    CRect comboRc;
    CRect btnRc;
    CRect childRc;

    GetClientRect(rc);
    cboDialogList_.GetWindowRect(comboRc);
    ScreenToClient(comboRc);
    btnClose_.GetWindowRect(btnRc);

    // move close button
    //
    btnClose_.SetWindowPos(&CWnd::wndTop, rc.right - 5 - btnRc.Width(), rc.bottom - 5 - btnRc.Height(), 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
    btnClose_.GetWindowRect(btnRc);
    ScreenToClient(btnRc);
    if (child_ == NULL)
        return;

    // move embedded dialog
    //
    childRc.top = comboRc.bottom + 5;
    childRc.left = 5;
    childRc.right = rc.right - 5;
    childRc.bottom = btnRc.top - 5;
    child_->SetWindowPos(&CWnd::wndTop, childRc.left, childRc.top, childRc.Width(), childRc.Height(), SWP_SHOWWINDOW);

}

void CEmbedDlgTester::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    UpdateWindowSize();	
}

