#include "stdafx.h"
#include "InvoiceNumberField.h"


CString InvoiceNumberField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();

	
	if(pInvoice->GetCustomNumber().IsEmpty())
		return pInvoice->GetInvoiceNoStr();
	else
		return pInvoice->GetCustomNumber();

	
}