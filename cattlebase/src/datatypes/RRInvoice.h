#ifndef __RRINVOICE_H__
#define __RRINVOICE_H__

#include "Invoice.h"
#include "InDoc.h"

class RRInvoice : public Invoice
{
public:
	RRInvoice(const Invoice *pInvoice = NULL);

	static bool HaveSameOwner(CCowArray &cows);
	static bool DoesAllCowsHasOwners(CCowArray& cows);
	static bool CanMakeInvoice(CCowArray& cows);
	static bool DoesAllCowsBelongToSameInDoc(CCowArray &cows);
	static Doc* GetInDocAllCowsBelongTo(CCowArray &cows);


	virtual CString GetInvoiceNoStr() const;	
	virtual Hent* GetInvoiceHent() const;	
	virtual BuyOrSell GetBuyOrSell() const;

	RRInvoice* GetLastGoodRRInvoice() const;
	InDoc* GetInDoc() const;

};




#endif