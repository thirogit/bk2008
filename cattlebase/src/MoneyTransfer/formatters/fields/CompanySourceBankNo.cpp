#include "stdafx.h"
#include "CompanySourceBankNo.h"
#include "../../../datatypes/CompanyInfo.h"
#include "utils/AccountNoUtils.h"

CString CompanySourceBankNo::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	return AccountNoUtils::GetBankNo(Company::GetCompany()->GetAccountNo());	
}