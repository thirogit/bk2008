#ifndef __SQLNULLINTDATAMEMBER_H__
#define __SQLNULLINTDATAMEMBER_H__

#include "../../DataMembersDefs.h"
#include "../SQLDataMember.h"

template<class NULLINTDATAMEMBER>
class SQLNullIntDataMember : public SQLDataMember<NULLINTDATAMEMBER>
{
public:
	SQLNullIntDataMember(NULLINTDATAMEMBER* pNullIntMember) : SQLDataMember(pNullIntMember) 
	{
	}
	virtual CString GetSQLStringValue(NULLINTDATAMEMBER* pNullIntMember)
	{
			NullInt nullInt = pNullIntMember->GetValue();
		if(!nullInt.IsNull())
		{
			return FormatInt(nullInt.ToInt());
		}
		else
			return GetSQLNull();
	}
};

typedef SQLNullIntDataMember<NullIntDataMember> SQLRawNullIntDataMember;


#endif