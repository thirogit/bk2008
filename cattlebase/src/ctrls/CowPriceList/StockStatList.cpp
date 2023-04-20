#include "stdafx.h"
#include "StockStatList.h"
#include "MainCfg.h"
#include "../../classes/IntString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#############################################################################
COMPARE_RESULT CStockStatComparator::CompareBy(UINT sortBy,StockStatData *DataLeft,StockStatData *DataRight)
{
	StockStatData *left = DataLeft,*right = DataRight;
		
	switch(sortBy)
	{
		default:
			ASSERT(0);
		case STOCKSTAT_SORT_BY_STOCKCODE:
			return m_stockComparator.CompareBy(STOCKS_SORT_BY_STOCKCODE,left->pStock,right->pStock);
			
		case STOCKSTAT_SORT_BY_SUMPRICE:
			return CompareMoney(left->sumPrice , right->sumPrice);
		
		case STOCKSTAT_SORT_BY_AVGPRICEPERKG:
			return CompareMoney(left->avgPricePerKg , right->avgPricePerKg);

		case STOCKSTAT_SORT_BY_COWSWITHCOUNT:
			return CompareInt(left->cowsWithThisStockCount,right->cowsWithThisStockCount);


		case STOCKSTAT_SORT_BY_SUMWEIGHT:
			return CompareType<Weight>(left->sumWeight,right->sumWeight);
	}

}

//#############################################################################
void CStockStatSet::ReStat(const CowPriceInvoiceSession* pInvoiceSession)
{
	int i,s;
	StockStatData *pStockStat = NULL;
	Stock *curStock = NULL;
	FreeAndFlush();
	stockStatMap.RemoveAll();
	for(i = 0,s = pInvoiceSession->GetCowCount();i < s;i++)
	{
		
		CowPriceData *pCow = pInvoiceSession->GetCowPrice(i);
		curStock = pCow->GetStock();

		if(stockStatMap.Lookup(curStock,pStockStat))
		{
			pStockStat->cowsWithThisStockCount++;
			pStockStat->sumWeight += pCow->GetWeight(); 
			pStockStat->sumPrice += pCow->GetCowPriceNetto();	
		}
		else
		{
			pStockStat = new StockStatData();			
			pStockStat->cowsWithThisStockCount = 1;
			pStockStat->sumWeight = pCow->GetWeight(); 
			pStockStat->sumPrice = pCow->GetCowPriceNetto();			
			pStockStat->pStock = curStock;
			Add(pStockStat);
			stockStatMap.SetAt(curStock,pStockStat);
		}
	}

	for(i = 0,s = GetSize();i < s;i ++)
	{
		pStockStat = GetAt(i);
		if(pStockStat->sumWeight > 0)
		{
			pStockStat->avgPricePerKg = pStockStat->sumPrice / pStockStat->sumWeight;
		}
	}

}
//#############################################################################
const GRIDHEADERS CStockStatList::szStockStatListCols[] =
{
	{IDS_HEADERSTOCK,STOCKSTAT_SORT_BY_STOCKCODE},
	{IDS_HEADERAMOUNT,STOCKSTAT_SORT_BY_COWSWITHCOUNT},
	{IDS_HEADERSUMWEIGHT,STOCKSTAT_SORT_BY_SUMWEIGHT},
	{IDS_HEADERSUMPRICE,STOCKSTAT_SORT_BY_SUMPRICE},
	{IDS_HERDERAVGPRICEPERKG,STOCKSTAT_SORT_BY_AVGPRICEPERKG},	
	{0,0}
};

BEGIN_MESSAGE_MAP(CStockStatList,CBaseListCtrl)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

CStockStatList::CStockStatList(const CowPriceInvoiceSession* pInvoiceSession) : 
	CBaseListCtrl(szStockStatListCols,CFGID_STOCKSTATLIST),
	m_pInvoiceSession(pInvoiceSession)
{
}


CStockStatList::~CStockStatList() 
{
	m_stockStatSet.FreeAndFlush();
}
void CStockStatList::UpdateItemCount()
{
	SetItemCount(m_stockStatSet.GetSize());
}

void CStockStatList::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	StockStatData *stockStat = m_stockStatSet[nItem];
	CString sText;
	
	switch(GetColID(nSubItem))
	{
		case STOCKSTAT_SORT_BY_STOCKCODE:
			sText = stockStat->pStock->GetStockCode();
		break;
		case STOCKSTAT_SORT_BY_COWSWITHCOUNT:
			sText = (LPCSTR)CIntString(stockStat->cowsWithThisStockCount);
			
		break;
		case STOCKSTAT_SORT_BY_SUMWEIGHT:
			sText = stockStat->sumWeight.ToString();
		break;
		case STOCKSTAT_SORT_BY_SUMPRICE:			
			sText = stockStat->sumPrice.ToString();
		break;
		case STOCKSTAT_SORT_BY_AVGPRICEPERKG:
			sText = stockStat->avgPricePerKg.ToString();
		break;
		default:
			ASSERT(0);
	}
	strcpy(pszItemText,sText);
}

void CStockStatList::SortByCol(int Column,bool bAsc)
{
	m_stockStatSet.SortBy((STOCKSTAT_SORT_BY)GetColID(Column),bAsc);
	Invalidate();
}

void CStockStatList::OnShowWindow(BOOL bShow,UINT nStatus)
{
	
	if(bShow)
	{
		m_stockStatSet.ReStat(m_pInvoiceSession);
		UpdateItemCount();
	}
}


