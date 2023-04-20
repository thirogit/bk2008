#include "stdafx.h"
#include "InvoiceHentZipField.h"


CString InvoiceHentZipField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Hent* pHent = pMoneyTransfer->GetHent();
	return pHent->GetZip();
}

