#include "stdafx.h"
#include "SQLStockDataMember.h"

SQLStockDataMember::SQLStockDataMember(StockDataMember* pStockMember) :
		SQLDataMember(pStockMember) 
{
}
 CString SQLStockDataMember::GetSQLStringValue(StockDataMember* pStockMember)
 {
	Stock* pStock = pStockMember->GetValue();
	if(pStock)
	{
		return FormatInt(pStock->GetId());
	}
	else
		return GetSQLNull();
	
}