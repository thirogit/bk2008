#ifndef __INVOICENUMBERFIELD_H__
#define __INVOICENUMBERFIELD_H__

#include "FormatterField.h"

class InvoiceNumberField : public FormatterField
{
public:	
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif