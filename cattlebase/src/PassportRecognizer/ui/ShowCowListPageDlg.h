#pragma once

#include "../../ctrls/CowListCtrl.h"

// CShowCowListPageDlg dialog

class CShowCowListPageDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CShowCowListPageDlg)

public:
	CShowCowListPageDlg(CCowDataArray *cowArray,int iConfigId = -1);
	virtual ~CShowCowListPageDlg();

// Dialog Data
	enum { IDD = IDD_SHOWCOWLIST };
protected:
	CCowListCtrl m_ScanList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
