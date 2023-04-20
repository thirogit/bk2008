// SampleDlg.h : header file
//

#pragma once


// CSampleDlg dialog
class CSampleDlg : public CDialog
{
// Construction
public:
	INT CallbackFunc();
	CSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	VOID CreatePrinterList();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPrintBtn();
	afx_msg void OnGetInfoBtn();
	afx_msg void OnGetCallbackBtn();
	afx_msg void OnSelchangeListbox();
	DECLARE_MESSAGE_MAP()
};
