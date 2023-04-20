#include "stdafx.h"
#include "SellCowPriceDataComparator.h"
#include "HentComparator.h"
#include "../accessor/CowAccessor.h"

SellCowPriceDataComparator::SellCowPriceDataComparator(UINT nDataMember,bool bAsc) : CowPriceDataComparator(nDataMember,bAsc)
{

}


COMPARE_RESULT SellCowPriceDataComparator::CompareOtherPrice(CowPriceData* DataLeft,CowPriceData* DataRight)
{
	Cow *leftCow = DataLeft->GetCow();
	Cow *rightCow = DataRight->GetCow();
	
	if(leftCow->HasAnyBuyInvoice())
	{
		if(rightCow->HasAnyBuyInvoice())
			return CompareMoney(leftCow->GetBuyPrice(),rightCow->GetBuyPrice());
		else
			return COMPARE_GREATHER;
	}
    else
	{
		if(rightCow->HasAnyBuyInvoice())
			 return COMPARE_LESS;
		else 
			return COMPARE_EQUAL;
	}
}
COMPARE_RESULT SellCowPriceDataComparator::CompareHent(CowPriceData* DataLeft,CowPriceData* DataRight)
{
	return HentComparator().CompareBy(HENTS_SORT_BY_ALIAS,DataLeft->GetCow()->GetDeliver(),DataRight->GetCow()->GetDeliver());
}

COMPARE_RESULT SellCowPriceDataComparator::CompareHasTerm(CowPriceData* DataLeft,CowPriceData* DataRight)
{
	return CowPriceDataComparator::CompareHasTerm(BUY,DataLeft,DataRight);
	
}