#ifndef __USERDATACOMPARATOR_H__
#define __USERDATACOMPARATOR_H__

#include "RawData.h"
#include "UserData.h"
#include "DataComparator.h"

enum USERDATA_MEMBERS
{
	USERS_SORT_BY_USERNAME = RAWDATA_MEMBER_BASE+1,
	USERS_SORT_BY_FNAME,
	USERS_SORT_BY_LNAME,
	USERS_SORT_BY_LASTLOGIN,
	USERS_SORT_BY_PRIVS
};



class UserDataComparator : public DataComparator<UserData>
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,UserData* DataLeft,UserData* DataRight);
};


#endif