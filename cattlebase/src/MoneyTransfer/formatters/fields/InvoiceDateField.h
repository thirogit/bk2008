#ifndef __INVOICEDATEFIELD_H__
#define __INVOICEDATEFIELD_H__

#include "FormatterField.h"

class InvoiceDateField : public FormatterField
{
public:	
	InvoiceDateField(CDateTime::DateFormat dtFmt);
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	CDateTime::DateFormat m_dtFmt;

};


#endif