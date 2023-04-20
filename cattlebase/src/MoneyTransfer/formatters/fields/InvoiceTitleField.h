#ifndef __INVOICETITLEFIELD_H__
#define __INVOICETITLEFIELD_H__

#include "FormatterField.h"

class InvoiceTitleField : public FormatterField
{
public:	
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif