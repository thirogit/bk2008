#if !defined(__WAITDLG_H__)
#define __WAITDLG_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxmt.h>

#define WM_PROGRESS_SETTASK (WM_APP+1001)
#define WM_PROGRESS_ONSTEP (WM_APP+1002)
#define WM_PROGRESS_ONERROR (WM_APP+1003)

class CWaitDlg : public CDialog
{
public:
	CWaitDlg(CWnd* pParent = NULL);   
	enum { IDD = IDD_BUSY_DLG };

	void SetWaitText(const CString& waitText);
	void EnableAbortButton(bool bEnable);

private:
   CString        m_Text;
   bool           m_bEnableAbortButton;
   CProgressCtrl  m_Progress;
   bool		 	  m_bAborted;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
	virtual BOOL OnInitDialog();

protected:
	afx_msg LRESULT OnSetTask(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnError(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnStep(WPARAM wParam,LPARAM lParam);
	afx_msg void OnAbort();

	DECLARE_MESSAGE_MAP()
};


#endif 
