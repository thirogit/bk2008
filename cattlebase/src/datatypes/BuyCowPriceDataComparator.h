#ifndef __BUYCOWPRICEDATACOMPARATOR_H__
#define __BUYCOWPRICEDATACOMPARATOR_H__

#include "CowPriceDataComparator.h"

class BuyCowPriceDataComparator : public CowPriceDataComparator
{
public:
	BuyCowPriceDataComparator(UINT nDataMember,bool bAsc);
protected:
	virtual COMPARE_RESULT CompareOtherPrice(CowPriceData* DataLeft,CowPriceData* DataRight);
	virtual COMPARE_RESULT CompareHent(CowPriceData* DataLeft,CowPriceData* DataRight);
	virtual COMPARE_RESULT CompareHasTerm(CowPriceData* DataLeft,CowPriceData* DataRight);
};


#endif