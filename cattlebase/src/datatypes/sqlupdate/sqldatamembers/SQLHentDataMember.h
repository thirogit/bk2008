#ifndef __SQLHENTDATAMEMBER_H__
#define __SQLHENTDATAMEMBER_H__

#include "../../Cow.h"
#include "../SQLDataMember.h"

class SQLHentDataMember : public SQLDataMember<HentDataMember>
{
public:
	SQLHentDataMember(HentDataMember* pHentMember);
	virtual CString GetSQLStringValue(HentDataMember* pHentMember);	
};


#endif