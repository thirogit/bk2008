#include "stdafx.h"
#include "InvoiceHentAddrField.h"


CString InvoiceHentAddrField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	CString sHentAddr;
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();
	Hent *pInvHent = pInvoice->GetInvoiceHent();
	sHentAddr = pInvHent->GetStreet() + ' ' + pInvHent->GetPOBox() + ", " + pInvHent->GetCity() + ' ' + pInvHent->GetZip();
	return sHentAddr;
}

