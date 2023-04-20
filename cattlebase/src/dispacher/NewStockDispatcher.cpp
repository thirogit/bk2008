#include "stdafx.h"
#include "NewStockDispatcher.h"
#include "../global_arrays.h"

void NewStockDispatcher::Dispatch(Stock* pNewStock)
{
	ary_stocks.Add(pNewStock);
}
