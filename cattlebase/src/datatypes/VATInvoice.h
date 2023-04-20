#ifndef __VATINVOICE_H__
#define __VATINVOICE_H__

#include "Invoice.h"
#include "DocDataArray.h"

class VATInvoice : public Invoice
{
public:
	static bool CanMakeVATInvoiceOnDocs(CDocDataArray &docs);
public:
	VATInvoice(const Invoice *pInvoice = NULL);
	
	virtual CString GetInvoiceNoStr() const;	
	virtual BuyOrSell GetBuyOrSell() const;
	virtual Hent* GetInvoiceHent() const;
	VATInvoice* GetLastGoodVATInvoice() const;

	CDocDataArray invoiceDocs;
	
};

#endif