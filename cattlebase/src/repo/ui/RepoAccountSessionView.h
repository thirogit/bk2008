#ifndef __REPOACCOUNTSESSIONVIEW_H__
#define __REPOACCOUNTSESSIONVIEW_H__

#include "../RepoAccountSession.h"
#include "ViewHolder.h"
#include "RepoHentsView.h"
#include "RepoStocksView.h"
#include "RepoPurchaseView.h"
#include "RepoCompanyView.h"
#include "RepoAccountView.h"
#include "SessionAwareDialogFacade.h"
#include "ctrls/RepoToolBar.h"

class RepoAccountSessionView : public CStatic
{		
public:
	enum SessionElement
	{
		Account,
		Hents,
		Stocks,
		Company,
		Purchases
	};
public:
	RepoAccountSessionView(RepoAccountSession* pAccountSession);

	~RepoAccountSessionView();

	void Create(ViewHolder* pViewHolder);
	
	RepoAccountSession* GetAccountSession() const;
	
	void ShowElement(SessionElement element);

	void Connect();

	void Disconnect();

	bool IsConnected() const;

	void RegisterObserver(RepoSessionObserver* pObserver);

	void UnregisterObserver(RepoSessionObserver* pObserver);

	void UpdateAccount(const RepoAccount& accountUpdate);	
	const RepoAccount* GetAccount() const;
	
private:
	void SwitchToAccountView();
	void SwitchToHentsView();
	void SwitchToStocksView();
	void SwitchToCompanyView();
	void SwitchToPurchasesView();	
	void CreateToolBar();
	void PositionToolBar();

	RepoAccountSession* m_pAccountSession;
	ViewHolder* m_pViewHolder;

	CRepoCompanyView m_repoCompanyView;
	CRepoHentsView m_repoHentsView;
	CRepoStocksView m_repoStocksView;
	CRepoPurchaseView m_repoPurchaseView;
	CRepoAccountView m_repoAccountView;
	SessionElementViewFacade* m_pActiveView;

	NewPtrFlushArray<CWnd> m_SessionViews;


	CRepoToolBar m_repoCtrlToolBar;


	afx_msg void OnSize(UINT nType,int cx,int cy);

	DECLARE_MESSAGE_MAP()
};
	

#endif