#include "stdafx.h"
#include "RepoAccountsSyncCtrlDlg.h"
#include "../RepoAccountSession.h"
#include "WaitDialog.h"
#include <boost/bind.hpp>
#include "../RepoException.h"


#include "RepoColors.h"

#include "../image/Png.h"
#include "accountdlg/RepoAccountDlg.h"
#include "../../dao/RepoAccountDAO.h"
#include "../../exceptions/SQLException.h"
#include "../../classes/TempStringLoader.h"

//#include "question/QuestionBox.h"
//#include "../../messagebox/PPMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


RepoAccountsTreeItemAction::RepoAccountsTreeItemAction(RepoAccountSessionView* pAccountSessionView) : m_pAccountSessionView(pAccountSessionView)
{
	m_OnSelectedAction = boost::bind(&RepoAccountsTreeItemAction::NoAction,this,_1);
	m_OnDblClickAction = boost::bind(&RepoAccountsTreeItemAction::NoAction,this,_1);
	m_OnUnselectedAction = boost::bind(&RepoAccountsTreeItemAction::NoAction,this,_1);
}
void RepoAccountsTreeItemAction::SetOnDblClickAction(boost::function<void (RepoAccountSessionView*)> &onDblClickAction)
{
	m_OnDblClickAction = onDblClickAction;
}

void RepoAccountsTreeItemAction::SetOnSelectedAction(boost::function<void (RepoAccountSessionView*)> &onSelectedAction)
{
	m_OnSelectedAction = onSelectedAction;
}

void RepoAccountsTreeItemAction::SetOnUnelectedAction(boost::function<void (RepoAccountSessionView*)> &onUnselectedAction)
{
	m_OnUnselectedAction = onUnselectedAction;
}

void RepoAccountsTreeItemAction::NoAction(RepoAccountSessionView* pAccountSessionView)
{
}

void RepoAccountsTreeItemAction::OnDblClick()
{
	m_OnDblClickAction(m_pAccountSessionView);
}

void RepoAccountsTreeItemAction::OnSelected()
{
	m_OnSelectedAction(m_pAccountSessionView);
}

void RepoAccountsTreeItemAction::OnUnselected()
{
	m_OnUnselectedAction(m_pAccountSessionView);
}
	
RepoAccountSessionView* RepoAccountsTreeItemAction::GetAccountSessionView()
{
	return m_pAccountSessionView;
}

//#################################################################################################


IMPLEMENT_DYNAMIC(CRepoAccountsSyncCtrlDlg, CDialog)
CRepoAccountsSyncCtrlDlg::CRepoAccountsSyncCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRepoAccountsSyncCtrlDlg::IDD, pParent)//,m_pDialogOnRightHandSide(NULL)
{	

}

CRepoAccountsSyncCtrlDlg::~CRepoAccountsSyncCtrlDlg()
{
	m_accountSessions.FreeAndFlush();
	m_accounts.FreeAndFlush();

}

void CRepoAccountsSyncCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_ACCOUNTSTREE,m_accountsTree);
	DDX_Control(pDX,IDC_MISSING_LOCAL_SIDE,m_MissingLocalSideClrLbl);
	DDX_Control(pDX,IDC_BOTH_SIDES_NOT_EQUAL,m_BothSidesNotEqualClrLbl);
	DDX_Control(pDX,IDC_MISSING_REMOTE_SIDE,m_MissingRemoteSideClrLbl);
	//DDX_Control(pDX,IDC_REPOCTRL_TOOLBAR,m_repoCtrlToolBar);
	DDX_Control(pDX,IDC_ACCOUNTS_TOOLBAR,m_repoAccountsToolBar);
	
}

BEGIN_MESSAGE_MAP(CRepoAccountsSyncCtrlDlg, CDialog)

	ON_NOTIFY(NM_DBLCLK , IDC_ACCOUNTSTREE, OnItemDblClick)
	ON_NOTIFY(TVN_SELCHANGED , IDC_ACCOUNTSTREE, OnItemSelected)
	ON_WM_DESTROY()
			
END_MESSAGE_MAP()


