#ifndef __COWSTOCKSTATLIST_H__
#define __COWSTOCKSTATLIST_H__

#include "../datatypes/Cow.h"
#include "../datatypes/CowDataArray.h"
#include "../ctrls/BaseListCtrl.h"


#define COWSTATSTOCKLIST_STOCKCODE 5001
#define COWSTATSTOCKLIST_STOCKAMOUNT 1005

class CCowStockStat
{
public:
	CCowStockStat();
	Stock *statStock;
	int amount;
};

class CCowStockStatComparator : public DataComparator<CCowStockStat>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,CCowStockStat* DataLeft,CCowStockStat* DataRight);
};

class CCowStockStatList : public CDataArray<CCowStockStat,CCowStockStatComparator>
{	
};


class CCowStockStatListCtrl : public CBaseListCtrl
{
public:
	CCowStockStatListCtrl(CCowArray &arrayCows);	
	void UpdateStatistic();
	~CCowStockStatListCtrl();
protected:
	void UpdateItemCount();
	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	virtual void SortByCol(int Column,bool bAsc);

private:
	static const GRIDHEADERS szColHeaders[];
	CCowArray &m_arrayCows;
	CCowStockStatList m_stockStatList;
	
};



#endif