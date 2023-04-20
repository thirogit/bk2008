#ifndef __EXPENSEDATACOMPARATOR_H__
#define __EXPENSEDATACOMPARATOR_H__

#include "RawData.h"
#include "Expense.h"
#include "RawDataComparator.h"


enum EXPENSEDATA_MEMBERS
{
	 EXPENSEDATA_GROSVALUE = RAWDATA_MEMBER_BASE+1,
	 EXPENSEDATA_VATRATE,
	 EXPENSEDATA_EXPENSETYPE,
	 EXPENSEDATA_EXPENSECATEGORY,
	 EXPENSEDATA_INVOICENO,
	 EXPENSEDATA_EXPENSEDATE,
	 EXPENSEDATA_EXTRAS,
	 EXPENSEDATA_EXPENSETITLE,
	 EXPENSEDATA_NETVALUE,
	 EXPENSEDATA_VATVALUE,
	 EXPENSEDATA_MEMBER_MAX
};




class ExpenseDataComparator : public RawDataComparator<Expense>
{
public:
	virtual COMPARE_RESULT CompareDataBy(UINT dataMember,Expense* DataLeft,Expense* DataRight);
protected:
	

};


#endif