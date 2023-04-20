#include "stdafx.h"
#include "RepoCompanyView.h"
#include "../../datatypes/CompanyInfo.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "../image/Png.h"
#include "WaitDialog.h"
#include "../../classes/TempStringLoader.h"
#include "../RepoException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRepoCompanyView, CDialog)

CRepoCompanyView::CRepoCompanyView()
{	
}

CRepoCompanyView::~CRepoCompanyView()
{
}

void CRepoCompanyView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CRepoCompanyView, CDialog)
//ON_BN_CLICKED(IDC_UPDATEREPO, OnUpdateRepo)
	
		
END_MESSAGE_MAP()

void CRepoCompanyView::Create(CWnd* pParentWnd)
{
	VERIFY(CDialog::Create(IDD,pParentWnd));
}

BOOL CRepoCompanyView::OnInitDialog()
{
	CDialog::OnInitDialog();	

	const Company* pCompany = Company::GetCompany();

	SetDlgItemText(IDC_LOCAL_COMPANY_NAME,pCompany->GetCompanyName());
	SetDlgItemText(IDC_LOCAL_COMPANY_STREET,pCompany->GetStreet());
	SetDlgItemText(IDC_LOCAL_COMPANY_CITY,pCompany->GetCity());
	SetDlgItemText(IDC_LOCAL_COMPANY_POBOX,pCompany->GetPOBox());
	SetDlgItemText(IDC_LOCAL_COMPANY_ZIPCODE,pCompany->GetZipCode());
	SetDlgItemText(IDC_LOCAL_COMPANY_FARMNO,pCompany->GetFarmNo());
	SetDlgItemText(IDC_LOCAL_COMPANY_NIP,pCompany->GetNIP());
	SetDlgItemText(IDC_LOCAL_COMPANY_REGON,pCompany->GetREGON());
	SetDlgItemText(IDC_LOCAL_COMPANY_WETIDNO,pCompany->GetWetIdNo());
	SetDlgItemText(IDC_LOCAL_COMPANY_WETLICNO,pCompany->GetWetLicNo());
	SetDlgItemText(IDC_LOCAL_COMPANY_BANKACCOUNTNO,pCompany->GetAccountNo());
    SetDlgItemText(IDC_LOCAL_COMPANY_BANKNAME,pCompany->GetBank());	
	
	return TRUE;
}

void CRepoCompanyView::UpdateFrom(RepoAccountSession* pAccountSession)
{
	m_pAccountSession = pAccountSession;
	if(pAccountSession)
	{
		const RepoCompany* pRepoCompany = pAccountSession->GetCompany();
		SetDlgItemText(IDC_REPO_COMPANY_NAME,pRepoCompany->GetCompanyName());
		SetDlgItemText(IDC_REPO_COMPANY_STREET,pRepoCompany->GetStreet());
		SetDlgItemText(IDC_REPO_COMPANY_CITY,pRepoCompany->GetCity());
		SetDlgItemText(IDC_REPO_COMPANY_POBOX,pRepoCompany->GetPOBox());
		SetDlgItemText(IDC_REPO_COMPANY_ZIPCODE,pRepoCompany->GetZipCode());
		SetDlgItemText(IDC_REPO_COMPANY_FARMNO,pRepoCompany->GetFarmNo());
		SetDlgItemText(IDC_REPO_COMPANY_NIP,pRepoCompany->GetNIP());
		SetDlgItemText(IDC_REPO_COMPANY_REGON,pRepoCompany->GetREGON());
		SetDlgItemText(IDC_REPO_COMPANY_WETIDNO,pRepoCompany->GetWetIdNo());
		SetDlgItemText(IDC_REPO_COMPANY_WETLICNO,pRepoCompany->GetWetLicNo());
		SetDlgItemText(IDC_REPO_COMPANY_BANKACCOUNTNO,pRepoCompany->GetAccountNo());
		SetDlgItemText(IDC_REPO_COMPANY_BANKNAME,pRepoCompany->GetBank());
	}
	else
	{
		const UINT labelIds[] = 
		{
			IDC_REPO_COMPANY_NAME,
			IDC_REPO_COMPANY_STREET,
			IDC_REPO_COMPANY_CITY,
			IDC_REPO_COMPANY_POBOX,
			IDC_REPO_COMPANY_ZIPCODE,
			IDC_REPO_COMPANY_FARMNO,
			IDC_REPO_COMPANY_NIP,
			IDC_REPO_COMPANY_REGON,
			IDC_REPO_COMPANY_WETIDNO,
			IDC_REPO_COMPANY_WETLICNO,
			IDC_REPO_COMPANY_BANKACCOUNTNO,
			IDC_REPO_COMPANY_BANKNAME,
			0
		};

		const UINT* pLabelId = labelIds;
		while(*pLabelId != 0)
		{
			SetDlgItemText(*pLabelId,"");
			pLabelId++;
		}
	}
}


void CRepoCompanyView::UpdateToolBar(CallbackToolBar& toolbar)
{
	CPng uploadImg;
	uploadImg.LoadPNG(IDPNG_REPOACCOUNT_UPLOAD);
	
	boost::function<void()> uploadSelectedFn = boost::bind(&CRepoCompanyView::UpdateCompany,this);
	
	COLORREF bkgColor = ::GetSysColor(COLOR_3DFACE);
	toolbar.AddButton(uploadSelectedFn,uploadImg.GetBitmap24(bkgColor));

}

void CRepoCompanyView::UpdateCompany()
{
	CWaitDialog waitDlg(this,CTempStringLoader(IDS_REPO_UPDATINGCOMPANY));

	boost::function<void()> uploadFn = boost::bind(&RepoAccountSession::UploadCompany,m_pAccountSession) ;
	try
	{
		waitDlg.WaitFor(uploadFn);		
		UpdateFrom(m_pAccountSession);		
	}
	catch(CException* e)
	{
		AfxMessageBox(((RepoException*)e)->GetError());		
		e->Delete();		
	}

}

void CRepoCompanyView::BeforeUpdate()
{
}

void CRepoCompanyView::AfterUpdate()
{
}