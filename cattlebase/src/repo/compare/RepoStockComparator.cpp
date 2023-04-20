#include "stdafx.h"
#include "RepoStockComparator.h"

COMPARE_RESULT RepoStockComparator::CompareBy(UINT dataMember,RepoStock* DataLeft,RepoStock* DataRight)
{
	RepoStock *left = DataLeft,*right = DataRight;
	
	switch(dataMember)
	{
		default:
			ASSERT(0);
		case REPOSTOCK_MEMBER_NAME:
			return PolishStringCompare(left->GetStockName(),right->GetStockName());
		case REPOSTOCK_MEMBER_CODE:
			return PolishStringCompare(left->GetStockCode(),right->GetStockCode());
		case REPOSTOCK_MEMBER_MYPRICE:
			return CompareMoney(left->GetMyPerKgPrice(),right->GetMyPerKgPrice());
		case REPOSTOCK_MEMBER_PREDEFSEX:
			return CompareType<CowSex>(left->GetPredefSex(),right->GetPredefSex());		
	}
	ASSERT(0);
	return COMPARE_LESS;
}
