#include "stdafx.h"
#include "RepoHentsView.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "RepoColors.h"
#include "../image/Png.h"
#include "../utils/Predicates.h"
#include "SimpleProgressDialog.h"
#include "../RepoException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CRepoHentsView, ETSLayoutDialog)

CRepoHentsView::CRepoHentsView()
{	
}

CRepoHentsView::~CRepoHentsView()
{
}

void CRepoHentsView::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_ITEMLIST,m_HentsList);	
}

BEGIN_MESSAGE_MAP(CRepoHentsView, ETSLayoutDialog)
//	ON_BN_CLICKED(IDC_UPDATEREPO, OnUpdateRepo)		
END_MESSAGE_MAP()


void CRepoHentsView::Create(CWnd* pParentWnd)
{
	VERIFY(ETSLayoutDialog::Create(IDD,pParentWnd));
}

BOOL CRepoHentsView::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();	

	//m_HentsList.SetExtendedStyle(m_HentsList.GetExtendedStyle() | LVS_EX_CHECKBOXES);
	CreateRoot(HORIZONTAL,0)->addItem(IDC_ITEMLIST);
	return TRUE;
}

void CRepoHentsView::UpdateFrom(RepoAccountSession* pAccountSession)
{
	m_pAccountSession = pAccountSession;
	HentsCluster* pHentsCluster = NULL;

	if(pAccountSession)
		pHentsCluster = pAccountSession->GetHents();

	m_HentsList.UpdateListWith(pHentsCluster);	

}

void CRepoHentsView::BeforeUpdate()
{
	m_HentsList.Lock();
}

void CRepoHentsView::AfterUpdate()
{
	m_HentsList.Unlock();
}


void CRepoHentsView::UpdateToolBar(CallbackToolBar& toolbar)
{
	CPng checkBoxImg,boxImg,uploadImg,downloadImg;
	checkBoxImg.LoadPNG(IDPNG_REPOACCOUNT_CHECKBOX);
	boxImg.LoadPNG(IDPNG_REPOACCOUNT_BOX);
	uploadImg.LoadPNG(IDPNG_REPOACCOUNT_UPLOAD);
	downloadImg.LoadPNG(IDPNG_REPOACCOUNT_DOWNLOAD);

	boost::function<void()> selectAllFn = boost::bind(&CRepoHentsView::SelectAll,this);
	boost::function<void()> unselectAllFn = boost::bind(&CRepoHentsView::UnselectAll,this);
	boost::function<void()> selectMissingRemoteAllFn = boost::bind(&CRepoHentsView::SelectMissingOnRemoteSide,this);
	boost::function<void()> selectMissingLocaclAllFn = boost::bind(&CRepoHentsView::SelectMissingOnLocaclSide,this);
	boost::function<void()> selectNotEqualAllFn = boost::bind(&CRepoHentsView::SelectNotEqualSides,this);
	boost::function<void()> uploadSelectedFn = boost::bind(&CRepoHentsView::UploadSelected,this);
	boost::function<void()> downloadSelectedFn = boost::bind(&CRepoHentsView::DownloadSelected,this);

	COLORREF bkgColor = ::GetSysColor(COLOR_3DFACE);
	toolbar.AddButton(selectAllFn,checkBoxImg.GetBitmap24(bkgColor));
	toolbar.AddButton(unselectAllFn,boxImg.GetBitmap24(bkgColor));
	toolbar.AddButton(selectMissingRemoteAllFn,checkBoxImg.GetBitmap24(COLOR_MISSING_REMOTE_SIDE));
	toolbar.AddButton(selectMissingLocaclAllFn,checkBoxImg.GetBitmap24(COLOR_MISSING_LOCAL_SIDE));
	toolbar.AddButton(selectNotEqualAllFn,checkBoxImg.GetBitmap24(COLOR_BOTH_SIDES_NOT_EQUAL));
	toolbar.AddButton(uploadSelectedFn,uploadImg.GetBitmap24(bkgColor));
	toolbar.AddButton(downloadSelectedFn,downloadImg.GetBitmap24(bkgColor));

}

void CRepoHentsView::SelectAll()
{
	AlwaysTrue<HentEquivalenceItem*> alwaysTruePredicate;
	m_HentsList.Select(alwaysTruePredicate);
}

void CRepoHentsView::UnselectAll()
{
	AlwaysTrue<HentEquivalenceItem*> alwaysTruePredicate;
	NotPredicate<HentEquivalenceItem*> alwaysFalsePredicate(alwaysTruePredicate);
	m_HentsList.Select(alwaysFalsePredicate);
}

void CRepoHentsView::SelectMissingOnRemoteSide()
{
	SelectMissingRemote<HentEquivalenceItem*> missingRemotePredicate;
	m_HentsList.Select(missingRemotePredicate);
}

void CRepoHentsView::SelectMissingOnLocaclSide()
{
	SelectMissingLocal<HentEquivalenceItem*> missingLocalPredicate;
	m_HentsList.Select(missingLocalPredicate);
}

void CRepoHentsView::SelectNotEqualSides()
{
	SelectNotEqual<HentEquivalenceItem*> notEqualPredicate;
	m_HentsList.Select(notEqualPredicate);
}

void CRepoHentsView::UploadSelected()
{
	try
	{
		CSimpleProgressDialog uploadProgressDlg("uploading hents...");
		uploadProgressDlg.Create(this);
		m_pAccountSession->UploadHents(&uploadProgressDlg);
		m_HentsList.Invalidate(FALSE);
		
	}
	catch(RepoException* e)
	{
		AfxMessageBox(e->GetError());		
		e->Delete();		
	}
}

void CRepoHentsView::DownloadSelected()
{
}



