#include "stdafx.h"
#include "StockDataArray.h"
#include "StockDataComparator.h"

Stock* CStockDataArray::GetStockDataByStockId(int stockid)
{
	Stock *pStock = NULL;
	for(int i=0,s = GetSize();i < s;i++)
	{
		pStock = GetAt(i);
		if(pStock->GetId() == stockid)	
			return pStock;
		
	}
	return NULL;
}

Stock* CStockDataArray::GetStockDataByStockCode(LPCSTR stockCode)
{
	ASSERT(stockCode);
	Stock *pStock = NULL;

	for(int i=0,s = GetSize();i < s;i++)
	{		
		pStock = GetAt(i);
		if(!pStock->GetStockCode().Compare(stockCode))	
			return pStock;		
	}
	return NULL;

}




