#ifndef __FORMATTERFIELD_H__
#define __FORMATTERFIELD_H__

#include "../../MoneyTransferEntry.h"
#include "../../MoneyTransferContext.h"

class FormatterField
{
public:
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer) = 0;
	virtual ~FormatterField() {};
};


#endif