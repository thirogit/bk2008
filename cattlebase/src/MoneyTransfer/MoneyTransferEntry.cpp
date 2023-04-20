#include "stdafx.h"
#include "MoneyTransferEntry.h"
#include "../classes/TempStringLoader.h"

MoneyTransferEntry::MoneyTransferEntry(Invoice* pInvoice,ExchangeRate* pExchangeRate) : 
	m_pInvoice(pInvoice),m_InvalidCause(VALID),m_pExchangeRate(pExchangeRate)
{
	CheckValid();
}

Hent* MoneyTransferEntry::GetHent() const
{
	return m_pInvoice->GetInvoiceHent();
}

CString MoneyTransferEntry::GetInvoiceNo() const
{
	return m_pInvoice->GetInvoiceNoStr();
}

CString MoneyTransferEntry::GetInvoiceCustomNumer() const
{
	return m_pInvoice->GetCustomNumber();
}

Money MoneyTransferEntry::GetGrossValue() const
{	
	return m_pInvoice->GetInvoiceGrossValue();
}

bool MoneyTransferEntry::IsInvalid() const
{
	return m_InvalidCause != VALID;
}

CString MoneyTransferEntry::GetWhyInvalid() const
{
	switch(m_InvalidCause)
	{
	case ALREADY_PAID:
		return CTempStringLoader(IDS_MONEYTRANS_ALRADYPAID);
	case HENT_HASNOACCOUNTNO:
		return CTempStringLoader(IDS_MONEYTRANS_HENTHASNOACCNO);
	case PAID_BY_CASH:
		return CTempStringLoader(IDS_MONEYTRANS_PAIDBYCASH);
	case HAS_CORRECT:
		return CTempStringLoader(IDS_MONEYTRANS_HASCORRECT);
	}
	return CString();
}

void MoneyTransferEntry::CheckValid()
{
	if(m_pInvoice->IsPaid())
	{
		m_InvalidCause = ALREADY_PAID;
	}
	else if(m_pInvoice->GetInvoiceHent()->GetAccountNo().IsEmpty())
	{
		m_InvalidCause = HENT_HASNOACCOUNTNO;
	} 
	else if(m_pInvoice->GetPayWay() == Invoice::PAY_CASH)
	{
		m_InvalidCause = PAID_BY_CASH;
	}
	else if(m_pInvoice->HasCorrect())
	{
		m_InvalidCause = HAS_CORRECT;
	}
	else
	{
		m_InvalidCause = VALID;
	}
}
Money MoneyTransferEntry::GetTransferValue() const
{
	return GetGrossValue() * m_pExchangeRate->GetExchangeRate();
}

CString MoneyTransferEntry::GetCurrencyCode() const
{
	return m_pExchangeRate->GetCountry()->GetCurrencyCode();
}

Invoice* MoneyTransferEntry::GetInvoice() const
{
	return m_pInvoice;
}

ExchangeRate* MoneyTransferEntry::GetExchangeRate() const
{
	return m_pExchangeRate;	
}