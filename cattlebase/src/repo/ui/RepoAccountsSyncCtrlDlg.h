#ifndef __REPOACCOUNTSSYNCCTRLDLG_H__
#define __REPOACCOUNTSSYNCCTRLDLG_H__

#include "../RepoAccount.h"
#include "../../PtrFlushArray.h"
#include "../RepoClient.h"
#include "../RepoAccountSession.h"
#include <boost/function.hpp>
#include "../context/Context.h"
#include "../../ctrls/XColorStatic.h"
#include "ctrls/RepoAccountToolBar.h"
#include "ctrls/RepoToolBar.h"
#include "RepoAccountSessionView.h"
#include "ViewHolder.h"
#include "../ForwardRepoSessionObserver.h"
#include <map>

class RepoAccountsTreeItemAction
{
public:
	RepoAccountsTreeItemAction(RepoAccountSessionView* pAccountSessionView);
	void SetOnDblClickAction(boost::function<void (RepoAccountSessionView*)> &onDblClickAction);
	void SetOnSelectedAction(boost::function<void (RepoAccountSessionView*)> &onSelectedAction);
	void SetOnUnelectedAction(boost::function<void (RepoAccountSessionView*)> &onUnselectedAction);

	void OnDblClick();
	void OnSelected();
	void OnUnselected();

	RepoAccountSessionView* GetAccountSessionView();
private:
	void NoAction(RepoAccountSessionView* pAccountSessionView);

	boost::function<void (RepoAccountSessionView*)> m_OnDblClickAction;
	boost::function<void (RepoAccountSessionView*)> m_OnSelectedAction;
	boost::function<void (RepoAccountSessionView*)> m_OnUnselectedAction;

	RepoAccountSessionView* m_pAccountSessionView;
};


class CRepoAccountsSyncCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CRepoAccountsSyncCtrlDlg)

public:
	CRepoAccountsSyncCtrlDlg(CWnd* pParent = NULL);   
	virtual ~CRepoAccountsSyncCtrlDlg();
	
	enum { IDD = IDD_REPOACCOUNTSSYNCCTRLDLG };

private:

	PtrFlushArray<RepoAccount> m_accounts;
	PtrFlushArray<RepoAccountSession> m_accountSessions;
	PtrFlushArray<RepoAccountSessionView> m_accountSessionViews;
	std::map< HTREEITEM,ForwardRepoSessionObserver* > m_ItemObservers;

	CTreeCtrl m_accountsTree;

	

	CXColorStatic m_MissingLocalSideClrLbl;
	CXColorStatic m_BothSidesNotEqualClrLbl;
	CXColorStatic m_MissingRemoteSideClrLbl;

	CRepoAccountToolBar m_repoAccountsToolBar;
	ViewHolder m_SessionViewHolder;

	
	

	Context m_Context;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	void DoConnect(RepoAccountSessionView* pAccountSessionView);
	void UpdateAccountToolbarButtons(RepoAccountSessionView* pAccountSessionView);
	void DisableAccountToolbarButtons(RepoAccountSessionView* pAccountSessionView);

	void OnAccountItemSelected(RepoAccountSessionView* pAccountSessionView);
	

	void TraverseDeleteItemData(HTREEITEM hItem);
	void DeleteItemChildren(HTREEITEM hParentItem);
	void ShowRightHandSide();
	void HideRightHandSide();
	void AddAccountSession(RepoAccountSessionView* pAccountSessionView);


	void OnAccountHentsSelected(RepoAccountSessionView* pAccountSessionView);
	void OnAccountStocksSelected(RepoAccountSessionView* pAccountSessionView);
	void OnAccountCompanySelected(RepoAccountSessionView* pAccountSessionView);
	void OnAccountInboxSelected(RepoAccountSessionView* pAccountSessionView);

	void OnConnected(HTREEITEM hAccountItem);
	void OnDisconnected(HTREEITEM hAccountItem);
	
	void OnAddNewAccountClick();
	void OnDeleteAccountClick();
	void OnEditAccountClick();

	afx_msg void OnItemDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg	void OnItemSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();


	


	DECLARE_MESSAGE_MAP()

};

#endif //__HENTSYNCCTRLDLG_H__