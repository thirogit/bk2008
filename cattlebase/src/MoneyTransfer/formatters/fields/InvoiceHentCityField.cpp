#include "stdafx.h"
#include "InvoiceHentCityField.h"


CString InvoiceHentCityField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	return pMoneyTransfer->GetHent()->GetCity();
}

