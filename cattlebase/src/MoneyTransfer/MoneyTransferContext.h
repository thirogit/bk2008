#ifndef __MONEYTRANSFERCONTEXT_H__
#define __MONEYTRANSFERCONTEXT_H__

#include "ExchangeRate.h"
#include "../classes/types/DateTime.h"

class MoneyTransferContext
{
public:
	MoneyTransferContext();

	ExchangeRate* GetExchangeRate() const;
	void SetExchangeRate(ExchangeRate* pExchangeRate);

	CDateTime GetTransferDate() const;
	void SetTransferDate(const CDateTime& dtTransfer);

private:
	ExchangeRate* m_pExchangeRate;
	CDateTime     m_dtTransfer;

};


#endif