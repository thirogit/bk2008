#ifndef __COMPANYNAMEANDADDRFIELD_H__
#define __COMPANYNAMEANDADDRFIELD_H__

#include "FormatterField.h"

class CompanyNameAndAddrField : public FormatterField
{
public:
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif