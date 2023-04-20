#ifndef __STOCKSTATLIST_H__
#define  __STOCKSTATLIST_H__

#include "../../datatypes/CowPriceData.h"
#include "../BaseListCtrl.h"
#include "../../datatypes/StockDataComparator.h"
#include "../../datatypes/invoice/CowPriceInvoiceSession.h"
#include <map>


enum STOCKSTAT_SORT_BY
{	
	STOCKSTAT_SORT_BY_STOCKCODE = 1,
	STOCKSTAT_SORT_BY_COWSWITHCOUNT,
	STOCKSTAT_SORT_BY_SUMWEIGHT,
	STOCKSTAT_SORT_BY_SUMPRICE,
	STOCKSTAT_SORT_BY_AVGPRICEPERKG	
};

class StockStatData 
{
public:
	StockStatData() : 
	  cowsWithThisStockCount(0),
		  sumPrice(ZEROMONEY),
		  avgPricePerKg(ZEROMONEY),
		  pStock(NULL)
	  {};

	int cowsWithThisStockCount;
	Weight sumWeight;
	Money sumPrice;
	Money avgPricePerKg;
	Stock *pStock;
};

class CStockStatComparator : public DataComparator<StockStatData>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,StockStatData* DataLeft,StockStatData* DataRight);
private:
	StockDataComparator m_stockComparator;
};


class CStockStatSet : public CDataArray<StockStatData,CStockStatComparator>
{
public:
	void ReStat(const CowPriceInvoiceSession* pInvoiceSession); 	
protected:	
	CMap<Stock*,Stock*,StockStatData*,StockStatData*> stockStatMap;	
};

class CStockStatList : public CBaseListCtrl
{
public:	
	CStockStatList(const CowPriceInvoiceSession* pInvoiceSession);
	void UpdateItemCount();
	virtual ~CStockStatList();
private:
	static const GRIDHEADERS szStockStatListCols[]; 
	const CowPriceInvoiceSession* m_pInvoiceSession;
	CStockStatSet m_stockStatSet;	
protected:
	void SortByCol(int Column,bool bAsc);
	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);


	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus);
	DECLARE_MESSAGE_MAP();
};
#endif