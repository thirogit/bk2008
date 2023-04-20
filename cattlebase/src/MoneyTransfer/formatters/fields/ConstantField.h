#ifndef __CONSTANTFIELD_H__
#define __CONSTANTFIELD_H__

#include "FormatterField.h"

class ConstantField : public FormatterField
{
public:
	ConstantField(LPCSTR szConsantFieldValue);
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:
	CString m_FieldValue;

};


#endif