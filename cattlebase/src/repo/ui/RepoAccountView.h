#ifndef __REPOACCOUNTVIEW_H__
#define __REPOACCOUNTVIEW_H__

#include "../RepoAccountSession.h"
#include "ctrls/RepoToolBar.h"
#include "SessionAwareDialog.h"

class CRepoAccountView : public SessionElementView<CDialog>
{
	DECLARE_DYNAMIC(CRepoAccountView)

public:
	CRepoAccountView();   
	virtual ~CRepoAccountView();	
	enum { IDD = IDD_REPO_ACCOUNT_VIEW };
	void UpdateFrom(RepoAccountSession* pAccountSession);
	virtual void UpdateToolBar(CallbackToolBar& toolbar);
	virtual void BeforeUpdate();
	virtual void AfterUpdate();
	virtual void Create(CWnd* pParentWnd);
private:
	void OnConnect();
	void OnDisconnect();

	RepoAccountSession* m_pAccountSession;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();	
	DECLARE_MESSAGE_MAP()
};

#endif