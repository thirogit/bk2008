#ifndef __REPOCOWCOMPARATOR_H__
#define __REPOCOWCOMPARATOR_H__

#include "../RepoCow.h"
#include "DataComparator.h"

enum REPOCOW_MEMBERS
{
	REPOCOW_MEMBER_COWNO  = 1,	
	REPOCOW_MEMBER_COWSEX,
    REPOCOW_MEMBER_STOCK,
    REPOCOW_MEMBER_WEIGHT,
	REPOCOW_MEMBER_PRICE,

    REPOCOW_MEMBER_MAX
};

class RepoCowComparator : public DataComparator<RepoCow>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,RepoCow* DataLeft,RepoCow* DataRight);
};


#endif