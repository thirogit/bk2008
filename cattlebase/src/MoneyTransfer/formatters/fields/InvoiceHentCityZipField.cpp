#include "stdafx.h"
#include "InvoiceHentCityZipField.h"


CString InvoiceHentCityZipField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Hent* hent = pMoneyTransfer->GetHent();
	return hent->GetCity() + ' ' + hent->GetZip();
}

