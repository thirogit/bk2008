#ifndef __REPOSTOCKSVIEW_H__
#define __REPOSTOCKSVIEW_H__

#include "../RepoAccountSession.h"
#include "ctrls/RepoStocksListCtrl.h"
#include "../../LayoutManager/ETSLayout.h"
#include "ctrls/RepoToolBar.h"
#include "SessionAwareDialog.h"

class CRepoStocksView : public SessionElementView<ETSLayoutDialog>
{
	DECLARE_DYNAMIC(CRepoStocksView)

public:
	CRepoStocksView();   
	virtual ~CRepoStocksView();	
	enum { IDD = IDD_REPO_ITEMS_VIEW };
	void UpdateFrom(RepoAccountSession* pAccountSession);
	virtual void UpdateToolBar(CallbackToolBar& toolbar);
	virtual void BeforeUpdate();
	virtual void AfterUpdate();  
	virtual void Create(CWnd* pParentWnd);

private:
	RepoAccountSession* m_pAccountSession;
	CRepoStocksListCtrl m_StocksList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();	

	void SelectAll();
	void UnselectAll();
	void SelectMissingOnRemoteSide();
	void SelectMissingOnLocaclSide();
	void SelectNotEqualSides();
	void UpdateSelected();
	void DownloadSelected();

	
	DECLARE_MESSAGE_MAP()

};

#endif