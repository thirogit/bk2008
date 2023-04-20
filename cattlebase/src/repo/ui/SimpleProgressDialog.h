#ifndef __SIMPLEPROGRESSDIALOG_H__
#define __SIMPLEPROGRESSDIALOG_H__

#include "../../taskprogress/SimpleProgress.h"

#define WM_PROGRESS_SETSTEPS (WM_APP+2201)
#define WM_PROGRESS_STEP (WM_APP+2202)

class CSimpleProgressDialog : public CDialog, public SimpleProgress
{

public:
    CSimpleProgressDialog (const CString& sTaskName);
    virtual ~CSimpleProgressDialog ();

	virtual void SetSteps(int maxSteps);
	virtual bool Step();
	BOOL Create(CWnd *pParent);
	
private:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	void PumpMessages();
	virtual void OnCancel();
	BOOL WasCanceled();

    void ReEnableParent();
	BOOL DestroyDialog();
private:
	BOOL m_bCancel;
	CString m_sTaskName;
	CWnd* m_pParentWnd;
	BOOL m_bParentDisabled;
	CProgressCtrl	m_Progress;

private:
	 
    
	afx_msg	LRESULT OnMessageSetSteps(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMessageStep(WPARAM wParam,LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus);

	DECLARE_MESSAGE_MAP()
};


#endif