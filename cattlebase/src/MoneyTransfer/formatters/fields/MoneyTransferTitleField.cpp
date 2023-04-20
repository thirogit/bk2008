#include "stdafx.h"
#include "MoneyTransferTitleField.h"


CString MoneyTransferTitleField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();

	CString transferTitle("FAKTURA VAT ");

	if(pInvoice->GetRealInvoiceType() == IN_INV_RR)
		transferTitle += "RR ";

	if(pInvoice->GetCustomNumber().IsEmpty())
		transferTitle += pInvoice->GetInvoiceNoStr();
	else
		transferTitle += pInvoice->GetCustomNumber();

	transferTitle += " Z DNIA ";
	transferTitle += pInvoice->GetInvoiceDate().GetDateInFmt(CDateTime::YYYYdashMMdashDD);

	return transferTitle;
}