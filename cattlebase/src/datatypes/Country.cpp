#include "stdafx.h"
#include "Country.h"

Country::Country(UINT Id) : isoNumber(Id),inEU(false)
{	
}
	
UINT	Country::GetId() const
{
	return GetCountryISONumber();
}

UINT	Country::GetCountryISONumber() const
{
	return isoNumber;
}

bool	Country::IsCountryInEU() const
{
	return inEU;
}

CString Country::GetCountryName() const
{
	return countryName;
}

CString Country::GetCountryCode2A() const
{
	return code2a;
}

CString Country::GetCountryCode3A() const
{
	return code3a;
}

void	Country::SetCountryInEU(bool bInEU)
{
	inEU = bInEU;
}

void	Country::SetCountryName(const CString& sCountryName)
{
	countryName = sCountryName;
}

void	Country::SetCountryCode2A(const CString& sCountryCode2a)
{
	code2a = sCountryCode2a;
}

void	Country::SetCountryCode3A(const CString& sCountryCode3a)
{
	code3a = sCountryCode3a;
}

CString Country::GetCurrencyCode() const
{
	return currencyCode;
}

void	Country::SetCurrencyCode(const CString& sCurrencyCode)
{
	currencyCode = sCurrencyCode;
}