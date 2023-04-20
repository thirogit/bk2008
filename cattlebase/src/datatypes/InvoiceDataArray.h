#ifndef __INVOICEDATAARRAY_H__
#define __INVOICEDATAARRAY_H__

#include "../DataArray.h"
#include "Invoice.h"
#include "InvoiceDataComparator.h"

class  CInvoiceDataArray : public CDataArray<Invoice,InvoiceDataComparator>
{
public:
	Invoice* GetPtrByInvId(int invid);
	void CommitChanges();
	void RollbackChanges();
};




#endif