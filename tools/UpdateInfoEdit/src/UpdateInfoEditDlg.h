// UpdateInfoEditDlg.h : header file
//

#pragma once

#include "VersionInfoList.h"
#include "ctrls/VersionFilesListCtrl.h"
#include "../ftp/FTPProtocolOutput.h"

// CUpdateInfoEditDlg dialog
class CUpdateInfoEditDlg : public CDialog
{
// Construction
public:
	CUpdateInfoEditDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_UPDATEINFOEDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();

	void AddVersion(CSaveVersionInfo *newVersion);
	CString GetTempZipFileName(const CString& zipTempDir);
	void ReloadList();
	
	bool LoginFTP(nsFTP::CFTPClient& FTPClient);
	void UpdateMenu(bool bDownloadEnabled,bool bUploadEnabled,bool bBuildEnabled,bool bCreateEmptyEnabled);
	void OnClose();
	void EnableVersionMenu(bool bEnable);
	void EnableMenuItem(CMenu* pMenu,UINT nCommand,bool bEnable);

protected:
	HICON m_hIcon;

	CString m_versionNumber;
	CString m_archiveName;
	CString m_versionDesc;
	CVersionInfoList m_verList;

	CListBox m_verListBox;
	int		m_verFileCount;
	CVersionFilesListCtrl m_verFilesListCtrl;
	CStringArray	m_verFiles;

	

	nsFTP::nsView::CFTPProtocolOutput m_ProtocolOutput;
	
	
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBuild();
	afx_msg void OnProperties();
	afx_msg void OnAddVersion();
	afx_msg void OnLBSelChange();

	
	afx_msg void OnCreateEmpty();
	afx_msg void OnUpload();
	afx_msg void OnDownload();
	afx_msg void OnExit();

	afx_msg void OnRemoveVersion();
	afx_msg void OnEditVersion();
	

	DECLARE_MESSAGE_MAP()
};
