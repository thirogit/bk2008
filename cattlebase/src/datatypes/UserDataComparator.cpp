#include "stdafx.h"
#include "UserDataComparator.h"

COMPARE_RESULT UserDataComparator::CompareBy(UINT dataMember,UserData* DataLeft,UserData* DataRight)
{
	UserData *left = DataLeft,*right = DataRight;
	switch(dataMember)
	{
		default:
			ASSERT(0);
		case USERS_SORT_BY_USERNAME:
			return PolishStringCompare(left->username,right->username);
		case USERS_SORT_BY_FNAME:
			return PolishStringCompare(left->fname,right->fname);
		case USERS_SORT_BY_LNAME:
			return PolishStringCompare(left->lname,right->lname);
		case USERS_SORT_BY_LASTLOGIN:
			return CompareDateTime(left->lastlogin , right->lastlogin);
		case USERS_SORT_BY_PRIVS:
			if(left->privledges > right->privledges)
				return COMPARE_GREATHER;
			else
			{
				if(left->privledges < right->privledges)
					return COMPARE_LESS;

			}
			return COMPARE_EQUAL;
	}

	ASSERT(0);
	return COMPARE_LESS;
}
