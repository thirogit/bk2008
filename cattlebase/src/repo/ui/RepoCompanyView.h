#ifndef __REPOCOMPANYVIEW_H__
#define __REPOCOMPANYVIEW_H__

#include "../RepoAccountSession.h"
#include "ctrls/RepoToolBar.h"
#include "SessionAwareDialog.h"

class CRepoCompanyView : public SessionElementView<CDialog>
{
	DECLARE_DYNAMIC(CRepoCompanyView)

public:
	CRepoCompanyView();   
	virtual ~CRepoCompanyView();	
	enum { IDD = IDD_REPO_COMPANY_VIEW };
	void UpdateFrom(RepoAccountSession* pAccountSession);
	virtual void UpdateToolBar(CallbackToolBar& toolbar);
	virtual void BeforeUpdate();
	virtual void AfterUpdate();
	virtual void Create(CWnd* pParentWnd);
private:
	RepoAccountSession* m_pAccountSession;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	void UpdateCompany();



	DECLARE_MESSAGE_MAP()
};

#endif