#ifndef __COMPANYSOURCEBANKNO_H__
#define __COMPANYSOURCEBANKNO_H__

#include "FormatterField.h"

class CompanySourceBankNo : public FormatterField
{
public:
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);

};


#endif