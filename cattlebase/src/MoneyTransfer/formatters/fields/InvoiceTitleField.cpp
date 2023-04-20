#include "stdafx.h"
#include "InvoiceTitleField.h"


CString InvoiceTitleField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();

	CString invoiceTitle("FAKTURA VAT ");

	if(pInvoice->GetRealInvoiceType() == IN_INV_RR)
		invoiceTitle += "RR";

	return invoiceTitle;
}