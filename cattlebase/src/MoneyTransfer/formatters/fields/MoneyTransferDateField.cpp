#include "stdafx.h"
#include "MoneyTransferDateField.h"


MoneyTransferDateField::MoneyTransferDateField(CDateTime::DateFormat dtFmt) :m_dtFmt(dtFmt)
{
}

CString MoneyTransferDateField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	return  pContext->GetTransferDate().GetDateInFmt(m_dtFmt);
}

