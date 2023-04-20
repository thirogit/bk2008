#ifndef __SQLSTOCKDATAMEMBER_H__
#define __SQLSTOCKDATAMEMBER_H__

#include "../../Cow.h"
#include "../SQLDataMember.h"

class SQLStockDataMember : public SQLDataMember<StockDataMember>
{
public:
	SQLStockDataMember(StockDataMember* pStockMember);
	virtual CString GetSQLStringValue(StockDataMember* pStockMember);	
};


#endif