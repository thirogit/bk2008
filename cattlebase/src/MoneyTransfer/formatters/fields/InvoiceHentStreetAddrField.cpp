#include "stdafx.h"
#include "InvoiceHentStreetAddrField.h"


CString InvoiceHentStreetAddrField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Hent* pHent = pMoneyTransfer->GetHent();
	return pHent->GetStreet() + ' ' + pHent->GetPOBox();
}

