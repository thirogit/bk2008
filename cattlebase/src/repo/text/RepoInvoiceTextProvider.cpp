#include "stdafx.h"
#include "../text/RepoInvoiceTextProvider.h"
#include "../compare/RepoInvoiceComparator.h"
#include "../../classes/IntString.h"
#include "utils/RepoInvoiceTypeString.h"
#include "utils/PayWayString.h"

CString RepoInvoiceTextProvider::GetDataText(RepoInvoice *pData,UINT memberId)
{
	CString sText;
   	switch(memberId)
	{
		case REPOINVOICE_MEMBER_INVOICEDATE:
			sText = pData->GetInvoiceDate().GetDayDate();
			break;

		case REPOINVOICE_MEMBER_TRANSACTIONDATE:
			sText = pData->GetTransactionDate().GetDayDate();
			break;

		case REPOINVOICE_MEMBER_TRANSACTIONPLACE:
			sText = pData->GetTransactionPlace();
			break;

		case REPOINVOICE_MEMBER_VATRATE:
			sText = pData->GetVatRate().To100String();
			break;

		case REPOINVOICE_MEMBER_PAYWAY:
			sText = PayWayString(pData->GetPayWay()).ToString();
			break;
		case REPOINVOICE_MEMBER_CUSTOMNUMBER:
			sText = pData->GetCustomNumber();
			break;
		case REPOINVOICE_MEMBER_PAYDUEDAYS:
			break;
		case REPOINVOICE_MEMBER_INVOICEHENTALIAS:
			sText = pData->GetInvoiceHent()->GetAlias();
			break;

		case REPOINVOICE_MEMBER_COWCOUNT:
			sText = (CString)CIntString(pData->GetCowCount());
			break;
		case REPOINVOICE_MEMBER_TOTALNET:
			sText = pData->GetTotalNet().ToString();
			break;
		case REPOINVOICE_MEMBER_INVOICETYPE:
			sText  = RepoInvoiceTypeString(pData->GetInvoiceType()).ToString();
			break;
		default:
			ASSERT(0);
			break;
	}	
	return sText;
}
