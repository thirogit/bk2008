#include "stdafx.h"
#include "CountryCodeArray.h"
#include "../missing/util.h"

Country *CCountryCodeArray::GetCountryByCountryNum(int countryISONumber)
{
	Country *pCountry;
	for(int i = 0;i< GetSize();i++)
	{
		pCountry = (Country*)GetAt(i); 
        if(pCountry->GetCountryISONumber() == countryISONumber) return pCountry;
	}
	return NULL;
	
}

Country *CCountryCodeArray::GetCountryByCountryCode(const CString& countryCode2a)
{
	if(countryCode2a.GetLength() < 2) return NULL;
	
	CString countryCode = countryCode2a.Left(2);
	if(!CUtil::IsAlpha(countryCode))
		return false;

	Country *pCountry;
	for(int i = 0;i< GetSize();i++)
	{
		pCountry = (Country*)GetAt(i); 
		if(pCountry->GetCountryCode2A() == countryCode) 
			return pCountry;
	}
	return NULL;
}