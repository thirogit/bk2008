#include "stdafx.h"
#include "InvoiceIdField.h"
#include "../../../classes/IntString.h"


CString InvoiceIdField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();

	
	return CIntString(pInvoice->GetId());
}