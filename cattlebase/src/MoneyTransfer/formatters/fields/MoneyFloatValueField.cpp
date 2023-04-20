#include "stdafx.h"
#include "MoneyFloatValueField.h"

MoneyFloatValueField::MoneyFloatValueField(const FloatFormat& floatFmt) : m_FloatFmt(floatFmt)
{
}

CString MoneyFloatValueField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	char test[50];
	double a = 1112.44499999999999999;
	
	Money transferValue  = pMoneyTransfer->GetTransferValue();
	double value = transferValue.GetDouble();
	sprintf(test,"%.f",value);
	sprintf(test,"%.f",a);
	return transferValue.ToString(NULL,m_FloatFmt);
}