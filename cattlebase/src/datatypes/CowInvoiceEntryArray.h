#ifndef __COWINVOICEENTRYARRAY_H__
#define __COWINVOICEENTRYARRAY_H__

#include "CowInvoiceEntry.h"
#include "../SortArray.h"
#include "../accessor/CowAccessor.h"
#include "CowInvoiceEntryComparator.h"

class CCowInvoiceEntryArray : public CSortArray<CowInvoiceEntry>
{
private:
	class CCowInvoiceEntrySortArrayComparator : public CCowInvoiceEntryComparator,public CSortArray<CowInvoiceEntry>::SortArrayComparator
	{
	public:
		CCowInvoiceEntrySortArrayComparator(UINT sortBy,bool bAsc,CowAccessor *pAccessor);
		virtual COMPARE_RESULT Compare(CowInvoiceEntry *DataLeft,CowInvoiceEntry *DataRight);
	protected:
		CowAccessor *m_pAccessor;
		UINT m_sortByMember;
	};

public:
	//void CommitChanges();
	//void RollbackChanges();
	void CommitCowChanges();
	void RollbackCowChanges();
	virtual void SortBy(UINT sortBy,bool bAscending,CowAccessor *pAccessor);
};



#endif