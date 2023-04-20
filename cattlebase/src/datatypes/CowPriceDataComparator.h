#ifndef __COWPRICEDATACOMPARATOR_H__
#define __COWPRICEDATACOMPARATOR_H__


#include "CowPriceData.h"
#include "DataComparator.h"

enum COWPRICEDATA_COMPARE
{
	COWPRICEDATA_HASTERM,
	COWPRICEDATA_EAN,
	COWPRICEDATA_STOCK,
	COWPRICEDATA_WEIGHT,
	COWPRICEDATA_MYPRICE,
	COWPRICEDATA_HENT,
	COWPRICEDATA_OTHERPRICE,
	COWPRICEDATA_RULE,
	COWPRICEDATA_PRICEPERKG,
	COWPRICEDATA_NETTO,
	COWPRICEDATA_VAT,
	COWPRICEDATA_BRUTTO
};


class CowPriceDataComparator : public DataComparator<CowPriceData>, public CSortArray<CowPriceData>::SortArrayComparator
{
public:
	CowPriceDataComparator(UINT nDataMember,bool bAsc);
	virtual COMPARE_RESULT Compare(CowPriceData *DataLeft,CowPriceData *DataRight);
protected:
	COMPARE_RESULT CompareBy(UINT dataMember,CowPriceData* DataLeft,CowPriceData* DataRight);
protected:
	COMPARE_RESULT CompareCommon(UINT dataMember,CowPriceData* DataLeft,CowPriceData* DataRight);
	virtual COMPARE_RESULT CompareOtherPrice(CowPriceData* DataLeft,CowPriceData* DataRight) = 0;
	virtual COMPARE_RESULT CompareHent(CowPriceData* DataLeft,CowPriceData* DataRight) = 0;
	virtual COMPARE_RESULT CompareHasTerm(CowPriceData* DataLeft,CowPriceData* DataRight) = 0;
	COMPARE_RESULT CompareHasTerm(BuyOrSell buyOrSell,CowPriceData* DataLeft,CowPriceData* DataRight);
private:
	UINT m_nDataMember;
};


#endif