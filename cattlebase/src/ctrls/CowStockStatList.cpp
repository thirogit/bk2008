#include "stdafx.h"
#include "CowStockStatList.h"
#include "../MainCfg.h"

/////////////////////////////////////////////////////////////////////////////////////////
CCowStockStat::CCowStockStat() : statStock(NULL), amount (0)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
COMPARE_RESULT CCowStockStatComparator::CompareBy(UINT dataMember,CCowStockStat* DataLeft,CCowStockStat* DataRight)
{
	switch(dataMember)
	{
	case COWSTATSTOCKLIST_STOCKCODE:
		return PolishStringCompare(DataLeft->statStock->GetStockCode(),DataRight->statStock->GetStockCode());
		
	case COWSTATSTOCKLIST_STOCKAMOUNT:
		return CompareInt(DataLeft->amount,DataRight->amount);
		
	default:
		ASSERT(FALSE);
	}
	return COMPARE_LESS;
}
/////////////////////////////////////////////////////////////////////////////////////////
const GRIDHEADERS CCowStockStatListCtrl::szColHeaders[] =
{
	{IDS_HEADERSTOCK,COWSTATSTOCKLIST_STOCKCODE},
	{IDS_HEADERAMOUNT,COWSTATSTOCKLIST_STOCKAMOUNT},
	{0,0}
};



CCowStockStatListCtrl::CCowStockStatListCtrl(CCowArray &arrayCows) : m_arrayCows(arrayCows),CBaseListCtrl(szColHeaders,CFGID_COWSTOCKSTATLIST)
{
}

CCowStockStatListCtrl::~CCowStockStatListCtrl()
{
	m_stockStatList.FreeAndFlush();
}

void CCowStockStatListCtrl::UpdateStatistic()
{
	m_stockStatList.FreeAndFlush();
	CMap<Stock*,Stock*,CCowStockStat*,CCowStockStat*> stockStatMap;
	Stock *pCowStock = NULL;
	CCowStockStat* stockStat = NULL;
	Cow* pCow = NULL;
	CCowArray::ArrayIterator it = m_arrayCows.iterator();
	while(it.hasNext())
	{
		pCow = *(it++);
		pCowStock = pCow->GetStock();
		stockStat = NULL;
		if(stockStatMap.Lookup(pCowStock,stockStat))
		{
			stockStat->amount++;
		}
		else
		{
			stockStat = new CCowStockStat();
			stockStat->statStock = pCowStock;
			stockStat->amount = 1;
			stockStatMap.SetAt(pCowStock,stockStat);
		}
	}

	POSITION pos = stockStatMap.GetStartPosition();
	while(pos)
	{
		stockStatMap.GetNextAssoc(pos,pCowStock,stockStat);
		m_stockStatList.Add(stockStat);
	}

	UpdateItemCount();

}
void CCowStockStatListCtrl::UpdateItemCount()
{
	SetItemCount(m_stockStatList.GetCount());
}
void CCowStockStatListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	CCowStockStat *pStockStat = m_stockStatList[nItem];
	switch(GetColID(nSubItem))
	{
	case COWSTATSTOCKLIST_STOCKCODE:
		strcpy(pszItemText,(LPCSTR)pStockStat->statStock->GetStockCode());
		break;		
	case COWSTATSTOCKLIST_STOCKAMOUNT:
		sprintf(pszItemText,"%d",pStockStat->amount);
		break;		
	default:
		ASSERT(FALSE);
	}
}
void CCowStockStatListCtrl::SortByCol(int Column,bool bAsc)
{
	m_stockStatList.SortBy(GetColID(Column),bAsc);
}