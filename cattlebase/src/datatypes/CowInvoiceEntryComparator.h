#ifndef __COWINVOICEENTRYCOMPARATOR_H__
#define __COWINVOICEENTRYCOMPARATOR_H__

#include "CowInvoiceEntry.h"
#include "../accessor/CowAccessor.h"
#include "../DataComparator.h"

enum COWINVENTRY_SORTBY
{
	COWINVENTRY_SORTBY_DOC = -1,
	COWINVENTRY_SORTBY_EAN = -2,
	COWINVENTRY_SORTBY_STOCK = -3,
	COWINVENTRY_SORTBY_WEIGHT = -4,
	COWINVENTRY_SORTBY_MYPRICE = -5,
	COWINVENTRY_SORTBY_NETTOPRICE = -6,
	COWINVENTRY_SORTBY_VATVALUE = -7,
	COWINVENTRY_SORTBY_BRUTTOPRICE = -8,
	COWINVENTRY_SORTBY_ALIAS = -9,
	COWINVENTRY_SORTBY_INV_WEIGHT = -10,
	COWINVENTRY_SORTBY_INV_STOCK = -11,
	COWINVENTRY_SORTBY_TERM_WEIGHT = -12,
	COWINVENTRY_SORTBY_TERM_STOCK = -13,
	COWINVENTRY_SORTBY_TERM_PRICE = -14
};

class CCowInvoiceEntryComparator : public DataComparator<CowInvoiceEntry>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,CowInvoiceEntry* DataLeft,CowInvoiceEntry* DataRight,CowAccessor *pAccessor);
private:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,CowInvoiceEntry* DataLeft,CowInvoiceEntry* DataRight);
};

#endif