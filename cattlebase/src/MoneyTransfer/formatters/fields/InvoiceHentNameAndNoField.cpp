#include "stdafx.h"
#include "InvoiceHentNameAndNoField.h"

CString InvoiceHentNameAndNoField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();
	Hent *pInvHent = pInvoice->GetInvoiceHent();
	return pInvHent->GetName() + '(' + pInvHent->GetFarmNo() + ')';
}