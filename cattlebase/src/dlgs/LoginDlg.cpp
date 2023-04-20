// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoginDlg.h"
#include "../global_arrays.h"
#include "../DataTypes.h"
#include "MainCfg.h"

// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)
CLoginDlg::CLoginDlg(UserData* pUser/* = NULL*/,bool UpdateDB/* = true*/,bool bTryAutoLogin/* = false*/,CWnd* pParent /*=NULL*/) : CDialog(CLoginDlg::IDD, pParent),
										m_bTryAutoLogin(bTryAutoLogin),m_bUpdateDB(UpdateDB),m_pUser(pUser)
{

	if(m_bTryAutoLogin)
	{
		if(AppCfg().GetBool(CFG_AUTOLOGIN))
		{
			UserData *pUser;
			m_pass = AppCfg().GetString(CFG_AUTOPASS);
			pUser = ary_users.GetPtrByUserId( AppCfg().GetNum(CFG_AUTOUSERID) );
			if(pUser)
				m_user = pUser->username;
		}
	}

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_LOGIN,m_user);
	DDX_Text(pDX,IDC_PASSWORD,m_pass);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)

END_MESSAGE_MAP()

BOOL CLoginDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog()) return FALSE;

	if(m_bTryAutoLogin)
	{
		if(AppCfg().GetBool(CFG_AUTOLOGIN))
		{
			if(DoLogin(m_user,m_pass))
				EndDialog(IDOK);
		}
	}

	if(m_pUser)
	{
		m_user = m_pUser->username;		
		GetDlgItem(IDC_LOGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_PASSWORD)->SetFocus();
	}
	else
		m_pass.Empty();

	UpdateData(FALSE);

	return TRUE;
}

bool CLoginDlg::DoLogin(CString &username,CString &password)
{
	UserData *pUser = ary_users.GetPtrByUserName(username);
	if(!pUser)
	{
		AfxMessageBox(IDS_ERRWRONGUSRNAME,MB_OK,0);
		return false;
	}
	else
	{
		if(pUser->pass != password)
		{
			AfxMessageBox(IDS_ERRWRONGPASS,MB_OK,0);
			return false;
		}
	}

	if(m_bUpdateDB)
		pUser->UpdateLastLogin();
	UserData::SetCurrentUser(pUser);
	return true;
}

void CLoginDlg::OnOK()
{
	UpdateData();

	if(DoLogin(m_user,m_pass))
		EndDialog(IDOK);
}

