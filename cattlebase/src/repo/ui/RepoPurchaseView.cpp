#include "stdafx.h"
#include "RepoPurchaseView.h"
#include "../../classes/TempStringLoader.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "RepoColors.h"
#include "../image/Png.h"
#include "../utils/Predicates.h"
#include "DownloadControllerImpl.h"
#include "../RepoException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRepoPurchaseView, ETSLayoutDialog)

CRepoPurchaseView::CRepoPurchaseView()
{	
	m_pCurrentSelection = NULL;
}

CRepoPurchaseView::~CRepoPurchaseView()
{
}

void CRepoPurchaseView::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
	

	DDX_Control(pDX,IDC_PURACHELIST,m_PurchaseList);
	DDX_Control(pDX,IDC_PURCHASETABS,m_PurchaseTabs);
}


BEGIN_MESSAGE_MAP(CRepoPurchaseView, ETSLayoutDialog)	
	ON_NOTIFY(NM_CLICK,			IDC_PURACHELIST, OnPurchaseClick)
	//ON_NOTIFY(TCN_SELCHANGE,	IDC_PURCHASETAB, OnTabChange)
	//ON_NOTIFY(TCN_SELCHANGING,	IDC_PURCHASETAB, OnTabChanging)
	
END_MESSAGE_MAP()


void CRepoPurchaseView::Create(CWnd* pParentWnd)
{
	VERIFY(ETSLayoutDialog::Create(IDD,pParentWnd));
	
}

BOOL CRepoPurchaseView::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();
		
	m_PurchaseHentsPage.Create(CRepoPurchaseHentsPage::IDD,&m_PurchaseTabs);
	m_PurchaseInvoicesPage.Create(CRepoPurchaseInvoicesPage::IDD,&m_PurchaseTabs);
	m_PurchaseSummaryPage.Create(CRepoPurchaseSummaryPage::IDD,&m_PurchaseTabs);
		
	m_PurchaseTabs.AddTab(&m_PurchaseInvoicesPage,(LPCSTR)CTempStringLoader(IDS_PURCHASE_INVOICES_TABTEXT));
	m_PurchaseTabs.AddTab(&m_PurchaseHentsPage,(LPCSTR)CTempStringLoader(IDS_PURCHASE_HENTS_TABTEXT));
	m_PurchaseTabs.AddTab(&m_PurchaseSummaryPage,(LPCSTR)CTempStringLoader(IDS_PURCHASE_SUMMARY_TABTEXT));
	
	CPane rootPane = CreateRoot(HORIZONTAL,0);
	rootPane->addItem(&m_PurchaseList);
	rootPane->addItem(&m_PurchaseTabs);

	BeforeUpdate();
	m_PurchaseInvoicesPage.UpdateFrom(NULL);
	m_PurchaseHentsPage.UpdateFrom(NULL);
	m_PurchaseSummaryPage.UpdateFrom(NULL);
	
	return TRUE;
}

void CRepoPurchaseView::BeforeUpdate()
{
	m_PurchaseList.BeforeUpdate();
	m_PurchaseInvoicesPage.BeforeUpdate();
	m_PurchaseHentsPage.BeforeUpdate();
	m_PurchaseSummaryPage.BeforeUpdate();
}

void CRepoPurchaseView::AfterUpdate()
{
}

void CRepoPurchaseView::UpdateFrom(RepoAccountSession* pAccountSession)
{
	m_pAccountSession = pAccountSession;
	PurachesCluster* pPurchasesCluster = NULL;

	if(pAccountSession)
		pPurchasesCluster = pAccountSession->GetPurchases();

	m_PurchaseList.UpdateListWith(pPurchasesCluster);
	m_PurchaseInvoicesPage.UpdateFrom(NULL);
	m_PurchaseHentsPage.UpdateFrom(NULL);
	m_PurchaseSummaryPage.UpdateFrom(NULL);
}

void CRepoPurchaseView::OnPurchaseClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE plvItemActive = (LPNMITEMACTIVATE)pNMHDR;
    
	if(plvItemActive->iItem >= 0)
	{
		if(m_pCurrentSelection != NULL)
			OnPurchaseUnselected(m_pCurrentSelection);

		PurachesCluster* pCluster = m_pAccountSession->GetPurchases();
		m_pCurrentSelection = pCluster->GetItem(plvItemActive->iItem);

		OnPurchaseSelected(m_pCurrentSelection);

		
		RepoPurchase* pPurchase = m_pCurrentSelection->GetPurchase();

		m_PurchaseInvoicesPage.BeforeUpdate();
		m_PurchaseInvoicesPage.UpdateFrom(pPurchase);
		m_PurchaseHentsPage.UpdateFrom(pPurchase);
		m_PurchaseSummaryPage.UpdateFrom(pPurchase);

		

		

	}
	*pResult = 0;
}

void CRepoPurchaseView::UpdateToolBar(CallbackToolBar& toolbar)
{
	CPng downloadImg;
	downloadImg.LoadPNG(IDPNG_REPOACCOUNT_DOWNLOAD);
	boost::function<void()> downloadSelectedFn = boost::bind(&CRepoPurchaseView::DownloadSelected,this);	
	COLORREF bkgColor = ::GetSysColor(COLOR_3DFACE);	
	toolbar.AddButton(downloadSelectedFn,downloadImg.GetBitmap24(bkgColor));
}

void CRepoPurchaseView::DownloadSelected()
{
	DownloadControllerImpl controller;
	try
	{
		m_pAccountSession->DownloadPurchases(&controller);
	}
	catch(RepoException* e)
	{
		AfxMessageBox(e->GetError());
		e->Delete();
	}
}

void CRepoPurchaseView::OnPurchaseSelected(PurchaseItem* pItem)
{
	pItem->SetDoUpdate(true);
}

void CRepoPurchaseView::OnPurchaseUnselected(PurchaseItem* pItem)
{
	pItem->SetDoUpdate(false);
}


