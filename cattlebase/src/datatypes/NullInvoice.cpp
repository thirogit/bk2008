#include "stdafx.h"
#include "NullInvoice.h"
#include "../exceptions/StrException.h"


NullInvoice::NullInvoice(UINT Id/* = NULL_INVOICEID*/) : Invoice(Id)
{

}

int NullInvoice::GetCowQty() const
{
	ASSERT(0);
	throw new CStrException("Error NullInvoice::GetCowQty()");
}
CString NullInvoice::GetInvoiceNoStr() const
{
	ASSERT(0);
	throw new CStrException("Error NullInvoice::GetInvoiceNoStr()");
}	
BuyOrSell NullInvoice::GetBuyOrSell() const
{
	ASSERT(0);
	throw new CStrException("Error NullInvoice::GetBuyOrSell()");
}
Hent* NullInvoice::GetInvoiceHent() const
{
	ASSERT(0);
	throw new CStrException("Error NullInvoice::GetInvoiceHent()");
}
void NullInvoice::GetInvoiceCows(CCowArray &cows)
{
	ASSERT(0);
	throw new CStrException("Error NullInvoice::GetInvoiceCows()");
}