#ifndef __INVOICEPAIDDATEVALIDATOR_H__
#define __INVOICEPAIDDATEVALIDATOR_H__

#include "../types/Validator.h"
#include "../../datatypes/Invoice.h"

class CInvoicePaidDateValidator : public CValidator<const CDateTime&>
{
public:
	CInvoicePaidDateValidator(Invoice* pInvoice);
	virtual bool IsValid(const CDateTime& anArg);
	virtual CString TellMeWhy(const CDateTime& anArg);
	virtual bool IsSoftViolation(const CDateTime& anArg);
	virtual bool IsHardViolation(const CDateTime& anArg);

private:
	Invoice* m_pInvoice;
};

#endif