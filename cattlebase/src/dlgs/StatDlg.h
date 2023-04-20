#pragma once

#include "../ctrls/NSChartCtrl.h"

// CStatDlg dialog

#define STAT_START_DAYS 7

class CStatDlg : public CDialog
{
	DECLARE_DYNAMIC(CStatDlg)

public:
	CStatDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStatDlg();

// Dialog Data
	enum { IDD = IDD_STATS };

	CNSChartCtrl m_chart;
	CComboBox    m_statdata;
	CComboBox    m_days;
	CString      m_total;

	CattleSQL::STATDATA m_currData;
	int                 m_currDays;

	void ReloadChart(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnChangeData();
	afx_msg void OnChangeDays();

	DECLARE_MESSAGE_MAP()
};
