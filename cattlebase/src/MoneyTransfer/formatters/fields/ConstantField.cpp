#include "stdafx.h"
#include "ConstantField.h"


ConstantField::ConstantField(LPCSTR szConsantFieldValue) : m_FieldValue(szConsantFieldValue)
{

}

CString ConstantField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	return m_FieldValue;
}