BOOL CRepoAccountsSyncCtrlDlg::OnInitDialog()
{

	CDialog::OnInitDialog();	

	m_accountSessions.FreeAndFlush();
	m_accounts.FreeAndFlush();

	RepoAccountDAO repoAccountDao;
	try
	{
		repoAccountDao.LoadRepoAccounts(m_accounts,NULL);
	}
	catch(CSQLException* e)
	{
		e->Delete();
		EndDialog(IDABORT);
		return TRUE;
	}

	PtrFlushArray<RepoAccount>::ArrayIterator accountIt = m_accounts.iterator();
	while(accountIt.hasNext())
	{
		m_accountSessions.Add(new RepoAccountSession(*(accountIt++),&m_Context));
	}
	
	

	CWnd* pViewHolderPlaceHolder = GetDlgItem(IDC_VIEWHOLDER);
	CRect viewHolderRect;
	pViewHolderPlaceHolder->GetWindowRect(&viewHolderRect);
	ScreenToClient(&viewHolderRect);
	pViewHolderPlaceHolder->DestroyWindow();

	m_SessionViewHolder.Create(this,viewHolderRect,IDC_VIEWHOLDER);
	m_SessionViewHolder.ShowWindow(SW_SHOW);

	PtrFlushArray<RepoAccountSession>::ArrayIterator accountSessionIt = m_accountSessions.iterator();
	while(accountSessionIt.hasNext())
	{
		RepoAccountSessionView* pAccountSessionView = new RepoAccountSessionView(*accountSessionIt);
		pAccountSessionView->Create(&m_SessionViewHolder);
		m_accountSessionViews.Add(pAccountSessionView);
		AddAccountSession(pAccountSessionView);
		accountSessionIt++;
	}
	

//	boost::function<void()> onConnectFn = boost::bind(&CRepoAccountsSyncCtrlDlg::OnConnectClick,this);
//	boost::function<void()> onDisconnectFn = boost::bind(&CRepoAccountsSyncCtrlDlg::OnDisconnectClick,this);
	boost::function<void()> onAddFn = boost::bind(&CRepoAccountsSyncCtrlDlg::OnAddNewAccountClick,this);
	boost::function<void()> onDeleteFn = boost::bind(&CRepoAccountsSyncCtrlDlg::OnDeleteAccountClick,this);
	boost::function<void()> onEditFn = boost::bind(&CRepoAccountsSyncCtrlDlg::OnEditAccountClick,this);
//	m_repoAccountsToolBar.SetOnConnect(onConnectFn);
//	m_repoAccountsToolBar.SetOnDisconnect(onDisconnectFn);
	m_repoAccountsToolBar.SetOnAdd(onAddFn);
	m_repoAccountsToolBar.SetOnDelete(onDeleteFn);
	m_repoAccountsToolBar.SetOnEdit(onEditFn);

	
	m_MissingLocalSideClrLbl.SetBackgroundColor(COLOR_MISSING_LOCAL_SIDE);
	m_BothSidesNotEqualClrLbl.SetBackgroundColor(COLOR_BOTH_SIDES_NOT_EQUAL);
	m_MissingRemoteSideClrLbl.SetBackgroundColor(COLOR_MISSING_REMOTE_SIDE);
	
	DisableAccountToolbarButtons(NULL);

/*
	CPng closeImage;
	closeImage.LoadPNG(IDPNG_REPOACCOUNT_CLOSE);

	boost::function<void()> onCloseFn = boost::bind(&CRepoAccountsSyncCtrlDlg::EndDialog,this,IDOK);

	m_repoExitToolBar.AddButton(onCloseFn,closeImage.GetBitmap24(::GetSysColor(COLOR_3DFACE)));
*/	

	return TRUE;
}

