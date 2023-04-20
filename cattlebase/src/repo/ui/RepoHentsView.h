#ifndef __REPOHENTSVIEW_H__
#define __REPOHENTSVIEW_H__

#include "../RepoAccountSession.h"
#include "ctrls/RepoHentsListCtrl.h"
#include "../../LayoutManager/ETSLayout.h"
#include "ctrls/RepoToolBar.h"
#include "SessionAwareDialog.h"


class CRepoHentsView : public SessionElementView<ETSLayoutDialog>
{
	DECLARE_DYNAMIC(CRepoHentsView)

public:
	CRepoHentsView();   
	virtual ~CRepoHentsView();	
	enum { IDD = IDD_REPO_ITEMS };
	virtual void UpdateFrom(RepoAccountSession* pAccountSession);
	virtual void UpdateToolBar(CallbackToolBar& toolbar);
	virtual void BeforeUpdate();
	virtual void AfterUpdate();    
	virtual void Create(CWnd* pParentWnd);
private:
	RepoAccountSession* m_pAccountSession;
	CRepoHentsListCtrl m_HentsList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();

	void SelectAll();
	void UnselectAll();
	void SelectMissingOnRemoteSide();
	void SelectMissingOnLocaclSide();
	void SelectNotEqualSides();
	void UploadSelected();
	void DownloadSelected();
		
	DECLARE_MESSAGE_MAP()

};

#endif