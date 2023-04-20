#ifndef __REPOPURCHASEHENTSPAGE_H__
#define __REPOPURCHASEHENTSPAGE_H__

#include "../RepoAccountSession.h"
#include "ctrls/RepoPurchaseHentsListCtrl.h"
#include "../../LayoutManager/ETSLayout.h"

class CRepoPurchaseHentsPage : public ETSLayoutDialog
{
	DECLARE_DYNAMIC(CRepoPurchaseHentsPage)

public:
	CRepoPurchaseHentsPage();   
	virtual ~CRepoPurchaseHentsPage();	
	enum { IDD = IDD_REPO_ITEMS };
	void BeforeUpdate();
	void UpdateFrom(RepoPurchase* pPurchase);
private:
	//RepoAccountSession* m_pAccountSession;
	CRepoPurchaseHentsListCtrl m_HentList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	

	
	DECLARE_MESSAGE_MAP()

};

#endif