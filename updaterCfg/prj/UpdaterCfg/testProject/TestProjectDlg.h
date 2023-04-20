// TestProjectDlg.h : header file
//
#if !defined(_TESTPROJECTDLG_H_)
#define _TESTPROJECTDLG_H_

#include "UpdaterCfgCfgManagerDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CTestProjectDlg dialog

class CTestProjectDlg : public CDialog
{
// Construction
public:
    CTestProjectDlg(CWnd* pParent = NULL);	// standard constructor

    // Dialog Data
    //{{AFX_DATA(CTestProjectDlg)
    enum { IDD = IDD_TESTPROJECT_DIALOG };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTestProjectDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    // Generated message map functions
    //{{AFX_MSG(CTestProjectDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnCmdRuntest();
    afx_msg void OnCmdRunEmbedTest();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP();

    void UpdateWindowSize();

    bool initialized_;
    UpdaterCfgCfgManagerDlg childDlg_;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(_TESTPROJECTDLG_H_)
