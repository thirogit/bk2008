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

NullDouble CowInAccessor::GetWeight(Cow* pCow)
{
	return pCow->GetBuyWeight();		
}


Stock* CowInAccessor::GetTermStock(Cow* pCow)
{
	return pCow->GetTermBuyStock();
}

Money CowInAccessor::GetTermPrice(Cow* pCow)
{
	return pCow->GetTermBuyPrice();
}

NullDouble CowInAccessor::GetTermWeight(Cow* pCow)
{
	return pCow->GetTermBuyWeight();
}

Doc* CowInAccessor::GetDoc(Cow* pCow)
{
	return pCow->GetInDoc();
}

NullInt CowInAccessor::GetGroup(const Cow* pCow)
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

NullDouble CowOutAccessor::GetWeight(Cow* pCow)
{
	return pCow->GetSellWeight(); 		
}


Stock* CowOutAccessor::GetTermStock(Cow* pCow)
{
	return pCow->GetTermSellStock();
}

Money CowOutAccessor::GetTermPrice(Cow* pCow)
{
	return pCow->GetTermSellPrice();
}

NullDouble CowOutAccessor::GetTermWeight(Cow* pCow)
{
	return pCow->GetTermSellWeight();
}

Doc* CowOutAccessor::GetDoc(Cow* pCow)
{
	return pCow->GetOutDoc();
}

NullInt CowOutAccessor::GetGroup(const Cow* pCow)
{
	return pCow->GetOutGroup();
}
