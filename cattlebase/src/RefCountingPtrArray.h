#ifndef __REFCOUNTINGPTRARRAY_H__
#define __REFCOUNTINGPTRARRAY_H__

#include "PtrFlushArray.h"
#include "RefCountingPtr.h"

template<class PTRTYPE>
class CRefCountingPtrArray 
{
public:

	CRefCountingPtrArray()
	{
	}
	
	int GetSize() const
	{
		return m_refs.GetSize();
	}
	bool IsEmpty() const
	{
		return m_refs.IsEmpty();
	}

	bool RemoveAll()
	{
		bool bExternalReference = false;
		PtrFlushArray< RefCountingPtr<PTRTYPE> >::ArrayIterator it = m_refs.iterator();
		while(it.hasNext())
		{
			RefCountingPtr<PTRTYPE>* ref = *(it++);
			if(ref->Count() == 1)
			{
				delete ref->GetPtr();				
			}
			else
				bExternalReference = true;

			delete ref;
		}

		m_refs.RemoveAll();
		return !bExternalReference;
	}
	
	RefCountingPtr<PTRTYPE> GetAt(int nIndex) const
	{
		return *m_refs.GetAt(nIndex);
	}
	
	void Add(PTRTYPE* newElement)
	{
		m_refs.Add(new RefCountingPtr<PTRTYPE>(newElement));
	}

	
	RefCountingPtr<PTRTYPE> operator[](int nIndex) const
	{
		return GetAt(nIndex);
	}
		
	bool RemoveAt(int nIndex, int nCount = 1)
	{
		bool bUnreferenced = false;
		RefCountingPtr<PTRTYPE>* ref = m_refs.GetAt(nIndex);
		if(ref->Count() == 1)
		{
			delete ref->GetPtr();
			bUnreferenced = true;
		}
		delete ref;
		m_refs.RemoveAt(nIndex);
		return bUnreferenced;
	}

	bool AllUnreferenced()
	{
		PtrFlushArray< RefCountingPtr<PTRTYPE>* >::ArrayIterator it = m_refs.iterator();
		while(it.hasNext())
		{
			RefCountingPtr<PTRTYPE>* ref = *it;
			if(ref->Count() > 1)
				return false;
			it++;
		}
		return true;
	}

protected:
	PtrFlushArray< RefCountingPtr<PTRTYPE> > m_refs;

};

#endif