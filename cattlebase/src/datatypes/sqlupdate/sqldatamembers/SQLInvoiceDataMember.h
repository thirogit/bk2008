#ifndef __SQLINVOICEDATAMEMBER_H__
#define __SQLINVOICEDATAMEMBER_H__

#include "../../Cow.h"
#include "../SQLDataMember.h"

template<class INVOICEDATAMEMBERTYPE>
class SQLInvoiceDataMember : public SQLDataMember<INVOICEDATAMEMBERTYPE>
{
public:
	SQLInvoiceDataMember(INVOICEDATAMEMBERTYPE* pInvoiceMember) : SQLDataMember(pInvoiceMember) {}
	virtual CString GetSQLStringValue(INVOICEDATAMEMBERTYPE* pInvoiceMember)
	{
		Invoice* pInvoice = pInvoiceMember->GetValue();
		if(pInvoice)
		{
			return FormatInt(pInvoice->GetId());
		}
		else
			return GetSQLNull();
	}
};

typedef SQLInvoiceDataMember<RRInvoiceDataMember> SQLRRInvoiceDataMember;
typedef SQLInvoiceDataMember<VATInvoiceDataMember> SQLVATInvoiceDataMember;

#endif