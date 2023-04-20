#include "stdafx.h"
#include "ExpenseMatcher.h"

#include "MatchType.h"
#include "MatchDouble.h"
#include "MatchString.h"

void CExpenseDataMatcher::FindGrosValue(double grosValue,CMP_ACTION_TYPE cmpAction)
{
	AddMatch(new MatchDouble<Expense,CExpenseDataMatcher>(this,grosValue,cmpAction,&CExpenseDataMatcher::GetGrosValue));
}

double CExpenseDataMatcher::GetGrosValue(Expense *pExpense)
{
	return pExpense->GetGrosValue().GetDouble();
}

void CExpenseDataMatcher::FindVATRate(double vatRate,CMP_ACTION_TYPE cmpAction)
{
	AddMatch(new MatchDouble<Expense,CExpenseDataMatcher>(this,vatRate,cmpAction,&CExpenseDataMatcher::GetVATRate));
}
double CExpenseDataMatcher::GetVATRate(Expense *pExpense)
{
	return pExpense->GetVATRate();
}

void CExpenseDataMatcher::FindExpenseType(Expense::EXPENSE_TYPE expenseType)
{
	AddMatch(new MatchType<Expense,CExpenseDataMatcher,Expense::EXPENSE_TYPE>(this,expenseType,&CExpenseDataMatcher::GetExpenseType));
}
Expense::EXPENSE_TYPE CExpenseDataMatcher::GetExpenseType(Expense *pExpense)
{
	return pExpense->GetExpenseType();
}

void CExpenseDataMatcher::FindInvoiceNo(const CString& invoiceNo)
{
	AddMatch(new MatchString<Expense,CExpenseDataMatcher>(this,invoiceNo,&CExpenseDataMatcher::GetInvoiceNo));
}
CString CExpenseDataMatcher::GetInvoiceNo(Expense *pExpense)
{
	return pExpense->GetInvoiceNo();
}

void CExpenseDataMatcher::FindExpenseTitle(const CString& expenseTitle)
{
	AddMatch(new MatchString<Expense,CExpenseDataMatcher>(this,expenseTitle,&CExpenseDataMatcher::GetExpenseTitle));
}
CString CExpenseDataMatcher::GetExpenseTitle(Expense *pExpense)
{
	return pExpense->GetExpenseTitle();
}

void CExpenseDataMatcher::FindExtras(const CString& extras)
{
	AddMatch(new MatchString<Expense,CExpenseDataMatcher>(this,extras,&CExpenseDataMatcher::GetExtras));
}
CString CExpenseDataMatcher::GetExtras(Expense *pExpense)
{
	return pExpense->GetExtras();
}

void CExpenseDataMatcher::FindExpenseDate(const CDateTime& expenseDt1,const CDateTime& expenseDt2,TIMESPAN spanType)
{
	AddMatch(new MatchDate<Expense,CExpenseDataMatcher>(this,expenseDt1,expenseDt2,spanType,&CExpenseDataMatcher::GetExpenseDate));
}
CDateTime CExpenseDataMatcher::GetExpenseDate(Expense *pExpense)
{
	return pExpense->GetExpenseDate();
}

void CExpenseDataMatcher::FindExpenseCategory(ExpenseCategory* expenseCategory)
{
	AddMatch(new MatchPtr<Expense,CExpenseDataMatcher,ExpenseCategory>(this,expenseCategory,false,&CExpenseDataMatcher::GetExpenseCategory));	
}
ExpenseCategory* CExpenseDataMatcher::GetExpenseCategory(Expense *pExpense)
{
	return pExpense->GetExpenseCategory().GetPtr();
}






	
	





