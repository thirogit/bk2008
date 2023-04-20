#ifndef __STOCKEQUIVALENCEITEM_H__
#define __STOCKEQUIVALENCEITEM_H__

#include "StockEquivalence.h"
#include "UpdatableItem.h"

class StockEquivalenceItem : public StockEquivalence,public UpdatableItem
{
public:
	StockEquivalenceItem(RepoStock* pLocalStock,RepoStock* pRemoteStock);
	
};

#endif