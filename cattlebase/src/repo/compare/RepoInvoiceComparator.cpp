#include "stdafx.h"
#include "RepoInvoiceComparator.h"

COMPARE_RESULT RepoInvoiceComparator::CompareBy(UINT dataMember,RepoInvoice* DataLeft,RepoInvoice* DataRight)
{
	RepoInvoice *left = DataLeft,*right = DataRight;
	
	switch(dataMember)
	{
		default:
			ASSERT(0);			
		case REPOINVOICE_MEMBER_INVOICEDATE:
			return CompareDateTime(left->GetInvoiceDate(),right->GetInvoiceDate());
		case REPOINVOICE_MEMBER_TRANSACTIONDATE:
			return CompareDateTime(left->GetTransactionDate(),right->GetTransactionDate());
		case REPOINVOICE_MEMBER_TRANSACTIONPLACE:
			return PolishStringCompare(left->GetTransactionPlace(),right->GetTransactionPlace());
		case REPOINVOICE_MEMBER_VATRATE:
			return CompareType<VATRate>(left->GetVatRate(),right->GetVatRate());
		case REPOINVOICE_MEMBER_PAYWAY:
			return CompareType<Invoice::PAYWAYS>(left->GetPayWay(),right->GetPayWay());
		case REPOINVOICE_MEMBER_CUSTOMNUMBER:
			return PolishStringCompare(left->GetCustomNumber(),right->GetCustomNumber());
		case REPOINVOICE_MEMBER_PAYDUEDAYS:
			return CompareInt(left->GetPayDueDays(),right->GetPayDueDays());
		case REPOINVOICE_MEMBER_INVOICEHENTALIAS:
			return PolishStringCompare(left->GetInvoiceHent()->GetAlias(),right->GetInvoiceHent()->GetAlias());
		case REPOINVOICE_MEMBER_COWCOUNT:
			return CompareInt(left->GetCowCount(),right->GetCowCount());
		case REPOINVOICE_MEMBER_TOTALNET:
			return CompareMoney(left->GetTotalNet(),right->GetTotalNet());
		case REPOINVOICE_MEMBER_INVOICETYPE:
			return CompareType<RepoInvoiceType>(right->GetInvoiceType(),left->GetInvoiceType());
		
	}
	ASSERT(0);
	return COMPARE_LESS;
}
