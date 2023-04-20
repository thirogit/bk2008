#ifndef __STOCKDATACOMPARATOR_H__
#define __STOCKDATACOMPARATOR_H__

#include "RawData.h"
#include "Stock.h"
#include "DataComparator.h"

enum STOCKDATA_MEMBERS
{
	STOCKS_SORT_BY_STOCKNAME = RAWDATA_MEMBER_BASE+1,
	STOCKS_SORT_BY_STOCKCODE,
	STOCKS_SORT_BY_RULE,
	STOCKS_SORT_BY_MYPERKGPRICE,
	STOCK_SORT_BY_PREDEFSEX,
	STOCK_SORT_BY_OFFICIALCODE
};


class StockDataComparator : public DataComparator<Stock>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,Stock* DataLeft,Stock* DataRight);
};


#endif