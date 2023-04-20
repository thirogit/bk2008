#ifndef __SQLCOUNTRYDATAMEMBER_H__
#define __SQLCOUNTRYDATAMEMBER_H__

#include "../../Hent.h"
#include "../SQLDataMember.h"

class SQLCountryDataMember : public SQLDataMember<CountryDataMember>
{
public:
	SQLCountryDataMember(CountryDataMember* pCountryMember);
	virtual CString GetSQLStringValue(CountryDataMember* pCountryMember);	
};


#endif