void CRepoAccountsSyncCtrlDlg::AddAccountSession(RepoAccountSessionView* pAccountSessionView)
{
	RepoAccountSession* pAccountSession = pAccountSessionView->GetAccountSession();
	const RepoAccount* pAccount = pAccountSession->GetAccount();
	RepoAccountsTreeItemAction* pItemAction = new RepoAccountsTreeItemAction(pAccountSessionView);

	boost::function<void(RepoAccountSessionView*)> dblClickAction = boost::bind(&CRepoAccountsSyncCtrlDlg::DoConnect,this,_1);
	boost::function<void(RepoAccountSessionView*)> selectAction = boost::bind(&CRepoAccountsSyncCtrlDlg::OnAccountItemSelected,this,_1);
	boost::function<void(RepoAccountSessionView*)> unselectAction = boost::bind(&CRepoAccountsSyncCtrlDlg::DisableAccountToolbarButtons,this,_1);

	pItemAction->SetOnDblClickAction(dblClickAction);
	pItemAction->SetOnSelectedAction(selectAction);
	pItemAction->SetOnUnelectedAction(unselectAction);

	HTREEITEM hAccountItem = m_accountsTree.InsertItem((LPCSTR)pAccount->GetName());

	ForwardRepoSessionObserver* pForwardObserver = new ForwardRepoSessionObserver();

	boost::function<void()> onConnectedFn = boost::bind(&CRepoAccountsSyncCtrlDlg::OnConnected,this,hAccountItem);
	boost::function<void()> onDisconnectedFn = boost::bind(&CRepoAccountsSyncCtrlDlg::OnDisconnected,this,hAccountItem);
	pForwardObserver->SetOnConnected(onConnectedFn);
	pForwardObserver->SetOnDisconnect(onDisconnectedFn);

	pAccountSessionView->RegisterObserver(pForwardObserver);
	m_ItemObservers[hAccountItem] = pForwardObserver;
	
	m_accountsTree.SetItemData(hAccountItem,(DWORD_PTR)pItemAction);
	
}



void CRepoAccountsSyncCtrlDlg::OnDestroy()
{
	TraverseDeleteItemData(TVI_ROOT);
	m_accountSessions.FreeAndFlush();
	m_accounts.FreeAndFlush();
	m_accountSessionViews.FreeAndFlush();

	std::map< HTREEITEM,ForwardRepoSessionObserver* >::iterator observerIt = m_ItemObservers.begin();
	while(observerIt != m_ItemObservers.end())
	{
		delete observerIt->second;
		observerIt++;
	}
	
	
}

void CRepoAccountsSyncCtrlDlg::ShowRightHandSide()
{	
//	m_pDialogOnRightHandSide = pDialog;
//	m_pDialogOnRightHandSide->Show();	
}

void CRepoAccountsSyncCtrlDlg::HideRightHandSide()
{
	//if(m_pDialogOnRightHandSide)
	//{
	//	m_pDialogOnRightHandSide->Hide();
	//	m_pDialogOnRightHandSide = NULL;
	//}
}

void CRepoAccountsSyncCtrlDlg::TraverseDeleteItemData(HTREEITEM hItem)
{
	//if(m_accountsTree.ItemHasChildren(hItem))
	//{ /* iterate over children */
		HTREEITEM hSubItem = m_accountsTree.GetNextItem(hItem, TVGN_CHILD);
		while(hSubItem != NULL)
		{ /* recur */
			TraverseDeleteItemData(hSubItem);
			hSubItem = m_accountsTree.GetNextItem(hSubItem, TVGN_NEXT);
		} /* recur */
	//} /* iterate over children */

	if(TVI_ROOT != hItem)
	{
		RepoAccountsTreeItemAction* pItemAction = (RepoAccountsTreeItemAction*)m_accountsTree.GetItemData(hItem);
		if(pItemAction)
		{
			delete pItemAction;
		}
	}
}


void CRepoAccountsSyncCtrlDlg::DisableAccountToolbarButtons(RepoAccountSessionView* pAccountSessionView)
{
//	m_repoAccountsToolBar.EnableConnectButton(FALSE);
//	m_repoAccountsToolBar.EnableDisconnectButton(FALSE);
	m_repoAccountsToolBar.EnableEditButton(FALSE);
	m_repoAccountsToolBar.EnableDeleteButton(FALSE);
}

void CRepoAccountsSyncCtrlDlg::UpdateAccountToolbarButtons(RepoAccountSessionView* pAccountSessionView)
{
	RepoAccountSession* pAccountSession = pAccountSessionView->GetAccountSession();

	BOOL bConnected = pAccountSession->IsOpen() ? TRUE : FALSE;

//	m_repoAccountsToolBar.EnableConnectButton(!bConnected);
//	m_repoAccountsToolBar.EnableDisconnectButton(bConnected);
	m_repoAccountsToolBar.EnableEditButton(!bConnected);
	m_repoAccountsToolBar.EnableDeleteButton(TRUE);
}

