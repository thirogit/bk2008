#ifndef __REPOPURCHASELISTCTRL_H__
#define __REPOPURCHASELISTCTRL_H__

#include "../../../ctrls/BaseListCtrl.h"
#include "../../RepoAccountSession.h"
#include "../../compare/RepoPurchaseComparator.h"
#include "../../text/RepoPurchaseTextProvider.h"
#include "../../compare/Predicate.h"

class CRepoPurchaseListCtrl : public CBaseListCtrl
{
	static GRIDHEADERS LIST_HEADER_COLUMNS[];
public:
	CRepoPurchaseListCtrl(int iConfigId = -1);
	void BeforeUpdate();
	void UpdateListWith(PurachesCluster* pPurchasesCluster);
	void Select(Predicate<PurchaseItem*>& selectPredicate,bool bAddToSelected = false);
protected:
	PtrFlushArray<PurchaseItem> m_pPurchaseItems;
	RepoPurchaseComparator m_Comparator;
	RepoPurchaseTextProvider m_TextProvider;
	//CImageList m_checkboxImageList;
	
		
	void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
	virtual void UpdateItemCount();
	//virtual int GetItemImage(int iItem,int Column);
	//virtual void PostCreate();
	
protected:
	//afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()

};




#endif