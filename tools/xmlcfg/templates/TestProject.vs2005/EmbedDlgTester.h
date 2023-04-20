#if !defined(AFX_EMBEDDLGTESTER_H__C6F19291_61EE_4239_B608_F99680A956A9__INCLUDED_)
#define AFX_EMBEDDLGTESTER_H__C6F19291_61EE_4239_B608_F99680A956A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmbedDlgTester.h : header file
//

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
	CComboBox	cboDialogList_;
	CButton	btnClose_;
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
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMBEDDLGTESTER_H__C6F19291_61EE_4239_B608_F99680A956A9__INCLUDED_)
