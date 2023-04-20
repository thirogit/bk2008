#ifndef __SYNCCOWREJECTEDDLG_H__
#define __SYNCCOWREJECTEDDLG_H__

#include "SyncHelper.h"
#include "RejectedCowsListCtrl.h" 
#include "SyncRejectedDlg.h"

#pragma once

// CSyncCowRejectedDlg dialog

class CSyncCowRejectedDlg : public CSyncRejectedDlg
{
	DECLARE_DYNAMIC(CSyncCowRejectedDlg)

public:
	CSyncCowRejectedDlg(CSyncCowDataArray &rejectedCowsArray,
		UINT nCaption,int nListCfgId);   // standard constructor
	virtual ~CSyncCowRejectedDlg();
	virtual UINT RejectedCount();
	virtual void DoRefresh();
	virtual void UpdateCount();

protected:
	CRejectedCowsListCtrl m_rejectedCows;
	CSyncCowDataArray &m_rejectedCowsArray;
	
	CString FormatRejectedReasonsText(int nItemClicked);	

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnRejCorrect (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropChanges();
	DECLARE_MESSAGE_MAP()
};

#endif