#include "stdafx.h"
#include "CowAccessor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CowAccessor* CowAccessorFactory::createAccessor(BuyOrSell buyOrSell)
{
	if(buyOrSell == BUY)
		return new CowInAccessor();
	else
		return new CowOutAccessor();
}



Stock* CowInAccessor::GetStock(Cow* pCow)
{
	Stock *pStock = pCow->GetBuyStock();	
	return pStock;
}

Money CowInAccessor::GetPrice(Cow* pCow)
{
	return pCow->GetBuyPrice();
}

NullInt CowInAccessor::GetWeight(Cow* pCow)
{
	NullInt weight = pCow->GetBuyWeight();		
	return weight;
}


Stock* CowInAccessor::GetTermStock(Cow* pCow)
{
	return pCow->GetTermBuyStock();
}

Money CowInAccessor::GetTermPrice(Cow* pCow)
{
	return pCow->GetTermBuyPrice();
}

NullInt CowInAccessor::GetTermWeight(Cow* pCow)
{
	return pCow->GetTermBuyWeight();
}

Doc* CowInAccessor::GetDoc(Cow* pCow)
{
	return pCow->GetInDoc();
}

NullInt CowInAccessor::GetGroup(Cow* pCow)
{
	return pCow->GetInGroup();
}


Stock* CowOutAccessor::GetStock(Cow* pCow)
{
	Stock *pStock = pCow->GetSellStock();		
	return pStock;
}

Money CowOutAccessor::GetPrice(Cow* pCow)
{
	return pCow->GetSellPrice();
}

NullInt CowOutAccessor::GetWeight(Cow* pCow)
{
	NullInt weight = pCow->GetSellWeight(); 		
	return weight;
}


Stock* CowOutAccessor::GetTermStock(Cow* pCow)
{
	return pCow->GetTermSellStock();
}

Money CowOutAccessor::GetTermPrice(Cow* pCow)
{
	return pCow->GetTermSellPrice();
}

NullInt CowOutAccessor::GetTermWeight(Cow* pCow)
{
	return pCow->GetTermSellWeight();
}

Doc* CowOutAccessor::GetDoc(Cow* pCow)
{
	return pCow->GetOutDoc();
}

NullInt CowOutAccessor::GetGroup(Cow* pCow)
{
	return pCow->GetOutGroup();
}
