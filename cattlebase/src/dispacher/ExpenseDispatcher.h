#ifndef __EXPENSEDISPATCHER_H__
#define __EXPENSEDISPATCHER_H__

#include "../datatypes/Expense.h"
#include "../global_arrays.h"
#include "../mainfrm_ctrl.h"

class	ExpenseDispatcher
{
public:
	static void Dispatch(Expense *pExpense);
};


#endif