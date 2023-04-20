#ifndef __INVOICEHENTCITYZIPFIELD_H__
#define __INVOICEHENTCITYZIPFIELD_H__

#include "FormatterField.h"

class InvoiceHentCityZipField : public FormatterField
{
public:
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif