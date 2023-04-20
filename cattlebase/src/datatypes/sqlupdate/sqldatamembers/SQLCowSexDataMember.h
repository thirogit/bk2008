#ifndef __SQLCOWSEXDATAMEMBER_H__
#define __SQLCOWSEXDATAMEMBER_H__

#include "../../Cow.h"
#include "../SQLDataMember.h"

class SQLCowSexDataMember : public SQLDataMember<CowSexDataMember>
{
public:
	SQLCowSexDataMember(CowSexDataMember* pCowSexMember);
	virtual CString GetSQLStringValue(CowSexDataMember* pCowSexMember);	
};


#endif