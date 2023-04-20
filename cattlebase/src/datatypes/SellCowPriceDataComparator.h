#ifndef __SELLCOWPRICEDATACOMPARATOR_H__
#define __SELLCOWPRICEDATACOMPARATOR_H__

#include "CowPriceDataComparator.h"

class SellCowPriceDataComparator : public CowPriceDataComparator
{
public:
	SellCowPriceDataComparator(UINT nDataMember,bool bAsc);
protected:
	virtual COMPARE_RESULT CompareOtherPrice(CowPriceData* DataLeft,CowPriceData* DataRight);
	virtual COMPARE_RESULT CompareHent(CowPriceData* DataLeft,CowPriceData* DataRight);
	virtual COMPARE_RESULT CompareHasTerm(CowPriceData* DataLeft,CowPriceData* DataRight);
};


#endif