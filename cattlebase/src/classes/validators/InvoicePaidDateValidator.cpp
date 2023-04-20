#include "stdafx.h"
#include "InvoicePaidDateValidator.h"
//#include "../../missing/util.h"
#include "../TempStringLoader.h"



CInvoicePaidDateValidator::CInvoicePaidDateValidator(Invoice* pInvoice) : m_pInvoice(pInvoice)
{

}

bool CInvoicePaidDateValidator::IsValid(const CDateTime& anArg)
{
	return anArg >= m_pInvoice->GetInvoiceDate();
}

CString CInvoicePaidDateValidator::TellMeWhy(const CDateTime& anArg)
{
	return CTempStringLoader(IDS_INVALIDPAIDDATE);
}

bool CInvoicePaidDateValidator::IsSoftViolation(const CDateTime& anArg)
{
	return !IsValid(anArg);
}

bool CInvoicePaidDateValidator::IsHardViolation(const CDateTime& anArg)
{
	return !IsValid(anArg);
}

