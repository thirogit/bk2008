#pragma once


#include "..\classes\types\DateTime.h"
#include <afxeditbrowsectrl.h>


class CBackupDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CBackupDlg)

public:
	CBackupDlg();
	virtual ~CBackupDlg();

// Dialog Data
	enum { IDD = IDD_BACKUP };
	static void MakeBackupCopy(CWnd *msgWnd,CString &strBackupDir);
protected:

	CMFCEditBrowseCtrl   m_BackupDirectory;
	CListBox        m_copiesList;
	CStatic         m_lastCopy;
	CComboBox       m_timeSpan;

	CDateTime m_lastCopyTime;
	CString  m_BackupDir;
	BOOL m_ask4BaseDir;
	BOOL m_doPassImgBackup;


	BOOL OnInitDialog();
	BOOL OnApply();
protected:
	void FillListBox();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool GetChoosenBackupDir(CString &choosenBackupDir);

	afx_msg void OnMakeCopy();
	afx_msg void OnDelCopy();
	afx_msg void OnRestoreCopy();
	afx_msg LRESULT OnDirChange(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedMakeDBCopy();

	DECLARE_MESSAGE_MAP()

	
};
