#ifndef __NEWSTOCKDISPATCHER_H__
#define __NEWSTOCKDISPATCHER_H__

#include "../datatypes/Stock.h"

class NewStockDispatcher
{
public:
	static void Dispatch(Stock* pNewStock);
};

#endif