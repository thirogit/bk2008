#include "stdafx.h"
#include "CowDataComparator.h"
#include "Doc.h"
#include "RRInvoice.h"

template<typename U>
COMPARE_RESULT CowDataComparator::CompareBuy(Cow *leftCow,Cow *rightCow,
										U leftValue,U rightValue, 
										COMPARE_RESULT (CowDataComparator::*fComapreValues)(Cow*,U,Cow*,U))
{
	if(leftCow->HasAnyBuyInvoice())
	{
		if(rightCow->HasAnyBuyInvoice())
		{			
			return (this->*fComapreValues)(leftCow,leftValue,rightCow,rightValue);					
		}
		else
			return COMPARE_GREATHER;
	}
	else
	{
		if(rightCow->HasAnyBuyInvoice())	return COMPARE_LESS;
			
	}		
	return COMPARE_EQUAL;
}

template<typename U>
COMPARE_RESULT CowDataComparator::CompareSell(Cow *leftCow,Cow *rightCow,
										 U leftValue,U rightValue, 
										 COMPARE_RESULT (CowDataComparator::*fComapreValues)(Cow*,U,Cow*,U))
{				
	if(leftCow->IsOut())
	{
		if(rightCow->IsOut())
		{					
			if(leftCow->HasVATSellInvoice())
			{
				if(rightCow->HasVATSellInvoice())
				{
					return (this->*fComapreValues)(leftCow,leftValue,rightCow,rightValue);														
				}
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
		else
			return COMPARE_GREATHER;
	}
	else
	{
		if(rightCow->IsOut())
			return COMPARE_LESS;			
	}
	return COMPARE_EQUAL;
}

COMPARE_RESULT CowDataComparator::CompareCowWeight(Cow* leftCow,const NullDouble& leftValue,Cow* rightCow,const NullDouble& rightValue)
{
	NullDouble leftWeight = leftValue,rightWeight = rightValue;
	if(leftWeight.IsNull()) leftWeight = NullDouble(leftCow->GetWeight().GetDouble());
	if(rightWeight.IsNull()) rightWeight = NullDouble(rightCow->GetWeight().GetDouble());
	return CompareNullDouble(leftWeight,rightWeight);
}
COMPARE_RESULT CowDataComparator::CompareTermCowWeight(Cow* leftCow,const NullDouble& leftValue,Cow* rightCow,const NullDouble& rightValue)
{
	return CompareNullDouble(leftValue,rightValue);
}

COMPARE_RESULT CowDataComparator::CompareCowPrice(Cow* leftCow,const Money& leftValue,Cow* rightCow,const Money& rightValue)
{
	return CompareMoney(leftValue, rightValue);
}
COMPARE_RESULT CowDataComparator::CompareCowStock(Cow* leftCow,const Stock* leftValue,Cow* rightCow,const Stock* rightValue)
{	
	const Stock *leftStock = leftValue,*rightStock = rightValue;
	if(!leftStock) leftStock = leftCow->GetStock();
	if(!rightStock) rightStock = rightCow->GetStock();
	return PolishStringCompare(leftStock->GetStockCode(),rightStock->GetStockCode());	
}

COMPARE_RESULT CowDataComparator::CompareTermCowStock(Cow* leftCow,const Stock* leftValue,Cow* rightCow,const Stock* rightValue)
{
	const Stock *leftStock = leftValue,*rightStock = rightValue;
	if(leftStock)
	{
		if(rightStock)	return PolishStringCompare(leftStock->GetStockCode(),rightStock->GetStockCode());	
		else
			return COMPARE_GREATHER;
	}
	else
	{
		if(rightStock)
			return COMPARE_LESS;
	}
	 return COMPARE_EQUAL;	
}

COMPARE_RESULT CowDataComparator::CompareCowSex(const CowSex& leftCowSex,const CowSex& rightCowSex)
{
	if(leftCowSex > rightCowSex)
		return COMPARE_GREATHER;
	else
	{
		if(leftCowSex == rightCowSex)
			return COMPARE_EQUAL;
		else
			return COMPARE_LESS;
	}
}


COMPARE_RESULT CowDataComparator::CompareDataBy(UINT dataMember,Cow* DataLeft,Cow* DataRight)
{
	Cow *left = DataLeft,*right = DataRight;
	
	switch(dataMember)
	{
		default:
			ASSERT(0);
		case COWS_SORT_BY_STATE:

			if(left->IsOut())
			{
				if(right->IsOut())
					return COMPARE_EQUAL;
				else
					return COMPARE_GREATHER;
			}
            else
			{
				if(right->IsOut())
					 return COMPARE_LESS;
				else 
					return COMPARE_EQUAL;
			}
			
		case COWS_SORT_BY_EAN:
			return left->GetEAN().CompareNoCase(right->GetEAN());

		case COWS_SORT_BY_PASSNO:
			return left->GetPassNo().CompareNoCase(right->GetPassNo());

		case COWS_SORT_BY_DELIVER:
			return PolishStringCompare(left->GetDeliver()->GetAlias(),right->GetDeliver()->GetAlias());

		case COWS_SORT_BY_FSTOWNER:

			if(left->HasFirstOwner())
			{
				if(right->HasFirstOwner())
					return PolishStringCompare(left->GetFirstOwner()->GetAlias(),right->GetFirstOwner()->GetAlias());
				else
					return COMPARE_GREATHER;
			}
            else
			{
				if(right->HasFirstOwner())
					 return COMPARE_LESS;
				else 
					return COMPARE_EQUAL;
			}
		case COWS_SORT_BY_BIRTHDATE:
			return CompareDateTime(left->GetBirthDate(),right->GetBirthDate());

		case COWS_SORT_BY_WEIGHT:
			return CompareType<Weight>(left->GetWeight(),right->GetWeight());

		case COWS_SORT_BY_SEX:
			return CompareCowSex(left->GetSex(),right->GetSex());
		case COWS_SORT_BY_STOCK:
			return PolishStringCompare(left->GetStock()->GetStockCode(),right->GetStock()->GetStockCode());

		case COWS_SORT_BY_BUYPRICE:
			return CompareBuy<const Money&>(left,right,left->GetBuyPrice(),right->GetBuyPrice(),&CowDataComparator::CompareCowPrice);
		case COWS_SORT_BY_SELLPRICE:
			return CompareSell<const Money&>(left,right,left->GetSellPrice(),right->GetSellPrice(),&CowDataComparator::CompareCowPrice);
		case COWS_SORT_BY_OUTGRP:
			return CompareNullInt(left->GetOutGroup(),right->GetOutGroup());
		case COWS_SORT_BY_INGRP:
			return CompareNullInt(left->GetInGroup(),right->GetInGroup());
		case COWS_SORT_BY_MYPRICE:
			return CompareMoney(left->GetMyPrice() , right->GetMyPrice());
		case COWS_SORT_BY_INDOCNO:
			return CompareInt(left->GetInDoc()->GetId(),right->GetInDoc()->GetId());
		case COWS_SORT_BY_OUTDOCNO:
			if(left->IsOut())
			{
				if(right->IsOut())
				{					
					return CompareInt(left->GetOutDoc()->GetId(),right->GetOutDoc()->GetId());
				}
				else
					return COMPARE_GREATHER;
			}
            else
			{
				if(right->IsOut())
					 return COMPARE_LESS;
				else 
					return COMPARE_EQUAL;
			}
				
		case COWS_SORT_BY_EXTRAS:
			return PolishStringCompare(left->GetExtras(),right->GetExtras());
		case COWS_SORT_BY_INDATE:
			return CompareDateTime(left->GetInDoc()->GetLoadDate() , right->GetInDoc()->GetLoadDate());

		case COWS_SORT_BY_OUT_DATE:

			if(left->IsOut())
			{
				if(right->IsOut())
				{					
					return CompareDateTime(left->GetOutDoc()->GetDocDate() , right->GetOutDoc()->GetDocDate());
				}
				else
					return COMPARE_GREATHER;
			}
            else
			{
				if(right->IsOut())
					 return COMPARE_LESS;
				else 
					return COMPARE_EQUAL;
			}
					
		case COWS_SORT_BY_BUYER:
			if(left->IsOut())
			{
				if(right->IsOut())
				{					
					return PolishStringCompare(left->GetBuyer()->GetAlias(),right->GetBuyer()->GetAlias());
				}
				else
					return COMPARE_GREATHER;
			}
            else
			{
				if(right->IsOut())
					 return COMPARE_LESS;
				else 
					return COMPARE_EQUAL;
			}

		case COWS_SORT_BY_BUYSTOCK:
			return CompareBuy<const Stock*>(left,right,left->GetBuyStock(),right->GetBuyStock(),&CowDataComparator::CompareCowStock);		
		case COWS_SORT_BY_SELLSTOCK:
			return CompareSell<const Stock*>(left,right,left->GetSellStock(),right->GetSellStock(),&CowDataComparator::CompareCowStock);
		case COWS_SORT_BY_BUYWEIGHT:
			return CompareBuy<const NullDouble&>(left,right,left->GetBuyWeight(),right->GetBuyWeight(),&CowDataComparator::CompareCowWeight);
		case COWS_SORT_BY_SELLWEIGHT:
			return CompareSell<const NullDouble&>(left,right,left->GetSellWeight(),right->GetSellWeight(),&CowDataComparator::CompareCowWeight);
		case COWS_SORT_BY_TERMBUYWEIGHT:
			return CompareTermCowWeight(left,left->GetTermBuyWeight(),right,right->GetTermBuyWeight());
		case COWS_SORT_BY_TERMBUYPRICE:
			return CompareCowPrice(left,left->GetTermBuyPrice(),right,right->GetTermBuyPrice());
		case COWS_SORT_BY_TERMBUYSTOCK:
			return CompareTermCowStock(left,left->GetTermBuyStock(),right,right->GetTermBuyStock());
		case COWS_SORT_BY_TERMSELLSTOCK:
			return CompareTermCowStock(left,left->GetTermSellStock(),right,right->GetTermSellStock());			
		case COWS_SORT_BY_TERMSELLWEIGHT:
			return CompareTermCowWeight(left,left->GetTermSellWeight(),right,right->GetTermSellWeight());			
		case COWS_SORT_BY_TERMSELLPRICE:
			return CompareCowPrice(left,left->GetTermSellPrice(),right,right->GetTermSellPrice());
		case COWS_SORT_BY_RRINV:
			if(left->HasRRBuyInvoice())
			{
				if(right->HasRRBuyInvoice())
				{			
					RRInvoice *leftrrinv = left->GetRRInvoice();
					RRInvoice *rightrrinv = right->GetRRInvoice();
					if(leftrrinv->GetInvoiceNo() > rightrrinv->GetInvoiceNo())
						return COMPARE_GREATHER;
					else
					{
						if(leftrrinv->GetInvoiceNo() < rightrrinv->GetInvoiceNo())
							return COMPARE_LESS;

					}
					return COMPARE_EQUAL;
				}
				else
					return COMPARE_GREATHER;
			}
            else
			{
				if(right->HasRRBuyInvoice())
					 return COMPARE_LESS;
				else 
					return COMPARE_EQUAL;
			}
		case COWS_SORT_BY_INAGENT:
			{
				Agent *leftAgent = left->GetInDoc()->GetAgent(),*rightAgent = right->GetInDoc()->GetAgent();
				if(leftAgent)
				{
					if(rightAgent)
					{			
						return PolishStringCompare(leftAgent->GetAgentCode(),rightAgent->GetAgentCode());
					}
					else
						return COMPARE_GREATHER;
				}
				else
				{
					if(rightAgent)
						 return COMPARE_LESS;
					else 
						return COMPARE_EQUAL;
				}
			}


	}
	

	ASSERT(0);
	return COMPARE_LESS;
}
