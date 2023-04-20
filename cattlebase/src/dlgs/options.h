
#ifndef __OPTIONSSHEET_H__
#define __OPTIONSSHEET_H__

#include "FirmInfDlg.h"
#include "BackupDlg.h"
#include "MainOpt.h"
#include "UserDlg.h"
#include "PreSelOpt.h"
#include "ArchiveOpt.h"
#include "PassDBMngmntOpt.h"
#include "ImportExportOptDlg.h"

class COptionsSheet : public CPropertySheet
{
// Construction
public:
	COptionsSheet(CWnd* pWndParent = NULL);

// Attributes
public:
	CFirmInfDlg m_FirmInfoPage;
	CBackupDlg  m_BackupPage;
	CMainOpt    m_MainOptPage;
	CUserDlg    m_UsersOpt;
	CPreSelOpt  m_PreSel;
	CArchiveOpt m_ArchiveOpt;
	CPassDBMngmntOpt m_PassFileOpt;
	CImportExportOptDlg m_ImportExportOpt;

// Operations
public:



// Implementation
public:
	virtual ~COptionsSheet();

// Generated message map functions
protected:
	virtual BOOL OnInitDialog();
	//{{AFX_MSG(COptionsSheet)
	/*afx_msg BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);*/
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif
