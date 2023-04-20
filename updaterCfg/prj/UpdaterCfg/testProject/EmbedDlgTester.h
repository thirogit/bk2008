// EmbedDlgTester.h : header file
//

#pragma once

#include "UpdaterCfgUpdaterCfg.h"
/////////////////////////////////////////////////////////////////////////////
// CEmbedDlgTester dialog

class CEmbedDlgTester : public CDialog
{
// Construction
public:
    CEmbedDlgTester(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CEmbedDlgTester)
    enum { IDD = IDD_EMBED_TEST_DIALOG };
    CButton	btnClose_;
    CComboBox cboDialogList_;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEmbedDlgTester)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CEmbedDlgTester)
    	afx_msg void OnSelchangeCombo1();
    	afx_msg virtual BOOL OnInitDialog();
    	afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP();

    void UpdateWindowSize();

    CWnd* child_;
    bool initialized_;

    UpdaterCfgUpdaterCfg updaterCfg_;
};

