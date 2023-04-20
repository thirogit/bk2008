#ifndef __PURCHASEITEM_H__
#define __PURCHASEITEM_H__

#include "UpdatableItem.h"
#include "RepoPurchase.h"

class PurchaseItem :  public UpdatableItem
{
public:
	PurchaseItem(RepoPurchase* pPurchase);
	~PurchaseItem();
	RepoPurchase* GetPurchase();
private:
	RepoPurchase* m_pPurchase;
};

#endif