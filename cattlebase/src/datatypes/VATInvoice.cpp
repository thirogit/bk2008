#include "stdafx.h"
#include "VATInvoice.h"
//#include "RRInvoiceData.h"


bool VATInvoice::CanMakeVATInvoiceOnDocs(CDocDataArray &docs)
{
	Doc *pDoc = NULL;
	int i,s = docs.GetSize();
	if(s > 0)
	{
		pDoc = docs[0];
		Hent *firstDocHent = pDoc->GetHent();
		
		for(i = 1;i < s;i++)
			if(docs[i]->GetHent() != firstDocHent) return false;

		for(i = 0;i < s;i++)
			if(!docs[i]->CanMakeVATInvoiceOnThisDoc()) return false;
	
		return true;
	}
	return false;
}


VATInvoice::VATInvoice(const Invoice *pInvoice/* = NULL*/)
{
	CopyFrom(*pInvoice);
}


CString VATInvoice::GetInvoiceNoStr() const
{
	CString outNo;
	outNo.Format("FV%04d",invoiceno);
	return outNo;
}

BuyOrSell VATInvoice::GetBuyOrSell() const
{
	switch(GetRealInvoiceType())
	{
	case IN_INV_VAT:
        return BUY;
	case OUT_INV_VAT:
        return SELL;
	default:
		ASSERT(NULL && "Unknown VAT invoice type");
	}
	return BUY;
}

Hent* VATInvoice::GetInvoiceHent() const
{
	return invoiceDocs[0]->GetHent();
}
VATInvoice* VATInvoice::GetLastGoodVATInvoice() const
{
	VATInvoice* pNonConstInvoice = const_cast<VATInvoice*>(this);
	return (VATInvoice*)pNonConstInvoice->GetLastGoodInvoice();
}

