#ifndef __STOCKDATAARRAY_H__
#define __STOCKDATAARRAY_H__

#include "../DataArray.h"
#include "Stock.h"
#include "StockDataComparator.h"

class CStockDataArray : public CDataArray<Stock,StockDataComparator> 
{
public:
	Stock* GetStockDataByStockId(int stockid);
	Stock* GetStockDataByStockCode(LPCSTR stockCode);
};




#endif