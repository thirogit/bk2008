#include "stdafx.h"
#include "CowInvoiceEntryComparator.h"
#include "Doc.h"

COMPARE_RESULT CCowInvoiceEntryComparator::CompareBy(UINT dataMember,CowInvoiceEntry* DataLeft,CowInvoiceEntry* DataRight,CowAccessor *pAccessor)
{
	switch(dataMember)
	{
		case COWINVENTRY_SORTBY_DOC:
			return CompareInt(pAccessor->GetDoc(DataLeft->GetCow())->GetId(),pAccessor->GetDoc(DataRight->GetCow())->GetId());
		case COWINVENTRY_SORTBY_EAN:
			return CompareAnsiString(DataLeft->GetCow()->GetEAN(),DataRight->GetCow()->GetEAN());
		case COWINVENTRY_SORTBY_STOCK:
			return PolishStringCompare(DataLeft->GetCow()->GetStock()->GetStockCode(),
									   DataRight->GetCow()->GetStock()->GetStockCode());
		case COWINVENTRY_SORTBY_WEIGHT:
			return CompareType<Weight>(DataLeft->GetCow()->GetWeight(),
								  DataRight->GetCow()->GetWeight());
		case COWINVENTRY_SORTBY_MYPRICE:
			return CompareMoney(DataLeft->GetCow()->GetMyPrice(),
								DataRight->GetCow()->GetMyPrice());
		case COWINVENTRY_SORTBY_NETTOPRICE:
		case COWINVENTRY_SORTBY_VATVALUE:
		case COWINVENTRY_SORTBY_BRUTTOPRICE:
			return CompareMoney(DataLeft->GetPrice(),DataRight->GetPrice());
		case COWINVENTRY_SORTBY_ALIAS:
			return PolishStringCompare(pAccessor->GetDoc(DataLeft->GetCow())->GetHent()->GetAlias(),
									   pAccessor->GetDoc(DataRight->GetCow())->GetHent()->GetAlias());
		case COWINVENTRY_SORTBY_INV_WEIGHT:
			return CompareType<Weight>(DataLeft->GetWeight(),DataRight->GetWeight());
		case COWINVENTRY_SORTBY_INV_STOCK:
			return PolishStringCompare(DataLeft->GetStock()->GetStockCode(),
										DataRight->GetStock()->GetStockCode());
		case COWINVENTRY_SORTBY_TERM_WEIGHT:
			return CompareNullDouble(pAccessor->GetTermWeight(DataLeft->GetCow()),
									pAccessor->GetTermWeight(DataRight->GetCow()));
		case COWINVENTRY_SORTBY_TERM_STOCK:
		{
			Stock* plStock = DataLeft->GetCow()->GetStock();
			Stock* prStock = DataRight->GetCow()->GetStock();
			if(plStock)
			{
				if(prStock)
					return PolishStringCompare(plStock->GetStockCode(),prStock->GetStockCode());
				else
					return COMPARE_GREATHER;
			}
			else
			{
				if(prStock)
					return COMPARE_LESS;
				else
					return COMPARE_EQUAL;
			}
		}
		case COWINVENTRY_SORTBY_TERM_PRICE:
			return CompareMoney(pAccessor->GetTermPrice(DataLeft->GetCow()),
								pAccessor->GetTermPrice(DataRight->GetCow()));

	}

	ASSERT(0);
	return COMPARE_LESS;
	
}
COMPARE_RESULT CCowInvoiceEntryComparator::CompareBy(UINT dataMember,CowInvoiceEntry* DataLeft,CowInvoiceEntry* DataRight)
{
	ASSERT(0);
	return COMPARE_LESS;
}