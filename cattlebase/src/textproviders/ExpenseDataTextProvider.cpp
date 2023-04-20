#include "stdafx.h"
#include "ExpenseDataTextProvider.h"
#include "../consts.h"
#include "../datatypes/ExpenseDataComparator.h"


CString ExpenseDataTextProvider::GetDataText(Expense *pData,UINT memberID)
{
	CString retStr;
   	switch(memberID)
	{
		case EXPENSEDATA_NETVALUE:
			retStr = (pData->GetGrosValue()/(1.0+pData->GetVATRate())).ToString();
			break;
		case EXPENSEDATA_VATVALUE:
			retStr = ((pData->GetGrosValue()/(1.0+pData->GetVATRate()))*pData->GetVATRate()).ToString();
			break;
		case EXPENSEDATA_GROSVALUE:
			retStr = pData->GetGrosValue().ToString();
			break;
		case EXPENSEDATA_VATRATE:
			retStr.Format("%.2f%%",pData->GetVATRate()*100.0);
			break;
		case EXPENSEDATA_EXPENSETYPE:
			retStr = ExpenseTypeStr::GetExpenseTypeStr(pData->GetExpenseType());
			break;
		case EXPENSEDATA_EXPENSECATEGORY:
			retStr = pData->GetExpenseCategory()->GetExpenseCategoryName();
			break;
		case EXPENSEDATA_INVOICENO:
			retStr = pData->GetInvoiceNo();
			break;
		case EXPENSEDATA_EXPENSEDATE:
			retStr = pData->GetExpenseDate().GetDayDate();
			break;
		case EXPENSEDATA_EXTRAS:
			retStr = pData->GetExtras();
			break;
		case EXPENSEDATA_EXPENSETITLE:
			retStr = pData->GetExpenseTitle();	
			break;
		default:
			ASSERT(0);
			break;
	}	
	return retStr;
}
