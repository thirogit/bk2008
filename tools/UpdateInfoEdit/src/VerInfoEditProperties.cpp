// VerInfoEditProperties.cpp : implementation file
//

#include "stdafx.h"
#include "UpdateInfoEdit.h"
#include "VerInfoEditProperties.h"
#include "MainCfg.h"

// CVerInfoEditProperties dialog

IMPLEMENT_DYNAMIC(CVerInfoEditProperties, CDialog)

BEGIN_MESSAGE_MAP(CVerInfoEditProperties, CDialog)
END_MESSAGE_MAP()


CVerInfoEditProperties::CVerInfoEditProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CVerInfoEditProperties::IDD, pParent)
{

	
}

CVerInfoEditProperties::~CVerInfoEditProperties()
{
}

void CVerInfoEditProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_BASEDIR,m_baseDir);
	DDX_Text(pDX,IDC_ARCHOUTDIR,m_verArchDir);
	DDX_Text(pDX,IDC_HOSTNAME,m_hostname);
	DDX_Text(pDX,IDC_PORT,m_port);
	DDX_Text(pDX,IDC_USER,m_username);
	DDX_Text(pDX,IDC_PASSWORD,m_password);
	DDX_Check(pDX,IDC_PASV,m_pasv);
	DDX_Text(pDX,IDC_SERVERROOT,m_serverRoot);

}

BOOL CVerInfoEditProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_baseDir  = AppCfg().GetString(CFGID_BASEDIR);
	m_verArchDir = AppCfg().GetString(CFGID_ARCHIVESDIR);

	m_hostname  = AppCfg().GetString(CFGID_FTPHOST);
	m_port  = AppCfg().GetNum(CFGID_FTPPORT);
	m_username  = AppCfg().GetString(CFGID_FTPUSERNAME);
	m_password  = AppCfg().GetString(CFGID_FTPPASSWORD);
	m_pasv   = AppCfg().GetBool(CFGID_PASSIVEMODE);
	m_serverRoot = AppCfg().GetString(CFGID_SERVERROOT);


	UpdateData(FALSE);


	return TRUE;
}

void CVerInfoEditProperties::OnOK()
{
	UpdateData();

	AppCfg().SetString(CFGID_BASEDIR,m_baseDir);
	AppCfg().SetString(CFGID_ARCHIVESDIR,m_verArchDir);


	AppCfg().SetString(CFGID_FTPHOST,m_hostname);
	AppCfg().SetNum(CFGID_FTPPORT,m_port);
	AppCfg().SetString(CFGID_FTPUSERNAME,m_username);
	AppCfg().SetString(CFGID_FTPPASSWORD,m_password);
	AppCfg().SetBool(CFGID_PASSIVEMODE,m_pasv);

	AppCfg().SetString(CFGID_SERVERROOT,m_serverRoot);
	EndDialog(IDOK);
}


