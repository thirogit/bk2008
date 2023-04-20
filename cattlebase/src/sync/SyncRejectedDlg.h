#ifndef __SYNCREJECTEDDLG_H__
#define __SYNCREJECTEDDLG_H__

//#include "SyncHelper.h"
//#include "RejectedDocsListCtrl.h" 

#pragma once


#define WM_SETREJECTTEXT (WM_APP+5001)
// CSyncRejectedDlg dialog

class CSyncRejectedDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CSyncRejectedDlg)

public:
	CSyncRejectedDlg(UINT nCaptionId);   // standard constructor
	virtual ~CSyncRejectedDlg();
	virtual UINT RejectedCount() = 0;
	virtual void UpdateCount() = 0;
	CString GetRejectedReasonsText();
	
	UINT GetCaptionId();

// Dialog Data
	enum { IDD = IDD_SYNCREJECTED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void DoRefresh() = 0;
	virtual CString FormatRejectedReasonsText(int nItemClicked) = 0;


	UINT m_CaptionId;
	CString m_rejectedText;
	
	
	afx_msg void OnListClick(NMHDR* pNMHDR, LRESULT* pResult);

	//afx_msg LRESULT OnRefreshRejected(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};

#endif