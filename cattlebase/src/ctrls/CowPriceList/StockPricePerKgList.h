#ifndef __STOCKPRICEPERKGLIST_H__
#define  __STOCKPRICEPERKGLIST_H__

#include "../BaseListCtrl.h"
#include "../EditCellTemplate.h"
#include <set>
#include "../../datatypes/StockDataComparator.h"
#include "../../datatypes/invoice/CowPriceInvoiceSession.h"

class CStockPricePerKgList;


class CStockSet : public CStockDataArray
{
public:
	bool AddDistinct(Stock *pStock) 
	{
		for(int i  = 0,s = GetSize();i < s;i ++)
		{
			if(GetAt(i) == pStock)
				return false;
		}
		Add(pStock);
		return true;
	}
	
};

class CStockPricePerKgList : public CBaseListCtrl
{
public:

	enum STOCKPRICEPERKGLIST_COLS
	{
		STOCKPERKGPRICE_COL1 = STOCKS_SORT_BY_STOCKCODE,
		STOCKPERKGPRICE_COL2 = STOCKS_SORT_BY_MYPERKGPRICE
	};

	CStockPricePerKgList(const CowPriceInvoiceSession* pInvoiceSession);
	void UpdateItemCount();
	Stock * GetStockItem(int iItem);
private:
	static const GRIDHEADERS szStockPricePerKgListCols[]; 
	CStockSet m_stockSet;
	const CowPriceInvoiceSession* m_pInvoiceSession;
	
protected:
	void SortByCol(int Column,bool bAsc);
	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	//CComboBox *RuleComboSubItem(int Item, int Column);
	CFloatEditCell* FloatEditSubItem (int Item, int Column,double initValue,int prec = 4);
	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus);

	DECLARE_MESSAGE_MAP();
};
#endif