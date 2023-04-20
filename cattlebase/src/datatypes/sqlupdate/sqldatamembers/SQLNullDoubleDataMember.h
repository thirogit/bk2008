#ifndef __SQLNULLDOUBLEDATAMEMBER_H__
#define __SQLNULLDOUBLEDATAMEMBER_H__

#include "../../DataMembersDefs.h"
#include "../SQLDataMember.h"

template<class NULLDOUBLEDATAMEMBER,int floatPrecision = MAX_FLOAT_PREC>
class SQLNullDoubleDataMember : public SQLDataMember<NULLDOUBLEDATAMEMBER>
{
public:
	SQLNullDoubleDataMember(NULLDOUBLEDATAMEMBER* pNullDoubleMember,int precision = floatPrecision) :
		SQLDataMember(pNullDoubleMember),m_precision(precision)
	{
	}
	virtual CString GetSQLStringValue(NULLDOUBLEDATAMEMBER* pNullDoubleMember)
	{		
		if(!pNullDoubleMember->GetValue().IsNull())
		{
			return FormatFloat(pNullDoubleMember->GetValue().GetDouble(),m_precision);
		}
		else
			return GetSQLNull();
	}
private:
	int m_precision;
};

#endif