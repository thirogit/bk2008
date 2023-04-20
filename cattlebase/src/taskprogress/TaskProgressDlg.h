#if !defined(AFX_TASKPROGRESSDLG_H__0382CDF5_1D0E_4735_827F_A9C342591617__INCLUDED_)
#define AFX_TASKPROGRESSDLG_H__0382CDF5_1D0E_4735_827F_A9C342591617__INCLUDED_

#include "../resource.h"
#include "ProgressIFace.h"
#include "TaskIdTranslator.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaskProgressDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTaskProgressDialog dialog

#define WM_PROGRESSDLG_SETTASK (WM_APP+201)
#define WM_PROGRESSDLG_ONSTEP (WM_APP+202)
#define PROGRESSDLG_WM_TASKERROR (WM_APP+203)

class CTaskProgressDialog : public CDialog, public ITaskProgres
{
// Construction / Destruction
public:
    CTaskProgressDialog(const CString& caption,TaskIdTranslator *taskTranslator = NULL);   // standard constructor
    ~CTaskProgressDialog();
public:
	void SetTaskTranslator(TaskIdTranslator *taskTranslator);

	virtual void SetTask(UINT taskID,int steps);
	virtual bool OnStep(int step);
	virtual void OnTaskError(const CString& sError);

    BOOL Create(CWnd *pParent=NULL);

    // Checking for Cancel button
    BOOL CheckCancelButton();
    // Progress Dialog manipulation
	void SetCurTask(int nID,int nMaxSteps); 
    void SetRange(int nLower,int nUpper);
    int  SetStep(int nStep);
    int  SetPos(int nPos);
    int  OffsetPos(int nPos);
    int  StepIt();
	const CString& GetLastError();
        
// Dialog Data
    //{{AFX_DATA(CTaskProgressDialog)
	enum { IDD = IDD_TASKPROGRESS_DLG };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTaskProgressDialog)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
	TaskIdTranslator m_defaultTranslator;
	TaskIdTranslator *m_taskTranslator;
	CString			m_Caption;

    int				m_nLower;
    int				m_nUpper;
    int				m_nStep;
    
    BOOL			m_bCancel;
    BOOL			m_bParentDisabled;
	CString			m_sLastError;

    void ReEnableParent();
	virtual void OnCancel();
    virtual void OnOK() {}; 
    void UpdatePercent(int nCurrent);
    void PumpMessages();

    virtual BOOL OnInitDialog();
    
	afx_msg	LRESULT OnMessageSetTask(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMessageOnStep(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMessageTaskError(WPARAM wParam,LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus);

	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKPROGRESSDLG_H__0382CDF5_1D0E_4735_827F_A9C342591617__INCLUDED_)
