#include "stdafx.h"
#include "SQLCountryDataMember.h"

SQLCountryDataMember::SQLCountryDataMember(CountryDataMember* pCountryMember) :
		SQLDataMember(pCountryMember) 
{
}
 CString SQLCountryDataMember::GetSQLStringValue(CountryDataMember* pCountryMember)
 {
	Country* pCountry = pCountryMember->GetValue();
	if(pCountry)
	{
		return FormatInt(pCountry->GetId());
	}
	else
		return GetSQLNull();
	
}