#include "stdafx.h"
#include "MoneyTransferContext.h"


MoneyTransferContext::MoneyTransferContext() : m_pExchangeRate(NULL)
{

}

ExchangeRate* MoneyTransferContext::GetExchangeRate() const
{
	return m_pExchangeRate;
}

void MoneyTransferContext::SetExchangeRate(ExchangeRate* pExchangeRate)
{
	m_pExchangeRate = pExchangeRate;
}


CDateTime MoneyTransferContext::GetTransferDate() const
{
	return m_dtTransfer;
}

void MoneyTransferContext::SetTransferDate(const CDateTime& dtTransfer)
{
	m_dtTransfer = dtTransfer;
}

