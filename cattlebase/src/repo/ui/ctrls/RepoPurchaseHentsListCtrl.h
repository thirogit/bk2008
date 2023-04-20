#ifndef __REPOPURCHASEHENTSLISTCTRL_H__
#define __REPOPURCHASEHENTSLISTCTRL_H__

#include "../../../ctrls/BaseListCtrl.h"
#include "../../RepoAccountSession.h"
#include "../../compare/RepoHentComparator.h"
#include "../../text/RepoHentTextProvider.h"

class CRepoPurchaseHentsListCtrl : public CBaseListCtrl
{
	static GRIDHEADERS LIST_HEADER_COLUMNS[];
public:
	CRepoPurchaseHentsListCtrl(int iConfigId = -1);
    void BeforeUpdate();	
	void UpdateListWith(RepoPurchase* pPurchase);
	
protected:
	PtrFlushArray<RepoHent> m_pHentItems;
	RepoHentComparator m_Comparator;
	RepoHentTextProvider m_TextProvider;
			
	void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	
	virtual void UpdateItemCount();
	
	
protected:
	
	DECLARE_MESSAGE_MAP()

};




#endif