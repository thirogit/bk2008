#ifndef __COUNTRYCOMBO_H__
#define __COUNTRYCOMBO_H__

#include "ComboBoxSuper.h"
#include "../consts.h"
#include "../datatypes/Country.h"
#include "../flagsrepository/FlagsRepository.h"


#define DEFAULTCOUNTRY ISO_POL_NUMBER 

class CCountryCombo : public CComboBoxSuper
{
public:
	CCountryCombo();
	void InitCountryCombo(int iCountryPresel = DEFAULTCOUNTRY);
	Country* GetSelectedCountry();
	
protected:
	int m_CountryPresel;
	CFlagsRepository m_flagsRepo;

	DECLARE_MESSAGE_MAP();
};
#endif