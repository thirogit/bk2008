#ifndef __EXCHANGERATE_H__
#define __EXCHANGERATE_H__

#include "../datatypes/Country.h"

class ExchangeRate
{
public:
	ExchangeRate();

	Country* GetCountry() const;
	void SetCountry(Country* pCountry);

	double GetExchangeRate() const;
	void SetExchangeRate(double dExchangeRate);

private:

	Country* m_pCountry;
	double m_dExchangeRate;

};


#endif