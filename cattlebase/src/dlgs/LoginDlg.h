#pragma once

#include "DataTypes.h"

// CLoginDlg dialog

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(UserData* pUser = NULL,bool UpdateDB = true,bool bTryAutoLogin = false,CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
	enum { IDD = IDD_LOGIN };
protected:
	void OnOK();
	CString m_user;
	CString m_pass;
	bool m_bTryAutoLogin;
	bool m_bUpdateDB;
	UserData *m_pUser;
	bool DoLogin(CString &username,CString &password);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
