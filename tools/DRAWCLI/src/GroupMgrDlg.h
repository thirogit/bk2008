#pragma once

#include "DrawObjGroup.h"

// CGroupMgrDlg dialog

class CGroupMgrDlg : public CDialog
{
	DECLARE_DYNAMIC(CGroupMgrDlg)

public:
	CGroupMgrDlg(CView* pView);   // standard constructor
	virtual ~CGroupMgrDlg();

private:
	virtual BOOL PreTranslateMessage(MSG *pMsg);

	CListCtrl m_groups;
	CView *m_View;
	CGroupList *m_grpList;
	CString m_groupname;
	HACCEL m_hAccel;



// Dialog Data
	enum { IDD = IDD_GROUPMGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();

	afx_msg void GetDispInfo(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnAddGroup();
	afx_msg void OnDelGroup();
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
