#ifndef __REPOPURCHASECOMPARATOR_H__
#define __REPOPURCHASECOMPARATOR_H__

#include "../RepoPurchase.h"
#include "DataComparator.h"

enum REPOPURCHASE_MEMBERS
{
	REPOPURCHASE_MEMBER_AGENTCODE  = 1,	
	REPOPURCHASE_MEMBER_PLATENO,
    REPOPURCHASE_MEMBER_STARTDATE,
    REPOPURCHASE_MEMBER_ENDDATE,
	REPOPURCHASE_MEMBER_COWCOUNT,
	REPOPURCHASE_MEMBER_WASDOWNLOADED,
	
   	REPOPURCHASE_MEMBER_MAX
};


class RepoPurchaseComparator : public DataComparator<RepoPurchase>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,RepoPurchase* DataLeft,RepoPurchase* DataRight);
};


#endif