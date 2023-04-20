#ifndef __REPOPURCHASEINVOICESPAGE_H__
#define __REPOPURCHASEINVOICESPAGE_H__

#include "../RepoAccountSession.h"
#include "ctrls/RepoInvoicesListCtrl.h"
#include "ctrls/RepoCowsListCtrl.h"
#include "../../LayoutManager/ETSLayout.h"

class CRepoPurchaseInvoicesPage : public ETSLayoutDialog
{
	DECLARE_DYNAMIC(CRepoPurchaseInvoicesPage)

public:
	CRepoPurchaseInvoicesPage();   
	virtual ~CRepoPurchaseInvoicesPage();	
	enum { IDD = IDD_REPO_PURCHASE };

	void BeforeUpdate();
	void UpdateFrom(RepoPurchase* pPurchase);
private:
	RepoPurchase* m_pPurchase;
	CRepoInvoicesListCtrl m_InvoiceList;
	CRepoCowsListCtrl	  m_CowList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	
	afx_msg void OnInvoiceClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateRepo();
	DECLARE_MESSAGE_MAP()

};

#endif