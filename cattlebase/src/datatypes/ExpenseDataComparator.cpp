#include "stdafx.h"
#include "ExpenseDataComparator.h"

COMPARE_RESULT ExpenseDataComparator::CompareDataBy(UINT dataMember,Expense* DataLeft,Expense* DataRight)
{
	Expense *left = DataLeft,*right = DataRight;
	
	switch(dataMember)
	{
		default:
			ASSERT(0);
		case EXPENSEDATA_VATVALUE:
			return CompareMoney(left->GetGrosValue()*left->GetVATRate(),
								right->GetVATRate()*right->GetGrosValue());
		case EXPENSEDATA_NETVALUE:		
		case EXPENSEDATA_GROSVALUE:
			return CompareMoney(left->GetGrosValue(),right->GetGrosValue());
		case EXPENSEDATA_VATRATE:
			return CompareType<double>(left->GetVATRate(),right->GetVATRate());
		case EXPENSEDATA_EXPENSETYPE:
			if(left->GetExpenseType() > right->GetExpenseType())
			{
				return COMPARE_GREATHER;
			}
			else
			{
				if(left->GetExpenseType() < right->GetExpenseType())
					return COMPARE_LESS;
			}
			return COMPARE_EQUAL;
		case EXPENSEDATA_EXPENSECATEGORY:
			return PolishStringCompare(left->GetExpenseCategory()->GetExpenseCategoryName(),
												right->GetExpenseCategory()->GetExpenseCategoryName());
		case EXPENSEDATA_INVOICENO:
			return PolishStringCompare(left->GetInvoiceNo(),right->GetInvoiceNo());
		case EXPENSEDATA_EXPENSEDATE:
			return CompareDateTime(left->GetExpenseDate(),right->GetExpenseDate());
		case EXPENSEDATA_EXTRAS:
			return PolishStringCompare(left->GetExtras(),right->GetExtras());
		case EXPENSEDATA_EXPENSETITLE:
			return PolishStringCompare(left->GetExpenseTitle(),right->GetExpenseTitle());
	}

	ASSERT(0);
	return COMPARE_LESS;
}
