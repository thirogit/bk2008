#include "stdafx.h"
#include "AddUsrDlg.h"



// CAddUsrDlg dialog

//static const int CAddUsrDlg::ini[3] = {0,0,0};

const int aryPrivIDs[UserData::PRIVS_COUNT] =
{
	IDC_ADDMODOWN,
	IDC_MODFORG,
	IDC_DELOWN,
	IDC_DELFORG,	
	IDC_PRINTDOC,
	IDC_ADMIN,
	IDC_MAKEINVOICE	
};

IMPLEMENT_DYNAMIC(CAddUsrDlg, CDialog)
CAddUsrDlg::CAddUsrDlg(UserData *pUser,CWnd* pParent /*=NULL*/)
	: CDialog(CAddUsrDlg::IDD, pParent),m_bEditing(false)
{
	VERIFY(pUser);

	memset(m_privs,0,sizeof(BOOL)*UserData::PRIVS_COUNT);
	m_user = pUser;
	if(m_user->userid)
	{
		m_bEditing  = true;
		m_username = m_user->username;
		m_pass = m_user->pass;
		m_retype = m_pass;
		m_fname = m_user->fname;
		m_lname = m_user->lname;

		for(int i = 0;i < UserData::PRIVS_COUNT;i++)
				m_privs[i] = HAS_PRIV(m_user,i);
		
	  }

}

CAddUsrDlg::~CAddUsrDlg()
{
}


void CAddUsrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_USERNAME,m_username);
	DDX_Text(pDX,IDC_USERFNAME,m_fname);
	DDX_Text(pDX,IDC_USERLNAME,m_lname);
	DDX_Text(pDX,IDC_PASS,m_pass);
	DDX_Text(pDX,IDC_RETYPE,m_retype);

	for(int i = 0;i < UserData::PRIVS_COUNT;i++)
        DDX_Check(pDX,aryPrivIDs[i],m_privs[i]);
	
}


BEGIN_MESSAGE_MAP(CAddUsrDlg, CDialog)
	ON_BN_CLICKED(IDC_ADMIN,OnAdmin)
	ON_BN_CLICKED(IDC_DELOWN,OnDelOwn)
	ON_BN_CLICKED(IDC_DELFORG,OnDelForg)
END_MESSAGE_MAP()

BOOL CAddUsrDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog()) return FALSE;
	int idCaption,idBtnText;
	if(m_bEditing) 
	{
		idCaption = IDS_USEREDIT;
		idBtnText = IDS_BTNEDIT;
	}
	else
	{
		idCaption = IDS_USERADD;
		idBtnText = IDS_BTNADD;
	}
	CString caption;

	caption.LoadString(idCaption);
	SetWindowText(caption);

	caption.LoadString(idBtnText);
	SetDlgItemText(IDOK,caption);
	OnAdmin();


    
	return TRUE;

}

#define UpUser(upname,val) m_user->SQLUpdate.Set(UserData::upname,val)
void CAddUsrDlg::OnOK()
{
	UpdateData();
	if(!(m_username.GetLength() > 0 && m_fname.GetLength() > 0))
	{
		AfxMessageBox(IDS_ERRUSRADDEMPTYFIELDS,MB_OK,0);
		return;
	}
	if(m_pass != m_retype)
	{
		AfxMessageBox(IDS_ERRUSRADDPASSNOTMATCHING,MB_OK,0);
		return;
	}

	
	int privledges = 0,onepriv; 
	for(int i = 0;i < UserData::PRIVS_COUNT;i++)
	{
		if(m_privs[i]) 
		{
			onepriv = (1 << i);
			privledges |= onepriv;
		}
	}	

	bool up_username,up_fname,up_lname,up_pass,up_privs,go,flag = false;
	
	if(m_bEditing)
	{
		up_username = UpUser(update_username,m_user->username != m_username);
		up_fname = UpUser(update_fname,m_user->fname != m_fname);
		up_lname = UpUser(update_lname,m_user->lname != m_lname);
		up_pass = UpUser(update_pass,m_user->pass != m_pass);
		up_privs = UpUser(update_privledges,m_user->privledges != privledges);

	}
	go  = !m_bEditing;

	if(go || up_username)
	{
		m_user->username = m_username;
		flag = true;
	}
	if(go || up_lname)
	{
		m_user->lname = m_lname;
		flag = true;
	}
	if(go || up_fname)
	{
		m_user->fname = m_fname;
		flag = true;
	}
	if(go || up_pass)
	{
		m_user->pass = m_pass;
		m_user->pass.Encrypt();
		flag = true;
	}
	if(go || up_privs)
	{
		m_user->privledges = privledges;
		flag = true;
	}
		
	
	EndDialog((flag || !m_user->userid)  ? IDOK : IDCANCEL);

}

void CAddUsrDlg::EnableEditing(bool bEnable/* = true*/)
{
	m_bEditing = bEnable;
}
void CAddUsrDlg::OnAdmin()
{
	UpdateData();
	
		for(int i = 0;i < UserData::PRIVS_COUNT;i++)
		if(aryPrivIDs[i] == IDC_ADMIN) continue;
		else		
			::EnableWindow(GetDlgItem(aryPrivIDs[i])->GetSafeHwnd(),!m_privs[UserData::ADMIN]);
	
	
}

void CAddUsrDlg::OnDelForg()
{
	UpdateData();
	if(m_privs[UserData::DELFORG]) SendDlgItemMessage(IDC_DELOWN,BM_SETCHECK,TRUE);	
}

void CAddUsrDlg::OnDelOwn()
{
	UpdateData();
	if(!m_privs[UserData::DELOWN]) 
		SendDlgItemMessage(IDC_DELFORG,BM_SETCHECK,FALSE);	
	
}
