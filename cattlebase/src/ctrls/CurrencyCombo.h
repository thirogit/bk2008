#ifndef __CURRENCYCOMBO_H__
#define __CURRENCYCOMBO_H__

#include "ComboBoxSuper.h"
#include "../consts.h"
#include "../datatypes/Country.h"
#include "../flagsrepository/FlagsRepository.h"

#define DEFAULTCOUNTRY ISO_POL_NUMBER 

class CCurrencyCombo : public CComboBoxSuper
{
public:
	CCurrencyCombo();
	void InitCurrencyCombo(UINT idCountry = DEFAULTCOUNTRY);
	Country* GetSelectedCountry();
	
protected:
	int m_CountryPresel;
	CFlagsRepository m_flagsRepo;

	DECLARE_MESSAGE_MAP();
};
#endif