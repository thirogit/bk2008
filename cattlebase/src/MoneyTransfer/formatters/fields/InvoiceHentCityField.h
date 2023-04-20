#ifndef __INVOICEHENTCITYFIELD_H__
#define __INVOICEHENTCITYFIELD_H__

#include "FormatterField.h"

class InvoiceHentCityField : public FormatterField
{
public:
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif