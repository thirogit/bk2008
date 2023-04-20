#include "stdafx.h"
#include "StockDataComparator.h"

COMPARE_RESULT StockDataComparator::CompareBy(UINT dataMember,Stock* DataLeft,Stock* DataRight)
{
	Stock *left = DataLeft,*right = DataRight;
	
	switch(dataMember)
	{
		default:
			ASSERT(0);

		case STOCKS_SORT_BY_STOCKNAME:
			return PolishStringCompare(left->GetStockName(),right->GetStockName());
		case STOCKS_SORT_BY_STOCKCODE:
			return PolishStringCompare(left->GetStockCode(),right->GetStockCode());
		case STOCKS_SORT_BY_RULE:
			if(left->GetRule())
			{
				if(right->GetRule())
					return PolishStringCompare(left->GetRule()->GetRuleName(),
												right->GetRule()->GetRuleName());
				else
					return COMPARE_GREATHER;
			}
            else
			{
				if(right->GetRule())
					 return COMPARE_LESS;
				else 
					return COMPARE_EQUAL;
			}
			
		case STOCKS_SORT_BY_MYPERKGPRICE:
			return CompareMoney(left->GetMyPerKgPrice() , right->GetMyPerKgPrice());
		case STOCK_SORT_BY_PREDEFSEX:
			if(left->GetPredefSex() >  right->GetPredefSex())
				return COMPARE_GREATHER;
			else
			{
				if(left->GetPredefSex() == right->GetPredefSex())
					return COMPARE_EQUAL;
				else
					return COMPARE_LESS;
			}
		case STOCK_SORT_BY_OFFICIALCODE:
			return  PolishStringCompare(left->GetOfficialCode(),right->GetOfficialCode());

	}



	ASSERT(0);
	return COMPARE_LESS;
}
