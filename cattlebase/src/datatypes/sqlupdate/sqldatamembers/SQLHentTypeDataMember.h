#ifndef __SQLHENTTYPEDATAMEMBER_H__
#define __SQLHENTTYPEDATAMEMBER_H__

#include "../../Hent.h"
#include "../SQLDataMember.h"

class SQLHentTypeDataMember : public SQLDataMember<HentTypeDataMember>
{
public:
	SQLHentTypeDataMember(HentTypeDataMember* pHentTypeMember);
	virtual CString GetSQLStringValue(HentTypeDataMember* pHentTypeMember);	
};


#endif