#include "stdafx.h"
#include "DocDataComparator.h"
#include "VATInvoice.h"

COMPARE_RESULT DocDataComparator::CompareDataBy(UINT dataMember,Doc* DataLeft,Doc* DataRight)
{
	Doc *left = DataLeft,*right = DataRight;
	
	switch(dataMember)
	{
		default:
			ASSERT(0);

		case DOCS_SORT_BY_NUMBER:			
			if(left->GetId() > right->GetId())
				return COMPARE_GREATHER;
			else
			{
				if(left->GetId() < right->GetId())
					return COMPARE_LESS;

			}
			return COMPARE_EQUAL;

		case DOCS_SORT_BY_DOCDATE:
			return CompareDateTime(left->GetDocDate() , right->GetDocDate());
		case DOCS_SORT_BY_LOADDATE:
			return CompareDateTime(left->GetLoadDate() , right->GetLoadDate());
		case DOCS_SORT_BY_HENT:
			return PolishStringCompare(left->GetHent()->GetComboItemText() , right->GetHent()->GetComboItemText());
		case DOCS_SORT_BY_PLATENO:
			return PolishStringCompare(left->GetPlateNo(),right->GetPlateNo()) ;
		case DOCS_SORT_BY_COWCOUNT:
			if(left->GetCowCount() > right->GetCowCount())
				return COMPARE_GREATHER;
			else
			{
				if(left->GetCowCount() < right->GetCowCount())
					return COMPARE_LESS;

			}
			return COMPARE_EQUAL;
		case DOCS_SORT_BY_INVOICE:
			if(left->HasInvoice())
			{
				if(right->HasInvoice())
				{	
					if(left->GetInvoice()->GetInvoiceNo() > right->GetInvoice()->GetInvoiceNo())
						return COMPARE_GREATHER;
					else
					{
						if(left->GetInvoice()->GetInvoiceNo() < right->GetInvoice()->GetInvoiceNo())
							return COMPARE_LESS;

					}
					return COMPARE_EQUAL;
				}
				else
					return COMPARE_GREATHER;
			}
            else
			{
				if(right->HasInvoice())
					 return COMPARE_LESS;
				else 
					return COMPARE_EQUAL;
			}

			
		case DOCS_SORT_BY_EXTRAS:
			return PolishStringCompare(left->GetExtras(),right->GetExtras()) ;
		case DOCS_MEMBER_AGENT:
			if(left->GetAgent())
			{
				if(right->GetAgent())
				{	
					return PolishStringCompare(left->GetAgent()->GetAgentCode(),right->GetAgent()->GetAgentCode());						
				}
				else
					return COMPARE_GREATHER;
			}
            else
			{
				if(right->GetAgent())
					 return COMPARE_LESS;
				else 
					return COMPARE_EQUAL;
			}
		
	}



	ASSERT(0);
	return COMPARE_LESS;
}
