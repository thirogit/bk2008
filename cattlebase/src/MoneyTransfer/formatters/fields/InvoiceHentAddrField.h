#ifndef __INVOICEHENTADDRFIELD_H__
#define __INVOICEHENTADDRFIELD_H__

#include "FormatterField.h"

class InvoiceHentAddrField : public FormatterField
{
public:
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif