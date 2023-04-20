#include "stdafx.h"
#include "CompanyNameAndAddrField.h"
#include "../../../datatypes/CompanyInfo.h"


CString CompanyNameAndAddrField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	CString sCompanyNameAndAddr;
	Invoice* pInvoice = pMoneyTransfer->GetInvoice();
	
	const Company* pCompany = Company::GetCompany();

	sCompanyNameAndAddr = pCompany->GetCompanyName() + " (" + pCompany->GetFarmNo() + ") " + pCompany->GetStreet() + ' ' + pCompany->GetPOBox() + ", " + pCompany->GetCity() + ' ' + pCompany->GetZipCode();
	return sCompanyNameAndAddr;
}

