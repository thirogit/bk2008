#ifndef __COUNTRYCODEARRAY_H__
#define __COUNTRYCODEARRAY_H__

#include "../SortArray.h"
#include "Country.h"

class CCountryCodeArray : public CSortArray<Country>
{
public:
	Country *GetCountryByCountryNum(int countryISONumber);
	Country *GetCountryByCountryCode(const CString& countryCode2a);
};


#endif