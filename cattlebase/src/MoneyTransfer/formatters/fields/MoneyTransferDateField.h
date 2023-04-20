#ifndef __MONEYRANSFERDATEFIELD_H__
#define __MONEYRANSFERDATEFIELD_H__

#include "FormatterField.h"
#include "../../../classes/types/DateTime.h"

class MoneyTransferDateField : public FormatterField
{
public:
	MoneyTransferDateField(CDateTime::DateFormat dtFmt);
	
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	CDateTime::DateFormat m_dtFmt;

};


#endif