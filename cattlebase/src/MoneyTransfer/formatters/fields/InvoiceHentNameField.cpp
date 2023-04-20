#include "stdafx.h"
#include "InvoiceHentNameField.h"

CString InvoiceHentNameField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();
	Hent *pInvHent = pInvoice->GetInvoiceHent();
	return pInvHent->GetName();
}