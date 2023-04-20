#ifndef __EXPENSECATEGORYARRAY_H__
#define __EXPENSECATEGORYARRAY_H__

#include "../RefCountingPtrArray.h"
#include "ExpenseCategory.h"

typedef RefCountingPtr<ExpenseCategory> ExpenseCategoryPtr;

class CExpenseCategoryArray : public CRefCountingPtrArray<ExpenseCategory> 
{
public:
	ExpenseCategoryPtr GetExpenseCategory(int expenseCategoryId);
};

#endif