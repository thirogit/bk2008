#ifndef __HERDCOMPARATOR_H__
#define __HERDCOMPARATOR_H__

#include "../DataComparator.h"
#include "Herd.h"

enum HERD_MEMBERS
{
	HERD_MEMBER_NAME,
	HERD_MEMBER_CODE,
	HERD_MEMBER_HERDNO,
	HERD_MEMBER_STREET,
	HERD_MEMBER_POBOX,
	HERD_MEMBER_CITY,
	HERD_MEMBER_ZIP,
	HERD_MEMBER_LATITUDE,
	HERD_MEMBER_LONGITUDE,
    HERD_MEMBER_MAX
};


class HerdComparator : public DataComparator<Herd>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,Herd* DataLeft,Herd* DataRight);
	
};


#endif