#ifndef __MONEYTRANSFERENTRY_H__
#define __MONEYTRANSFERENTRY_H__

#include "../datatypes/Invoice.h"
#include "ExchangeRate.h"

class MoneyTransferEntry
{
	enum INVALID_CAUSE 
	{
		VALID,
		ALREADY_PAID,
		HENT_HASNOACCOUNTNO,
		PAID_BY_CASH,
		HAS_CORRECT,
	};

public:
	MoneyTransferEntry(Invoice* pInvoice,ExchangeRate* pExchangeRate);	
	
	Hent* GetHent() const;
	CString GetInvoiceNo() const;
	CString GetInvoiceCustomNumer() const;
	Money GetGrossValue() const;
	Money GetTransferValue() const;
	CString GetCurrencyCode() const;

	Invoice* GetInvoice() const;
	ExchangeRate* GetExchangeRate() const;

	
	bool IsInvalid() const;
	CString GetWhyInvalid() const;	
protected:
	void CheckValid();

	Money m_TransferValue;
	Invoice* m_pInvoice;
	INVALID_CAUSE m_InvalidCause;
	ExchangeRate* m_pExchangeRate;
};


#endif