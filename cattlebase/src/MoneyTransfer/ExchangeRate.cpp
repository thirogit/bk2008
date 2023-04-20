#include "stdafx.h"
#include "ExchangeRate.h"

ExchangeRate::ExchangeRate() : m_pCountry(NULL),m_dExchangeRate(1.0)
{

}

Country* ExchangeRate::GetCountry() const
{
	return m_pCountry;
}

void ExchangeRate::SetCountry(Country* pCountry)
{
	m_pCountry = pCountry;
}

double ExchangeRate::GetExchangeRate() const
{
	return m_dExchangeRate;
}

void ExchangeRate::SetExchangeRate(double dExchangeRate)
{
	m_dExchangeRate = dExchangeRate;
}



