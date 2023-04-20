#include "stdafx.h"
#include "RepoAccountSessionView.h"
#include "SimpleProgressDialog.h"
#include "../RepoException.h"
#include "../image/Png.h"
#include <boost/bind.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ID_REPOCTRL_TOOLBAR 1001
#define TOOLBAR_HEIGHT 45


BEGIN_MESSAGE_MAP(RepoAccountSessionView, CStatic)

	ON_WM_SIZE()
			
END_MESSAGE_MAP()

RepoAccountSessionView::RepoAccountSessionView(RepoAccountSession* pAccountSession) :
	m_pAccountSession(pAccountSession),m_pViewHolder(NULL),m_pActiveView(NULL)
{
}

RepoAccountSessionView::~RepoAccountSessionView()
{
}
	
RepoAccountSession* RepoAccountSessionView::GetAccountSession() const
{
	return m_pAccountSession;
}

void RepoAccountSessionView::Create(ViewHolder* pViewHolder)
{
	ASSERT(!m_pViewHolder);
	if(m_pViewHolder)
		return;

	m_pViewHolder = pViewHolder;

	CRect rect(0,0,0,0);
	
	CStatic::Create("",WS_CHILD | WS_TABSTOP,rect,pViewHolder);
	ModifyStyleEx(0,WS_EX_CONTROLPARENT);
	ASSERT(GetExStyle() & WS_EX_CONTROLPARENT);

	CreateToolBar();

	m_repoCompanyView.Create(this);
	m_SessionViews.Add(&m_repoCompanyView);

    m_repoHentsView.Create(this);
	m_SessionViews.Add(&m_repoHentsView);

	m_repoStocksView.Create(this);
	m_SessionViews.Add(&m_repoStocksView);

	m_repoPurchaseView.Create(this);
	m_SessionViews.Add(&m_repoPurchaseView);

	m_repoAccountView.Create(this);
	m_SessionViews.Add(&m_repoAccountView);

	m_pActiveView = &m_repoAccountView;
}

void RepoAccountSessionView::CreateToolBar()
{
	CRect toolbarRect(0,0,0,0);

	m_repoCtrlToolBar.Create(this,toolbarRect,ID_REPOCTRL_TOOLBAR);
	m_repoCtrlToolBar.ShowWindow(SW_SHOW);

}

void  RepoAccountSessionView::PositionToolBar()
{
	if(m_repoCtrlToolBar.m_hWnd)
	{
		CRect clientRect;
		GetClientRect(&clientRect);
		m_repoCtrlToolBar.SetWindowPos(NULL,0,0,clientRect.Width(),TOOLBAR_HEIGHT,SWP_NOZORDER);
	}
}

/*CRect clientRect;
	CRect treeCtrlRect;
	m_accountsTree.GetWindowRect(&treeCtrlRect);
	ScreenToClient(&treeCtrlRect);	

	GetClientRect(&clientRect);
	int repoAreaHeight = clientRect.Height() - treeCtrlRect.top;
	int repoAreaWidth = clientRect.Width() - treeCtrlRect.right;

	m_repoCompanyDlg.SetWindowPos(&m_accountsTree, treeCtrlRect.right, treeCtrlRect.top, 
						repoAreaWidth, repoAreaHeight, SWP_HIDEWINDOW | SWP_FRAMECHANGED);

	m_repoHentsDlg.SetWindowPos(&m_accountsTree, treeCtrlRect.right, treeCtrlRect.top,
						repoAreaWidth, repoAreaHeight, SWP_HIDEWINDOW | SWP_FRAMECHANGED);

	m_repoStocksDlg.SetWindowPos(&m_accountsTree, treeCtrlRect.right, treeCtrlRect.top, 
						repoAreaWidth, repoAreaHeight, SWP_HIDEWINDOW | SWP_FRAMECHANGED);

	m_repoPurchaseDlg.SetWindowPos(&m_accountsTree, treeCtrlRect.right, treeCtrlRect.top, 
						repoAreaWidth, repoAreaHeight, SWP_HIDEWINDOW | SWP_FRAMECHANGED);
	
	*/

void RepoAccountSessionView::OnSize(UINT nType,int cx,int cy)
{
	CStatic::OnSize(nType,cx,cy);

	NewPtrFlushArray<CWnd>::iterator viewIt = m_SessionViews.begin();
	int yViewPos = TOOLBAR_HEIGHT+5;
	while(viewIt != m_SessionViews.end())
	{
		(*viewIt)->SetWindowPos(NULL,0,yViewPos,cx,cy-yViewPos,SWP_NOZORDER | SWP_FRAMECHANGED);
		viewIt++;
	}
	PositionToolBar();
	
}

void RepoAccountSessionView::ShowElement(SessionElement element)
{
	m_pActiveView->Hide();
	switch(element)
	{
	case Account:
		SwitchToAccountView();
		
		break;
	case Hents:
		SwitchToHentsView();
		
		break;
	case Stocks:
		SwitchToStocksView();
		
		break;
	case Company:
		SwitchToCompanyView();
		
		break;
	case Purchases:
		SwitchToPurchasesView();
		
		break;
	}
	m_pActiveView->Show();
	m_pViewHolder->Show(this);

}

