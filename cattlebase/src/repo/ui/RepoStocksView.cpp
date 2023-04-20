#include "stdafx.h"
#include "RepoStocksView.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "RepoColors.h"
#include "../image/Png.h"
#include "../utils/Predicates.h"
#include "../RepoException.h"
#include "SimpleProgressDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRepoStocksView, ETSLayoutDialog)

CRepoStocksView::CRepoStocksView()
{	
}

CRepoStocksView::~CRepoStocksView()
{
}

void CRepoStocksView::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_ITEMLIST,m_StocksList);	
}


BEGIN_MESSAGE_MAP(CRepoStocksView, ETSLayoutDialog)
			
END_MESSAGE_MAP()

void CRepoStocksView::Create(CWnd* pParentWnd)
{
	VERIFY(ETSLayoutDialog::Create(IDD,pParentWnd));
}

BOOL CRepoStocksView::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();	

	CreateRoot(HORIZONTAL,0)->addItem(IDC_ITEMLIST);

	
	
	return TRUE;
}

void CRepoStocksView::BeforeUpdate()
{
	m_StocksList.Lock();
}

void CRepoStocksView::AfterUpdate()
{
	m_StocksList.Unlock();
}

void CRepoStocksView::UpdateFrom(RepoAccountSession* pAccountSession)
{
	m_pAccountSession = pAccountSession;
	StocksCluster* pStocksCluster = NULL;

	if(pAccountSession)
		pStocksCluster = pAccountSession->GetStocks();

	m_StocksList.UpdateListWith(pStocksCluster);
}


void CRepoStocksView::UpdateToolBar(CallbackToolBar& toolbar)
{
	CPng checkBoxImg,boxImg,uploadImg,downloadImg;
	checkBoxImg.LoadPNG(IDPNG_REPOACCOUNT_CHECKBOX);
	boxImg.LoadPNG(IDPNG_REPOACCOUNT_BOX);
	uploadImg.LoadPNG(IDPNG_REPOACCOUNT_UPLOAD);
	downloadImg.LoadPNG(IDPNG_REPOACCOUNT_DOWNLOAD);

	boost::function<void()> selectAllFn = boost::bind(&CRepoStocksView::SelectAll,this);
	boost::function<void()> unselectAllFn = boost::bind(&CRepoStocksView::UnselectAll,this);
	boost::function<void()> selectMissingRemoteAllFn = boost::bind(&CRepoStocksView::SelectMissingOnRemoteSide,this);
	boost::function<void()> selectMissingLocaclAllFn = boost::bind(&CRepoStocksView::SelectMissingOnLocaclSide,this);
	boost::function<void()> selectNotEqualAllFn = boost::bind(&CRepoStocksView::SelectNotEqualSides,this);
	boost::function<void()> uploadSelectedFn = boost::bind(&CRepoStocksView::UpdateSelected,this);
	boost::function<void()> downloadSelectedFn = boost::bind(&CRepoStocksView::DownloadSelected,this);

	COLORREF bkgColor = ::GetSysColor(COLOR_3DFACE);
	toolbar.AddButton(selectAllFn,checkBoxImg.GetBitmap24(bkgColor));
	toolbar.AddButton(unselectAllFn,boxImg.GetBitmap24(bkgColor));
	toolbar.AddButton(selectMissingRemoteAllFn,checkBoxImg.GetBitmap24(COLOR_MISSING_REMOTE_SIDE));
	toolbar.AddButton(selectMissingLocaclAllFn,checkBoxImg.GetBitmap24(COLOR_MISSING_LOCAL_SIDE));
	toolbar.AddButton(selectNotEqualAllFn,checkBoxImg.GetBitmap24(COLOR_BOTH_SIDES_NOT_EQUAL));
	toolbar.AddButton(uploadSelectedFn,uploadImg.GetBitmap24(bkgColor));
	toolbar.AddButton(downloadSelectedFn,downloadImg.GetBitmap24(bkgColor));

}

void CRepoStocksView::SelectAll()
{
	AlwaysTrue<StockEquivalenceItem*> alwaysTruePredicate;
	m_StocksList.Select(alwaysTruePredicate);
}

void CRepoStocksView::UnselectAll()
{
	AlwaysTrue<StockEquivalenceItem*> alwaysTruePredicate;
	NotPredicate<StockEquivalenceItem*> alwaysFalsePredicate(alwaysTruePredicate);
	m_StocksList.Select(alwaysFalsePredicate);
}

void CRepoStocksView::SelectMissingOnRemoteSide()
{
	SelectMissingRemote<StockEquivalenceItem*> missingRemotePredicate;
	m_StocksList.Select(missingRemotePredicate);
}

void CRepoStocksView::SelectMissingOnLocaclSide()
{
	SelectMissingLocal<StockEquivalenceItem*> missingLocalPredicate;
	m_StocksList.Select(missingLocalPredicate);
}

void CRepoStocksView::SelectNotEqualSides()
{
	SelectNotEqual<StockEquivalenceItem*> notEqualPredicate;
	m_StocksList.Select(notEqualPredicate);
}

void CRepoStocksView::UpdateSelected()
{
	try
	{
		CSimpleProgressDialog uploadProgressDlg("uploading stocks...");
		uploadProgressDlg.Create(this);
		m_pAccountSession->UpdateStocks(&uploadProgressDlg);
		m_StocksList.Invalidate(FALSE);
		
	}
	catch(RepoException* e)
	{
		AfxMessageBox(e->GetError());		
		e->Delete();		
	}
}

void CRepoStocksView::DownloadSelected()
{
	m_pAccountSession->DownloadStocks();
}

