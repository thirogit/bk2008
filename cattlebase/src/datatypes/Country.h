#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#define NULL_COUNTRYID (0)

class Country
{
public:
	Country(UINT Id);
	
	UINT	GetId() const;
	UINT	GetCountryISONumber() const;
	bool	IsCountryInEU() const;
	CString GetCountryName() const;
	CString GetCountryCode2A() const;
	CString GetCountryCode3A() const;
	CString GetCurrencyCode() const;
	void	SetCountryInEU(bool bInEU);
	void	SetCountryName(const CString& sCountryName);
	void	SetCountryCode2A(const CString& sCountryCode2a);
	void	SetCountryCode3A(const CString& sCountryCode3a);
	void	SetCurrencyCode(const CString& sCurrencyCode);
private:
	CString countryName;
	CString code2a;
	CString code3a;
	UINT	isoNumber;
	bool	inEU;
	CString currencyCode;
};


#endif