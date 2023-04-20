#ifndef __EXPENSEDATATEXTPROVIDER_H__
#define __EXPENSEDATATEXTPROVIDER_H__

#include "BaseTextProvider.h"
#include "../datatypes/Expense.h"

class ExpenseDataTextProvider : public BaseTextProvider<Expense>
{
public:
	virtual CString GetDataText(Expense *pData,UINT memberID);

};

#endif