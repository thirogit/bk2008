#ifndef __REPOINVOICESLISTCTRL_H__
#define __REPOINVOICESLISTCTRL_H__

#include "../../../ctrls/BaseListCtrl.h"
#include "../../RepoAccountSession.h"
#include "../../compare/RepoInvoiceComparator.h"
#include "../../text/RepoInvoiceTextProvider.h"

class CRepoInvoicesListCtrl : public CBaseListCtrl
{
	static GRIDHEADERS LIST_HEADER_COLUMNS[];
public:
	CRepoInvoicesListCtrl(int iConfigId = -1);
	void BeforeUpdate();
	void UpdateListWith(RepoPurchase* pPurchase);
	RepoInvoice* GetItemAt(int iItemIndex);
protected:
	PtrFlushArray<RepoInvoice> m_pInvoiceItems;
	RepoInvoiceComparator m_Comparator;
	RepoInvoiceTextProvider m_TextProvider;
	//CImageList m_checkboxImageList;
		
	void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	//virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
	virtual void UpdateItemCount();
	//virtual int GetItemImage(int iItem,int Column);
	//virtual void PostCreate();
protected:
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

};




#endif