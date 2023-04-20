#ifndef __SQLBOOLEANDATAMEMBER_H__
#define __SQLBOOLEANDATAMEMBER_H__

#include "../../Hent.h"
#include "../SQLDataMember.h"

class SQLBooleanDataMember : public SQLDataMember<BooleanDataMember>
{
public:
	SQLBooleanDataMember(BooleanDataMember* pBooleanMember);
	virtual CString GetSQLStringValue(BooleanDataMember* pBooleanMember);	
};


#endif