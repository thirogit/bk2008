#ifndef __MAXSIZEFIELD_H__
#define __MAXSIZEFIELD_H__

#include "WrapperField.h"

class MaxSizeField : public WrapperField
{
public:
	MaxSizeField(FormatterField* pField,UINT uMaxSize);
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	UINT m_uMaxSize;
};


#endif