void CRepoAccountsSyncCtrlDlg::DoConnect(RepoAccountSessionView* pAccountSessionView)
{
	if(pAccountSessionView->IsConnected())
		return;

	CWaitDialog waitDlg(this,CTempStringLoader(IDS_REPO_CONNECTING));

	boost::function<void()> connectFn = boost::bind(&RepoAccountSessionView::Connect,pAccountSessionView) ;
	try
	{
		waitDlg.WaitFor(connectFn);		
		
	}
	catch(CException* e)
	{
		AfxMessageBox(((RepoException*)e)->GetError());		
		e->Delete();
		
	}

	//UpdateAccountToolbarButtons(pAccountSessionView);
}

void CRepoAccountsSyncCtrlDlg::OnAccountItemSelected(RepoAccountSessionView* pAccountSessionView)
{
	UpdateAccountToolbarButtons(pAccountSessionView);
	pAccountSessionView->ShowElement(RepoAccountSessionView::Account);
}
	
void CRepoAccountsSyncCtrlDlg::OnAccountHentsSelected(RepoAccountSessionView* pAccountSessionView)
{
	pAccountSessionView->ShowElement(RepoAccountSessionView::Hents);
}

void CRepoAccountsSyncCtrlDlg::OnAccountStocksSelected(RepoAccountSessionView* pAccountSessionView)
{
	pAccountSessionView->ShowElement(RepoAccountSessionView::Stocks);
}

void CRepoAccountsSyncCtrlDlg::OnAccountCompanySelected(RepoAccountSessionView* pAccountSessionView)
{
	pAccountSessionView->ShowElement(RepoAccountSessionView::Company);
}

void CRepoAccountsSyncCtrlDlg::OnAccountInboxSelected(RepoAccountSessionView* pAccountSessionView)
{
	pAccountSessionView->ShowElement(RepoAccountSessionView::Purchases);
}

void CRepoAccountsSyncCtrlDlg::OnItemDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMIA)
	{		
		if(pNMIA->iItem >= 0)
		{
			HTREEITEM hSelectedItem = m_accountsTree.GetSelectedItem();
			RepoAccountsTreeItemAction* pItemAction = (RepoAccountsTreeItemAction*)m_accountsTree.GetItemData(hSelectedItem);
			pItemAction->OnDblClick();			
		}
	}
	*pResult = 1;
}

void CRepoAccountsSyncCtrlDlg::OnItemSelected(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW lpNMTREEVIEW = (LPNMTREEVIEW)pNMHDR;
 
	HTREEITEM hCurrSel = lpNMTREEVIEW->itemNew.hItem;
	HTREEITEM hPrevSel = lpNMTREEVIEW->itemOld.hItem;

	if(hPrevSel)
	{
		RepoAccountsTreeItemAction* pPrevItemAction = (RepoAccountsTreeItemAction*)m_accountsTree.GetItemData(hPrevSel);
		pPrevItemAction->OnUnselected();
	}

	RepoAccountsTreeItemAction* pNewItemAction = (RepoAccountsTreeItemAction*)m_accountsTree.GetItemData(hCurrSel);
	pNewItemAction->OnSelected(); 
 
	*pResult = 0;
}