void RepoAccountSessionView::SwitchToStocksView()
{
	m_pActiveView = &m_repoStocksView;

	
	try
	{
		m_pActiveView->BeforeUpdate();
		if(!m_pAccountSession->GotStocks())
		{
			CSimpleProgressDialog fetchProgressDlg("fetching stocks...");
			fetchProgressDlg.Create(this);
			m_pAccountSession->FetchStocks(&fetchProgressDlg);	
		}
		
		m_repoStocksView.UpdateFrom(m_pAccountSession);
		m_repoCtrlToolBar.RemoveAllButtons();
		m_repoStocksView.UpdateToolBar(m_repoCtrlToolBar);
		m_pActiveView->AfterUpdate();

	}
	catch(RepoException* e)
	{
		m_repoStocksView.UpdateFrom(NULL);
		m_pActiveView->AfterUpdate();

		AfxMessageBox(e->GetError());
		e->Delete();
	}
}

void RepoAccountSessionView::SwitchToCompanyView()
{
	m_pActiveView = &m_repoCompanyView;

		
	try
	{
		m_pActiveView->BeforeUpdate();
		if(!m_pAccountSession->GotCompany())
		{
			m_pAccountSession->FetchCompany();	
		}
		
		m_repoCompanyView.UpdateFrom(m_pAccountSession);
		m_repoCtrlToolBar.RemoveAllButtons();
		m_repoCompanyView.UpdateToolBar(m_repoCtrlToolBar);
		m_pActiveView->AfterUpdate();

	}
	catch(RepoException* e)
	{
		m_repoCompanyView.UpdateFrom(NULL);
		m_pActiveView->AfterUpdate();


		AfxMessageBox(e->GetError());
		e->Delete();
	}


}

void RepoAccountSessionView::SwitchToPurchasesView()
{
	m_pActiveView = &m_repoPurchaseView;

	try
	{
		m_pActiveView->BeforeUpdate();
		if(!m_pAccountSession->GotPurchases())
		{
			CSimpleProgressDialog fetchProgressDlg("fetching purchases...");
			fetchProgressDlg.Create(this);
			m_pAccountSession->FetchPurchases(&fetchProgressDlg);	
		}
		m_repoPurchaseView.UpdateFrom(m_pAccountSession);
		m_repoCtrlToolBar.RemoveAllButtons();
		m_repoPurchaseView.UpdateToolBar(m_repoCtrlToolBar);
		m_pActiveView->AfterUpdate();
	}
	catch(RepoException* e)
	{
		m_repoPurchaseView.UpdateFrom(NULL);
		m_pActiveView->AfterUpdate();

		AfxMessageBox(e->GetError());
		e->Delete();
	}
	

}



void RepoAccountSessionView::SwitchToAccountView()
{
	m_pActiveView = &m_repoAccountView;
	m_repoAccountView.BeforeUpdate();
	m_repoAccountView.UpdateFrom(m_pAccountSession);

	m_repoCtrlToolBar.RemoveAllButtons();
	m_repoAccountView.UpdateToolBar(m_repoCtrlToolBar);
	
	m_repoAccountView.AfterUpdate();
}

void RepoAccountSessionView::SwitchToHentsView()
{
	m_pActiveView = &m_repoHentsView;

	try
	{
		m_pActiveView->BeforeUpdate();
		if(!m_pAccountSession->GotHents())
		{
			CSimpleProgressDialog fetchProgressDlg("fetching hents...");
			fetchProgressDlg.Create(this);
			m_pAccountSession->FetchHents(&fetchProgressDlg);	
		}
		
		m_repoHentsView.UpdateFrom(m_pAccountSession);
		m_repoCtrlToolBar.RemoveAllButtons();
		m_repoHentsView.UpdateToolBar(m_repoCtrlToolBar);
		m_pActiveView->AfterUpdate();

	}
	catch(RepoException* e)
	{
		m_repoHentsView.UpdateFrom(NULL);
		m_pActiveView->AfterUpdate();

		AfxMessageBox(e->GetError());
		e->Delete();
	}
}

//void RepoAccountSessionView::SetOnDisconnect(boost::function<void (RepoAccountSessionView*)> &onDisconnect)
//{
//	m_onDisconnect = onDisconnect;
//}
//
//void RepoAccountSessionView::SetOnConnected(boost::function<void (RepoAccountSessionView*)> &onConnected)
//{
//	m_onConnected = onConnected;
//}
	

void RepoAccountSessionView::Connect()
{
	m_pAccountSession->Open();
}

void RepoAccountSessionView::Disconnect()
{
	if(m_pAccountSession->IsOpen())
		m_pAccountSession->Close();
}

bool RepoAccountSessionView::IsConnected() const
{
	return m_pAccountSession->IsOpen();
}

void RepoAccountSessionView::RegisterObserver(RepoSessionObserver* pObserver)
{
	m_pAccountSession->RegisterObserver(pObserver);
}

void RepoAccountSessionView::UnregisterObserver(RepoSessionObserver* pObserver)
{
	m_pAccountSession->UnregisterObserver(pObserver);
}

void RepoAccountSessionView::UpdateAccount(const RepoAccount& accountUpdate)
{
	m_pAccountSession->UpdateAccount(accountUpdate);
	m_repoAccountView.UpdateFrom(m_pAccountSession);
}

const RepoAccount* RepoAccountSessionView::GetAccount() const
{
	return m_pAccountSession->GetAccount();
}