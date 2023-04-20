#include "stdafx.h"
#include "RepoAccountView.h"
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



IMPLEMENT_DYNAMIC(CRepoAccountView, CDialog)

CRepoAccountView::CRepoAccountView()
{	
	m_pAccountSession = NULL;
}

CRepoAccountView::~CRepoAccountView()
{
	
}

void CRepoAccountView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CRepoAccountView, CDialog)
//ON_BN_CLICKED(IDC_UPDATEREPO, OnUpdateRepo)
	
		
END_MESSAGE_MAP()

void CRepoAccountView::Create(CWnd* pParentWnd)
{
	VERIFY(CDialog::Create(IDD,pParentWnd));
}

BOOL CRepoAccountView::OnInitDialog()
{
	CDialog::OnInitDialog();	
	
	return TRUE;
}

void CRepoAccountView::UpdateFrom(RepoAccountSession* pAccountSession)
{
	m_pAccountSession = pAccountSession;
	if(pAccountSession)
	{
		const RepoAccount* pAccount = pAccountSession->GetAccount();
		SetDlgItemText(IDC_REPO_ADDRESS,pAccount->GetAddress());
		SetDlgItemText(IDC_REPO_LOGIN,pAccount->GetLogin());
		SetDlgItemText(IDC_REPO_COMPANY_CITY,pAccount->GetPassword());
		
	}
	else
	{
		SetDlgItemText(IDC_REPO_ADDRESS,"");
		SetDlgItemText(IDC_REPO_LOGIN,"");
		SetDlgItemText(IDC_REPO_COMPANY_CITY,"");
	}
}


void CRepoAccountView::UpdateToolBar(CallbackToolBar& toolbar)
{
	CPng disconnectImg,connectImg;

	disconnectImg.LoadPNG(IDPNG_REPOACCOUNT_DISCONNECT);
	connectImg.LoadPNG(IDPNG_REPOACCOUNT_CONNECT);

	boost::function<void()> onConnectFn = boost::bind(&CRepoAccountView::OnConnect,this);
	boost::function<void()> onDisconnectFn = boost::bind(&CRepoAccountView::OnDisconnect,this);
	
	COLORREF bkgColor = ::GetSysColor(COLOR_3DFACE);

	toolbar.AddButton(onConnectFn,connectImg.GetBitmap24(bkgColor));
	toolbar.AddButton(onDisconnectFn,disconnectImg.GetBitmap24(bkgColor));

}


void CRepoAccountView::BeforeUpdate()
{
}

void CRepoAccountView::AfterUpdate()
{
}

void CRepoAccountView::OnConnect()
{
	if(!m_pAccountSession->IsOpen())
	{
	
		CWaitDialog waitDlg(this,CTempStringLoader(IDS_REPO_CONNECTING));

		boost::function<void()> connectFn = boost::bind(&RepoAccountSession::Open,m_pAccountSession) ;
		try
		{
			waitDlg.WaitFor(connectFn);		
			
		}
		catch(CException* e)
		{
			AfxMessageBox(((RepoException*)e)->GetError());		
			e->Delete();
			
		}
	}
}

void CRepoAccountView::OnDisconnect()
{
	if(m_pAccountSession->IsOpen())
	{
		m_pAccountSession->Close();
	}
}