void CRepoAccountsSyncCtrlDlg::OnConnected(HTREEITEM hAccountItem)
{	
	RepoAccountsTreeItemAction* pItemAction = (RepoAccountsTreeItemAction*)m_accountsTree.GetItemData(hAccountItem);
	RepoAccountSessionView* pAccountSessionView = pItemAction->GetAccountSessionView();


	HTREEITEM hAccountHentsItem = m_accountsTree.InsertItem("Kontrahenci",hAccountItem);
	RepoAccountsTreeItemAction* pHentsItemAction = new RepoAccountsTreeItemAction(pAccountSessionView);
	boost::function<void(RepoAccountSessionView*)> onHentsSelectedFn = boost::bind(&CRepoAccountsSyncCtrlDlg::OnAccountHentsSelected,this,_1);
	pHentsItemAction->SetOnSelectedAction(onHentsSelectedFn);
	m_accountsTree.SetItemData(hAccountHentsItem,(DWORD_PTR)pHentsItemAction);


	HTREEITEM hAccountStocksItem = m_accountsTree.InsertItem("Rasy",hAccountItem);
	RepoAccountsTreeItemAction* pStocksItemAction = new RepoAccountsTreeItemAction(pAccountSessionView);
	boost::function<void(RepoAccountSessionView*)> onStocksSelectedFn = boost::bind(&CRepoAccountsSyncCtrlDlg::OnAccountStocksSelected,this,_1);
	pStocksItemAction->SetOnSelectedAction(onStocksSelectedFn);
	m_accountsTree.SetItemData(hAccountStocksItem,(DWORD_PTR)pStocksItemAction);


	HTREEITEM hAccountCompanyInfoItem = m_accountsTree.InsertItem("Dane firmy",hAccountItem);
	RepoAccountsTreeItemAction* pCompanyItemAction = new RepoAccountsTreeItemAction(pAccountSessionView);
	boost::function<void(RepoAccountSessionView*)> onCompanySelectedFn = boost::bind(&CRepoAccountsSyncCtrlDlg::OnAccountCompanySelected,this,_1);
	pCompanyItemAction->SetOnSelectedAction(onCompanySelectedFn);
	m_accountsTree.SetItemData(hAccountCompanyInfoItem,(DWORD_PTR)pCompanyItemAction);

	HTREEITEM hAccountInboxItem = m_accountsTree.InsertItem("Skupy",hAccountItem);
	RepoAccountsTreeItemAction* pInboxItemAction = new RepoAccountsTreeItemAction(pAccountSessionView);
	boost::function<void(RepoAccountSessionView*)> onInboxSelectedFn = boost::bind(&CRepoAccountsSyncCtrlDlg::OnAccountInboxSelected,this,_1);
	pInboxItemAction->SetOnSelectedAction(onInboxSelectedFn);
	m_accountsTree.SetItemData(hAccountInboxItem,(DWORD_PTR)pInboxItemAction);
	
	m_accountsTree.Expand(hAccountItem,TVE_EXPAND);
}

void CRepoAccountsSyncCtrlDlg::OnDisconnected(HTREEITEM hAccountItem)
{
	DeleteItemChildren(hAccountItem);
}

//void CRepoAccountsSyncCtrlDlg::OnConnectClick()
//{
//	HTREEITEM hSelectedItem = m_accountsTree.GetSelectedItem();
//	RepoAccountsTreeItemAction* pItemAction = (RepoAccountsTreeItemAction*)m_accountsTree.GetItemData(hSelectedItem);
//	RepoAccountSessionView* pSessionView = pItemAction->GetAccountSessionView();
//	DoConnect(pSessionView);
//	
//}

void CRepoAccountsSyncCtrlDlg::DeleteItemChildren(HTREEITEM hParentItem)
{
	HTREEITEM hSubItem = m_accountsTree.GetNextItem(hParentItem, TVGN_CHILD);
	while(hSubItem != NULL)
	{ /* recur */
		DeleteItemChildren(hSubItem);			
		RepoAccountsTreeItemAction* pItemAction = (RepoAccountsTreeItemAction*)m_accountsTree.GetItemData(hSubItem);
		delete pItemAction;

		m_accountsTree.DeleteItem(hSubItem);

		hSubItem = m_accountsTree.GetNextItem(hParentItem, TVGN_CHILD);
	} /* recur */
}

//void CRepoAccountsSyncCtrlDlg::OnDisconnectClick()
//{
//	HTREEITEM hSelectedItem = m_accountsTree.GetSelectedItem();
//	RepoAccountsTreeItemAction* pItemAction = (RepoAccountsTreeItemAction*)m_accountsTree.GetItemData(hSelectedItem);
//	RepoAccountSessionView* pSessionView = pItemAction->GetAccountSessionView();
//	if(pSessionView->IsConnected())
//	{
//		pSessionView->Disconnect();
//	}	
//
//	DeleteItemChildren(hSelectedItem);
//	UpdateAccountToolbarButtons(pSessionView);
//}

