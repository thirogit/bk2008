#include "stdafx.h"
#include "RepoAccountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(CRepoAccountDlg, CDialog)
CRepoAccountDlg::CRepoAccountDlg(RepoAccount* pAccount,CWnd* pParent /*=NULL*/)
	: CDialog(CRepoAccountDlg::IDD, pParent),m_pAccount(pAccount)
{	
	ASSERT(m_pAccount != NULL);
	
}

CRepoAccountDlg::~CRepoAccountDlg()
{
	
}

void CRepoAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_REPO_ACCOUNT_NAME,m_sAccountName);
	DDX_Text(pDX,IDC_REPO_ADDRESS,m_sRepoAddress);
	DDX_Text(pDX,IDC_REPO_LOGIN,m_sRepoUserName);
	DDX_Text(pDX,IDC_REPO_PASSWORD,m_sRepoPassword);
}


BEGIN_MESSAGE_MAP(CRepoAccountDlg, CDialog)

		
END_MESSAGE_MAP()


void CRepoAccountDlg::SetDlgCaption(const CString& sCaption)
{
	m_sDlgCaption = sCaption;
}



BOOL CRepoAccountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_sDlgCaption);

	m_sAccountName = m_pAccount->GetName();
	m_sRepoAddress = m_pAccount->GetAddress();
	m_sRepoUserName = m_pAccount->GetLogin();
	m_sRepoPassword = m_pAccount->GetPassword();
	UpdateData(FALSE);
	return TRUE;
}

void CRepoAccountDlg::OnOK()
{
	UpdateData();

	

	m_pAccount->SetName(m_sAccountName);
	m_pAccount->SetAddress(m_sRepoAddress);
	m_pAccount->SetLogin(m_sRepoUserName);
	m_pAccount->SetPassword(m_sRepoPassword);



	EndDialog(IDOK);
}