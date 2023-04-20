#ifndef __INVOICEDETAILSEXTERNAL_H__
#define __INVOICEDETAILSEXTERNAL_H__

#include "InvoiceDetails.h"

class CInvoiceDetailsExternalDoc : public CInvoiceDetailsDoc
{
	DECLARE_DYNCREATE(CInvoiceDetailsExternalDoc)
public:
	CInvoiceDetailsExternalDoc();	
	
protected:
	virtual void PrintBottom();
	virtual void PrintRecord(int row,int iLP,CowAccessor* pAccessor,Cow *pCow);
};


#endif
