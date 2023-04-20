#ifndef __INVOICEHENTNAMEFIELD_H__
#define __INVOICEHENTNAMEFIELD_H__

#include "FormatterField.h"

class InvoiceHentNameField : public FormatterField
{
public:
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
};


#endif