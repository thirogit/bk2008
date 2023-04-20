#include "stdafx.h"
#include "InvoiceDataComparator.h"

COMPARE_RESULT InvoiceDataComparator::CompareDataBy(UINT dataMember,Invoice* DataLeft,Invoice* DataRight)
{
	Invoice *left = DataLeft,*right = DataRight;
	
	switch(dataMember)
	{
		case INVS_SORT_BY_HENT:
			return PolishStringCompare(left->GetInvoiceHent()->GetComboItemText(),right->GetInvoiceHent()->GetComboItemText()) ;
		case INVS_SORT_BY_QTY:
			if(left->GetCowQty() > right->GetCowQty())
				return COMPARE_GREATHER;
			else
			{
				if(left->GetCowQty() < right->GetCowQty())
					return COMPARE_LESS;

			}
			return COMPARE_EQUAL;
			
		case INVS_SORT_BY_NUMBER:
			if(left->GetInvoiceNo() > right->GetInvoiceNo())
				return COMPARE_GREATHER;
			else
			{
				if(left->GetInvoiceNo() < right->GetInvoiceNo())
					return COMPARE_LESS;

			}
			return COMPARE_EQUAL;
		case INVS_SORT_BY_CUSTOMNO:
			return PolishStringCompare(left->GetCustomNumber(),right->GetCustomNumber());
		case INVS_SORT_BY_INVDATE:
			return CompareDateTime(left->GetInvoiceDate() , right->GetInvoiceDate());
		case INVS_SORT_BY_CORRECT:
			if(left->HasCorrect())
				{
					if(right->HasCorrect())
					{					
						if(left->GetCorrect()->GetInvoiceNo() > right->GetCorrect()->GetInvoiceNo())
							return COMPARE_GREATHER;
						else
						{
							if(left->GetCorrect()->GetInvoiceNo() < right->GetCorrect()->GetInvoiceNo())
								return COMPARE_LESS;

						}
						return COMPARE_EQUAL;
					}
					else
						return COMPARE_GREATHER;
				}
				else
				{
					if(right->HasCorrect())
						 return COMPARE_LESS;
					else 
						return COMPARE_EQUAL;
				}
		case INVS_SORT_BY_VAT:
			if(left->GetVATRate() > right->GetVATRate())
				return COMPARE_GREATHER;
			else
			{
				if(left->GetVATRate() < right->GetVATRate())
					return COMPARE_LESS;

			}
			return COMPARE_EQUAL;

			
		case INVS_SORT_BY_PAYWAY:
			if(left->GetPayWay() > right->GetPayWay())
				return COMPARE_GREATHER;
			else
			{
				if(left->GetPayWay() < right->GetPayWay())
					return COMPARE_LESS;

			}
			return COMPARE_EQUAL;
		case INVS_SORT_BY_PAID:
			return CompareDateTime(left->GetPaidDate() , right->GetPaidDate());
		case INVS_SORT_BY_GROSSVALUEAFTERCORRECT:
			if(left->HasCorrect()) return false;
			if(right->HasCorrect()) return true;
			return (left->GetBruttoValueAfterCorrect() > right->GetBruttoValueAfterCorrect());

		case INVS_SORT_BY_NETVALUE:
			return CompareMoney(left->GetInvoiceNetValue() , right->GetInvoiceNetValue());
		case INVS_SORT_BY_GROSSVALUE:
			return CompareMoney(left->GetInvoiceGrossValue() , right->GetInvoiceGrossValue());
		case INVS_SORT_BY_EXTRAS:
			return PolishStringCompare(left->GetExtras(),right->GetExtras());
		case INVS_SORT_BY_PAYDUEDAYS:
			return CompareInt(left->GetPayDueDays(),right->GetPayDueDays());
		case INVS_SORT_BY_PAYDUEDAYSDATE:
			return CompareDateTime(left->GetPayDueDate(),right->GetPayDueDate());			
		default:
			ASSERT(0);
	}

	ASSERT(0);
	return COMPARE_LESS;
}