void CRepoAccountsSyncCtrlDlg::OnEditAccountClick()
{
	HTREEITEM hSelectedItem = m_accountsTree.GetSelectedItem();
	RepoAccountsTreeItemAction* pItemAction = (RepoAccountsTreeItemAction*)m_accountsTree.GetItemData(hSelectedItem);
	RepoAccountSessionView* pSessionView = pItemAction->GetAccountSessionView();
	
	if(!pSessionView->IsConnected())
	{		
		const RepoAccount* pReferenceAccount = pSessionView->GetAccount();
		RepoAccount accountCopy(pReferenceAccount->GetId());
		accountCopy.CopyFrom(*pReferenceAccount);
		CRepoAccountDlg editAccountDlg(&accountCopy,this);
		editAccountDlg.SetDlgCaption(CTempStringLoader(IDS_EDITREPOACCOUNT_DLG_CAPTION));
		if(editAccountDlg.DoModal() == IDOK)
		{			
			RepoAccountDAO repoAccountDao;
			try
			{
				Transaction transaction = repoAccountDao.OpenTransaction();
				repoAccountDao.UpdateRepoAccount(transaction,&accountCopy);
				transaction->Commit();

				pSessionView->UpdateAccount(accountCopy);
				m_accountsTree.SetItem(hSelectedItem,TVIF_TEXT,(LPCSTR)accountCopy.GetName(),0,0,0,0,0);
			}
			catch(CSQLException* e)
			{
				AfxMessageBox(e->GetError(),MB_OK);
				e->Delete();
				return;
			}
		}
	}
}

void CRepoAccountsSyncCtrlDlg::OnAddNewAccountClick()
{
	
	RepoAccount newAccount(NULL_ACCOUNT_ID);
	CRepoAccountDlg newAccountDlg(&newAccount,this);
	newAccountDlg.SetDlgCaption(CTempStringLoader(IDS_NEWREPOACCOUNT_DLG_CAPTION));
	if(newAccountDlg.DoModal() == IDOK)
	{
		RepoAccountDAO repoAccountDao;
		try
		{
			Transaction transaction = repoAccountDao.OpenTransaction();
			RepoAccount* pNewlyInsertedAccount = repoAccountDao.InsertRepoAccount(transaction,&newAccount);
			transaction->Commit();

			RepoAccountSession* pNewAccountsSession = new RepoAccountSession(pNewlyInsertedAccount,&m_Context);
			RepoAccountSessionView* pAccountSessionView = new RepoAccountSessionView(pNewAccountsSession);
			pAccountSessionView->Create(&m_SessionViewHolder);
			m_accounts.Add(pNewlyInsertedAccount);
			m_accountSessions.Add(pNewAccountsSession);
			m_accountSessionViews.Add(pAccountSessionView);
			AddAccountSession(pAccountSessionView);
		}
		catch(CSQLException* e)
		{
			AfxMessageBox(e->GetError(),MB_OK);
			e->Delete();
			return;
		}
	}

}

void CRepoAccountsSyncCtrlDlg::OnDeleteAccountClick()
{
	if(AfxMessageBox(IDS_CONFIRMDELETEREPOACCOUNT,MB_YESNO) == IDNO)
		return;

	HTREEITEM hSelectedItem = m_accountsTree.GetSelectedItem();
		
	m_SessionViewHolder.Show(NULL);
	
	RepoAccountsTreeItemAction* pItemAction = (RepoAccountsTreeItemAction*)m_accountsTree.GetItemData(hSelectedItem);
	RepoAccountSessionView* pSessionView = pItemAction->GetAccountSessionView();

	pSessionView->Disconnect();

	RepoAccountSession* pSession = pSessionView->GetAccountSession();
	const RepoAccount* pAccount = pSession->GetAccount();

	RepoAccountDAO repoAccountDao;
	try
	{
		Transaction transaction = repoAccountDao.OpenTransaction();
		repoAccountDao.DeleteRepoAccount(transaction,pSession->GetAccount());
		transaction->Commit();
	}
	catch(CSQLException* e)
	{
		e->Delete();
		return;
	}
	
	DisableAccountToolbarButtons(NULL);

	m_accountsTree.DeleteItem(hSelectedItem);

	m_accountSessionViews.RemovePtr(pSessionView);
	m_accountSessions.RemovePtr(pSession);
	m_accounts.RemovePtr(pAccount);

	delete pItemAction;
	delete pSessionView;
	delete pSession;
	delete pAccount;

	

		
}
