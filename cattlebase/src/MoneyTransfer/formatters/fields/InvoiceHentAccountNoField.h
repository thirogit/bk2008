#ifndef __INVOICEHENTACCOUNTNOFIELD_H__
#define __INVOICEHENTACCOUNTNOFIELD_H__

#include "FormatterField.h"

class InvoiceHentAccountNoField : public FormatterField
{
public:
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
};


#endif