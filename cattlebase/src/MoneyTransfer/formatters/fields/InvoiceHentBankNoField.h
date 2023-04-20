#ifndef __INVOICEHENTBANKNOFIELD_H__
#define __INVOICEHENTBANKNOFIELD_H__

#include "FormatterField.h"

class InvoiceHentBankNoField : public FormatterField
{
public:
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
};


#endif