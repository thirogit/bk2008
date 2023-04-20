#ifndef __REPOPURCHASESUMMARYPAGE_H__
#define __REPOPURCHASESUMMARYPAGE_H__

#include "../RepoAccountSession.h"
#include "../../LayoutManager/ETSLayout.h"

class CRepoPurchaseSummaryPage : public ETSLayoutDialog
{
	DECLARE_DYNAMIC(CRepoPurchaseSummaryPage)

public:
	CRepoPurchaseSummaryPage();   
	virtual ~CRepoPurchaseSummaryPage();	
	enum { IDD = IDD_REPO_PURCHASE_SUMMARY };
	void BeforeUpdate();
	void UpdateFrom(RepoPurchase* pPurchase);
private:
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
		
	DECLARE_MESSAGE_MAP()

};

#endif