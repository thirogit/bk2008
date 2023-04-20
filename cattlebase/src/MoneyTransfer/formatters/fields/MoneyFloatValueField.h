#ifndef __MONEYFLOATVALUEFIELD_H__
#define __MONEYFLOATVALUEFIELD_H__

#include "FormatterField.h"
#include "../../../classes/format/FloatFormat.h"

class MoneyFloatValueField : public FormatterField
{
public:
	MoneyFloatValueField(const FloatFormat& floatFmt);
	virtual CString GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer);
private:

	FloatFormat m_FloatFmt;
};


#endif