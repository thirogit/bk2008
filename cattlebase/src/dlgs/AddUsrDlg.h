#pragma once

#include "DataTypes.h"

// CAddUsrDlg dialog

class CAddUsrDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddUsrDlg)

public:
	CAddUsrDlg(UserData *pUser,CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddUsrDlg();
	
	void EnableEditing(bool bEnable = true);
private:
    CString m_username;
	CString m_fname;
	CString m_lname;
	CString m_pass;
	CString m_retype;

	BOOL m_privs[UserData::PRIVS_COUNT];
	//static const int ini[3];

	UserData *m_user;
	bool m_bEditing;

	// Dialog Data
	enum { IDD = IDD_ADDUSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	void OnOK();

	afx_msg void OnAdmin();
	afx_msg void OnDelForg();
	afx_msg void OnDelOwn();

	DECLARE_MESSAGE_MAP()
};
