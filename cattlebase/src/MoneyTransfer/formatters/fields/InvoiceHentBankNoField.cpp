#include "stdafx.h"
#include "InvoiceHentBankNoField.h"
#include "utils/AccountNoUtils.h"

CString InvoiceHentBankNoField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();
	return AccountNoUtils::GetBankNo(pInvoice->GetInvoiceHent()->GetAccountNo());
}