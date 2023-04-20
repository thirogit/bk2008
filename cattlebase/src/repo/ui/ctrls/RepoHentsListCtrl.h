#ifndef __REPOHENTSLISTCTRL_H__
#define __REPOHENTSLISTCTRL_H__

#include "../../../ctrls/BaseListCtrl.h"
#include "../../RepoAccountSession.h"
#include "../../compare/RepoHentComparator.h"
#include "../../text/RepoHentTextProvider.h"
#include "../../compare/Predicate.h"




class CRepoHentsListCtrl : public CBaseListCtrl
{
	static GRIDHEADERS LIST_HEADER_COLUMNS[];
public:
	CRepoHentsListCtrl(int iConfigId = -1); 
	void UpdateListWith(HentsCluster* pHentsCluster);
	void Select(Predicate<HentEquivalenceItem*>& selectPredicate,bool bAddToSelected = false);
protected:
	PtrFlushArray<HentEquivalenceItem> m_pHentItems;
	RepoHentComparator m_Comparator;
	RepoHentTextProvider m_TextProvider;
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