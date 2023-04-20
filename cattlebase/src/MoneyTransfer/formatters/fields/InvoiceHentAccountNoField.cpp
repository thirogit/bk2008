#include "stdafx.h"
#include "InvoiceHentAccountNoField.h"

CString InvoiceHentAccountNoField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();
	return pInvoice->GetInvoiceHent()->GetAccountNo();
}