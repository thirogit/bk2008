#include "stdafx.h"
#include "CowInvoiceEntryArray.h"


CCowInvoiceEntryArray::CCowInvoiceEntrySortArrayComparator::CCowInvoiceEntrySortArrayComparator(UINT sortBy,bool bAsc,CowAccessor *pAccessor) :
	CSortArray<CowInvoiceEntry>::SortArrayComparator(bAsc),m_pAccessor(pAccessor),m_sortByMember(sortBy)
{
}

COMPARE_RESULT CCowInvoiceEntryArray::CCowInvoiceEntrySortArrayComparator::Compare(CowInvoiceEntry *DataLeft,CowInvoiceEntry *DataRight)
{
	return CCowInvoiceEntryComparator::CompareBy(m_sortByMember,DataLeft,DataRight,m_pAccessor);
}


void CCowInvoiceEntryArray::CommitCowChanges()
{
	ArrayIterator it = iterator();
	while(it.hasNext())
		(*(it++))->GetCow()->CommitChanges();
}
void CCowInvoiceEntryArray::RollbackCowChanges()
{
	ArrayIterator it = iterator();
	while(it.hasNext())
		(*(it++))->GetCow()->RollbackChanges();
}


void CCowInvoiceEntryArray::SortBy(UINT sortBy,bool bAscending,CowAccessor *pAccessor)
{
	CSortArray<CowInvoiceEntry>::Sort(CCowInvoiceEntrySortArrayComparator(sortBy,bAscending,pAccessor));
}



