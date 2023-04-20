#ifndef __INVOICEDETAILSINTERNAL_H__
#define __INVOICEDETAILSINTERNAL_H__

#include "InvoiceDetails.h"

class CInvoiceDetailsInternalDoc : public CInvoiceDetailsDoc
{
	DECLARE_DYNCREATE(CInvoiceDetailsInternalDoc)
public:
	CInvoiceDetailsInternalDoc();	
	
protected:
	virtual void PrintBottom();
	virtual void PrintRecord(int row,int iLP,CowAccessor* pAccessor,Cow *pCow);
};


#endif
