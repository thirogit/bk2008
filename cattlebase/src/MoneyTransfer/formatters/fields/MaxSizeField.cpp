#include "stdafx.h"
#include "MaxSizeField.h"


MaxSizeField::MaxSizeField(FormatterField* pField,UINT uMaxSize) : WrapperField(pField),m_uMaxSize(uMaxSize)
{
}

CString MaxSizeField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	CString sValue = WrapperField::GetFieldValue(pContext,pMoneyTransfer);
	return sValue.Left(m_uMaxSize);
}
