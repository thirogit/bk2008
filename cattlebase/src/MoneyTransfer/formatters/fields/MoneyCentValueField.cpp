#include "stdafx.h"
#include "MoneyCentValueField.h"
#include "../../../classes/IntString.h"

CString MoneyCentValueField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	Money transferValue  = pMoneyTransfer->GetTransferValue();
	int cents = transferValue.MoneyInt() * 100 + transferValue.MoneyCents();

	return CIntString(cents);
}