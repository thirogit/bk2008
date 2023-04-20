#include "stdafx.h"
#include "RepoCowComparator.h"

COMPARE_RESULT RepoCowComparator::CompareBy(UINT dataMember,RepoCow* DataLeft,RepoCow* DataRight)
{
	RepoCow *left = DataLeft,*right = DataRight;
	
	switch(dataMember)
	{
		default:
			ASSERT(0);	

		case REPOCOW_MEMBER_COWNO:
			return CompareAnsiString(left->GetCowNo(),right->GetCowNo());
		case REPOCOW_MEMBER_COWSEX:
			return CompareType<CowSex>(left->GetSex(),right->GetSex());
		case REPOCOW_MEMBER_STOCK:
			return PolishStringCompare(left->GetStockCode(),right->GetStockCode());
		case REPOCOW_MEMBER_WEIGHT:
			return CompareType<Weight>(left->GetWeight(),right->GetWeight());
		case REPOCOW_MEMBER_PRICE:
			return CompareMoney(left->GetPrice(),right->GetPrice());
	}
	ASSERT(0);
	return COMPARE_LESS;
}
