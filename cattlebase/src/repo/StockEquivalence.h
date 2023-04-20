#ifndef __STOCKEQUIVALENCE_H__
#define __STOCKEQUIVALENCE_H__

#include "TypedEquivalence.h"
#include "RepoStock.h"
#include "compare/RepoStockComparator.h"

class StockEquivalence : public TypedEquivalence<RepoStock>
{
public:
	StockEquivalence(RepoStock* pLocalStock,RepoStock* pRemoteStock);
	virtual bool IsEqual();
private:
	RepoStockComparator m_Comparator;

};

#endif