#ifndef __REPOCOWSLISTCTRL_H__
#define __REPOCOWSLISTCTRL_H__

#include "../../../ctrls/BaseListCtrl.h"
#include "../../RepoAccountSession.h"
#include "../../compare/RepoCowComparator.h"
#include "../../text/RepoCowTextProvider.h"

class CRepoCowsListCtrl : public CBaseListCtrl
{
	static GRIDHEADERS LIST_HEADER_COLUMNS[];
public:
	CRepoCowsListCtrl(int iConfigId = -1); 
	void BeforeUpdate();
	void UpdateListWith(RepoInvoice* pInvoice);
protected:
	PtrFlushArray<RepoCow> m_pCowItems;
	RepoCowComparator m_Comparator;
	RepoCowTextProvider m_TextProvider;
	//CImageList m_checkboxImageList;
		
	void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	//virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
	virtual void UpdateItemCount();
	//virtual int GetItemImage(int iItem,int Column);
	//virtual void PostCreate();
protected:
	//afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

};




#endif