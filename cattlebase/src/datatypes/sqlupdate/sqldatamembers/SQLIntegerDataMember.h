#ifndef __SQLINTEGERDATAMEMBER_H__
#define __SQLINTEGERDATAMEMBER_H__

#include "../SQLDataMember.h"
#include "../../DataMembersDefs.h"

class SQLIntegerDataMember : public SQLDataMember<IntegerDataMember>
{
public:
	SQLIntegerDataMember(IntegerDataMember* pIntegerMember);
	virtual CString GetSQLStringValue(IntegerDataMember* pIntegerMember);	
};


#endif