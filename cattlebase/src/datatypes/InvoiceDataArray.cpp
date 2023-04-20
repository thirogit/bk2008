#include "stdafx.h"
#include "InvoiceDataArray.h"
#include "InvoiceDataComparator.h"


Invoice* CInvoiceDataArray::GetPtrByInvId(int invid)
{
	Invoice *temp;
	for(int i = 0;i< GetSize();i++)
	{
		temp = (Invoice*)GetAt(i); 
           if(temp->GetId() == invid) return temp;
	}
	return NULL;
}

void CInvoiceDataArray::CommitChanges()
{
	ArrayIterator it = iterator();
	while(it.hasNext())
		(*(it++))->CommitChanges();
}
void CInvoiceDataArray::RollbackChanges()
{
	ArrayIterator it = iterator();
	while(it.hasNext())
		(*(it++))->RollbackChanges();
}
