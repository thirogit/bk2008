#include "stdafx.h"
#include "InvoiceDateField.h"


InvoiceDateField::InvoiceDateField(CDateTime::DateFormat dtFmt) : m_dtFmt(dtFmt)
{
}

CString InvoiceDateField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();
	return pInvoice->GetInvoiceDate().GetDateInFmt(m_dtFmt);
}