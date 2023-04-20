#ifndef __REPOSTOCKSLISTCTRL_H__
#define __REPOSTOCKSLISTCTRL_H__

#include "../../../ctrls/BaseListCtrl.h"
#include "../../RepoAccountSession.h"
#include "../../compare/RepoStockComparator.h"
#include "../../compare/Predicate.h"
#include "../../text/RepoStockTextProvider.h"

class CRepoStocksListCtrl : public CBaseListCtrl
{
	static GRIDHEADERS LIST_HEADER_COLUMNS[];
public:
	CRepoStocksListCtrl(int iConfigId = -1); 
	void UpdateListWith(StocksCluster* pStocksCluster);
	void Select(Predicate<StockEquivalenceItem*>& selectPredicate,bool bAddToSelected = false);
protected:
	PtrFlushArray<StockEquivalenceItem> m_pStockItems;
	RepoStockComparator m_Comparator;
	RepoStockTextProvider m_TextProvider;
	CImageList m_checkboxImageList;
		
	void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
	virtual void UpdateItemCount();
	virtual int GetItemImage(int iItem,int Column);
	virtual void PostCreate();
protected:
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

};




#endif