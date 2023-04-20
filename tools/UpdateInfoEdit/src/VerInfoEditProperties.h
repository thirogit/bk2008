#pragma once


// CVerInfoEditProperties dialog

class CVerInfoEditProperties : public CDialog
{
	DECLARE_DYNAMIC(CVerInfoEditProperties)

public:
	CVerInfoEditProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVerInfoEditProperties();

// Dialog Data
	enum { IDD = IDD_PROPERTIES };
protected:
	CString m_baseDir;
	CString m_verArchDir;

	CString m_hostname;
	UINT	m_port;
	CString m_username;
	CString m_password;
	BOOL	m_pasv;

	CString m_serverRoot;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};
