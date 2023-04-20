#include "stdafx.h"
#include "CowPriceDataComparator.h"
#include "../accessor/CowAccessor.h"


CowPriceDataComparator::CowPriceDataComparator(UINT nDataMember,bool bAsc) : CSortArray<CowPriceData>::SortArrayComparator(bAsc),m_nDataMember(nDataMember)
{
}

COMPARE_RESULT CowPriceDataComparator::Compare(CowPriceData *DataLeft,CowPriceData *DataRight)
{
	return CompareBy(m_nDataMember,DataLeft,DataRight);
}

COMPARE_RESULT CowPriceDataComparator::CompareBy(UINT dataMember,CowPriceData* DataLeft,CowPriceData* DataRight)
{
	switch(dataMember)
	{
		default:
			return CompareCommon(dataMember,DataLeft,DataRight);

		case COWPRICEDATA_OTHERPRICE:
			return CompareOtherPrice(DataLeft,DataRight);
			
		case COWPRICEDATA_HASTERM:
			return CompareHasTerm(DataLeft,DataRight);

		case COWPRICEDATA_HENT:
			return CompareHent(DataLeft,DataRight);
	}
	ASSERT(0);
	return COMPARE_LESS;

}

COMPARE_RESULT CowPriceDataComparator::CompareCommon(UINT dataMember,CowPriceData* DataLeft,CowPriceData* DataRight)
{
	switch(dataMember)
	{
		case COWPRICEDATA_WEIGHT:	
			return CompareType<Weight>(DataLeft->GetWeight(),DataRight->GetWeight());
		case COWPRICEDATA_EAN:
			return PolishStringCompare(DataLeft->GetCow()->GetEAN(),DataRight->GetCow()->GetEAN());
		case COWPRICEDATA_STOCK:
			return PolishStringCompare(DataLeft->GetStock()->GetStockCode(),DataRight->GetStock()->GetStockCode());
		case COWPRICEDATA_RULE:
		{
			Rule *leftRule = DataLeft->GetPriceRule();
			Rule *rightRule = DataRight->GetPriceRule();
			if(leftRule)
			{
				if(rightRule)
					return PolishStringCompare(leftRule->GetRuleName(),rightRule->GetRuleName());
				else
					return COMPARE_GREATHER;
			}
            else
			{
				if(rightRule)
					 return COMPARE_LESS;
				else 
					return COMPARE_EQUAL;
			}
		}
		case COWPRICEDATA_MYPRICE:
			return CompareMoney(DataLeft->GetCow()->GetMyPrice() , DataRight->GetCow()->GetMyPrice());			
		case COWPRICEDATA_PRICEPERKG:
			return CompareMoney(DataLeft->GetPricePerKg(),DataRight->GetPricePerKg());			
		case COWPRICEDATA_BRUTTO:
		case COWPRICEDATA_VAT:
		case COWPRICEDATA_NETTO:
			return CompareMoney(DataLeft->GetCowPriceNetto(),DataRight->GetCowPriceNetto());		
	}
	ASSERT(0);
	return COMPARE_LESS;		
}

COMPARE_RESULT CowPriceDataComparator::CompareHasTerm(BuyOrSell buyOrSell,CowPriceData* DataLeft,CowPriceData* DataRight)
{
	CowAccessor *cowAccessorLeft = CowAccessorFactory::createAccessor(buyOrSell);
	CowAccessor *cowAccessorRight = CowAccessorFactory::createAccessor(buyOrSell);
	
	Cow* pLeftCow = DataLeft->GetCow();
	Cow* pRightCow = DataRight->GetCow();

	bool bLeft = (	cowAccessorLeft->GetTermPrice(pLeftCow) >= 0.01 || 
					cowAccessorLeft->GetTermStock(pLeftCow) != NULL || 
					!cowAccessorLeft->GetTermWeight(pLeftCow).IsNull()
				 ),
		 bRight = (
					cowAccessorRight->GetTermPrice(pRightCow) >= 0.01 || 
					cowAccessorRight->GetTermStock(pRightCow) != NULL || 
					!cowAccessorRight->GetTermWeight(pRightCow).IsNull()
				  );

	if(bLeft && bRight)
		return COMPARE_EQUAL;
	else
	{
		if(bLeft && !bRight)
			return COMPARE_GREATHER;				
	}
	return COMPARE_LESS;		
}
	