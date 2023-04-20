#include "stdafx.h"
#include "PurchaseItem.h"


PurchaseItem::PurchaseItem(RepoPurchase* pPurchase) : m_pPurchase(pPurchase)
{
}

PurchaseItem::~PurchaseItem()
{
	delete m_pPurchase;
}

RepoPurchase* PurchaseItem::GetPurchase()
{
	return m_pPurchase;
}
	



