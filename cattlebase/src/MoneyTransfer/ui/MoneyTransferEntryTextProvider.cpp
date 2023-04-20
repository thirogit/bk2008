#include "stdafx.h"
#include "MoneyTransferEntryTextProvider.h"
//#include "../consts.h"
#include "MoneyTransferEntryComparator.h"
//#include "../classes/IntString.h"
//#include "../datatypes/VATInvoiceData.h"


CString MoneyTransferEntryTextProvider::GetDataText(MoneyTransferEntry *pData,UINT memberID)
{
	CString retStr;
   	switch(memberID)
	{
		case MONEYTRANSFERENTRY_HENT:
			retStr = pData->GetHent()->GetComboItemText();
			break;
		case MONEYTRANSFERENTRY_INVOICENO:
			retStr = pData->GetInvoiceNo();
			break;
		case MONEYTRANSFERENTRY_INVOICECUSTNO:
			retStr = pData->GetInvoiceCustomNumer();
			break;
		case MONEYTRANSFERENTRY_INVOICEVALUE:
			retStr = pData->GetGrossValue().ToString() + " PLN";
			break;
		case MONEYTRANSFERENTRY_INVALIDCAUSE:
			retStr = pData->GetWhyInvalid();
			break;	
		case MONEYTRANSFERENTRY_TRANSFERVALUE:
			retStr = pData->GetTransferValue().ToString() + ' ' + pData->GetCurrencyCode();
			break;
		default:
			ASSERT(0);
			break;
	}	
	return retStr;
}
