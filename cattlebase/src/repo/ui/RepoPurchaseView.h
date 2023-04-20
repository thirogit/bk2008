#ifndef __REPOPURCHASEVIEW_H__
#define __REPOPURCHASEVIEW_H__

#include "../RepoAccountSession.h"
#include "ctrls/RepoPurchaseListCtrl.h"
#include "RepoPurchaseInvoicesPage.h"
#include "RepoPurchaseHentsPage.h"
#include "RepoPurchaseSummaryPage.h"
#include "../../LayoutManager/ETSLayout.h"
#include "../../ctrls/XTabCtrl/XTabCtrl.h"
#include "ctrls/RepoToolBar.h"
#include "SessionAwareDialog.h"

class CRepoPurchaseView : public SessionElementView<ETSLayoutDialog>
{
	DECLARE_DYNAMIC(CRepoPurchaseView)

public:
	CRepoPurchaseView();   
	virtual ~CRepoPurchaseView();	
	enum { IDD = IDD_REPO_PURACHES };
	void UpdateFrom(RepoAccountSession* pAccountSession);
	virtual void UpdateToolBar(CallbackToolBar& toolbar);
	virtual void BeforeUpdate();
	virtual void AfterUpdate();
	virtual void Create(CWnd* pParentWnd);
private:
	RepoAccountSession* m_pAccountSession;
	CRepoPurchaseListCtrl m_PurchaseList;

	CRepoPurchaseHentsPage m_PurchaseHentsPage;
	CRepoPurchaseInvoicesPage m_PurchaseInvoicesPage;
	CRepoPurchaseSummaryPage m_PurchaseSummaryPage;
	CXTabCtrl m_PurchaseTabs;

	PurchaseItem* m_pCurrentSelection;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	void DownloadSelected();
	void OnPurchaseSelected(PurchaseItem* pItem);
	void OnPurchaseUnselected(PurchaseItem* pItem);


	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPurchaseClick(NMHDR* pNMHDR, LRESULT* pResult);
	
	
	DECLARE_MESSAGE_MAP()

};

#endif