#include "stdafx.h"
#include "CompanySourceAccountNo.h"
#include "../../../datatypes/CompanyInfo.h"

CString CompanySourceAccountNo::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	return Company::GetCompany()->GetAccountNo();	
}