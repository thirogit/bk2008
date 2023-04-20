#include "stdafx.h"
#include "CurrencyCodeField.h"

CString CurrencyCodeField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	return pMoneyTransfer->GetExchangeRate()->GetCountry()->GetCurrencyCode();
}