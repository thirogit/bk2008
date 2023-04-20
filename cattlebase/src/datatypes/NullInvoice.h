#ifndef __NULLINVOICE_H__
#define __NULLINVOICE_H__

#include "Invoice.h"

class NullInvoice : public Invoice
{
public:
	NullInvoice(UINT Id = NULL_INVOICEID);
	virtual int GetCowQty() const;
	virtual CString GetInvoiceNoStr() const;	
	virtual BuyOrSell GetBuyOrSell() const;
	virtual Hent* GetInvoiceHent() const;
	virtual void GetInvoiceCows(CCowArray &cows);
};

#endif