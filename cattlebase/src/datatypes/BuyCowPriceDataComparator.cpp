#include "stdafx.h"
#include "BuyCowPriceDataComparator.h"
#include "HentComparator.h"

BuyCowPriceDataComparator::BuyCowPriceDataComparator(UINT nDataMember,bool bAsc) : CowPriceDataComparator(nDataMember,bAsc)
{

}

COMPARE_RESULT BuyCowPriceDataComparator::CompareOtherPrice(CowPriceData* DataLeft,CowPriceData* DataRight)
{
	Cow *leftCow = DataLeft->GetCow();
	Cow *rightCow = DataRight->GetCow();

	if(leftCow->HasVATSellInvoice())
	{
		if(rightCow->HasVATSellInvoice())
			return CompareMoney(leftCow->GetSellPrice(),
									rightCow->GetSellPrice());
		else
			return COMPARE_GREATHER;
	}
    else
	{
		if(rightCow->HasVATSellInvoice())
			 return COMPARE_LESS;
		else 
			return COMPARE_EQUAL;
	}
}
COMPARE_RESULT BuyCowPriceDataComparator::CompareHent(CowPriceData* DataLeft,CowPriceData* DataRight)
{
	Cow *leftCow = DataLeft->GetCow();
	Cow *rightCow = DataRight->GetCow();

	if(leftCow->IsOut())
	{
		if(rightCow->IsOut())
			return HentComparator().CompareBy(HENTS_SORT_BY_ALIAS,leftCow->GetBuyer(),rightCow->GetBuyer());
		else
			return COMPARE_GREATHER;
	}
    else
	{
		if(rightCow->IsOut())
			 return COMPARE_LESS;
		else 
			return COMPARE_EQUAL;
	}
}

COMPARE_RESULT BuyCowPriceDataComparator::CompareHasTerm(CowPriceData* DataLeft,CowPriceData* DataRight)
{
	return CowPriceDataComparator::CompareHasTerm(SELL,DataLeft,DataRight);
}