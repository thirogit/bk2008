#ifndef __COWPRICELISTVIEW_H__
#define  __COWPRICELISTVIEW_H__

#include "../BaseListCtrl.h"
#include "../../DataTypes.h"
#include "../../accessor/CowAccessor.h"

class CCowPriceViewList : public CBaseListCtrl
{
public:
	CCowPriceViewList(Invoice *invoice);
	void UpdateItemCount();
	~CCowPriceViewList();
	
private:
	static const GRIDHEADERS szCowPriceListViewCols[];
 	CCowInvoiceEntryArray m_cowPrices;
	CowAccessor *m_cowAccessor;
	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);	
	virtual void SortByCol(int Column,bool bAsc);	
protected:
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP();
};

#endif