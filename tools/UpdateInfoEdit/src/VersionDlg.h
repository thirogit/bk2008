#pragma once

#include "ctrls/VersionFilesListCtrl.h"
#include "VersionInfoClassTypes.h"

class CVersionDlg : public CDialog
{
	DECLARE_DYNAMIC(CVersionDlg)

public:
	CVersionDlg(UINT minVerNo,CWnd* pParent = NULL);   // standard constructor
	virtual ~CVersionDlg();

// Dialog Data
	enum { IDD = IDD_VERSIONDLG };
protected:
	CVersionInfo* CreateNewVersionInfo();

	UINT			m_minVerNo;
	UINT			m_versionNo;
	CString			m_archiveName;
	CString			m_description;
	CStringArray	m_files;
	BOOL			m_bOWArchName;

protected:
	CVersionFilesListCtrl m_fileListCtrl;

	virtual void SetupDialog();
	virtual BOOL OnInitDialog();
	virtual CString GetWindowCaption() = 0;
	virtual CString GetOKBtnText() = 0;
	virtual void OnOK();
	virtual bool CommitVersionInfo() = 0;
	CString GetArchiveName(UINT verNo);
	


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	afx_msg void OnAddFile();
	afx_msg void OnRemFile();
	afx_msg void OnOverrideArchName();
	DECLARE_MESSAGE_MAP()
};
