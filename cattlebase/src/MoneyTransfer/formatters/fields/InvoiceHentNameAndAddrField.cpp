#include "stdafx.h"
#include "InvoiceHentNameAndAddrField.h"


CString InvoiceHentNameAndAddrField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	CString sHentNameAndAddr;
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();
	Hent *pInvHent = pInvoice->GetInvoiceHent();
	sHentNameAndAddr = pInvHent->GetName() + " (" + pInvHent->GetFarmNo() + ") " + pInvHent->GetStreet() + ' ' + pInvHent->GetPOBox() + ", " + pInvHent->GetCity() + ' ' + pInvHent->GetZip();
	return sHentNameAndAddr;
}

