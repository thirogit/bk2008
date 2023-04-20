#ifndef __DISCREPANCY_H__
#define __DISCREPANCY_H__


enum DISCREPANCY_TYPE
{
	MISSING_REMOTE,
	MISSING_LOCAL,
	HAS_BOTH,
	BOTH_MISSING
};

template<class TYPE> class TypedEquivalence;

template<class TYPE>
class Discrepancy 
{
public:

	Discrepancy(const TypedEquivalence<TYPE>& equivalence) : m_Side(NULL),m_Type(BOTH_MISSING)
	{
		bool bGotLocal = equivalence.GetLocal() != NULL;
		bool bGotRemote = equivalence.GetRemote() != NULL;

		if(bGotLocal && bGotRemote)
		{
			m_Type = HAS_BOTH;
			m_Side = equivalence.GetLocal();
		}

		if(bGotLocal && !bGotRemote)
		{
			m_Type = MISSING_REMOTE;
			m_Side = equivalence.GetLocal();
		}

		if(!bGotLocal && bGotRemote)
		{
			m_Type = MISSING_LOCAL;
			m_Side = equivalence.GetRemote();
		}
	}

	Discrepancy(const Discrepancy& src)
	{
		CopyFrom(src);
	}

	Discrepancy& operator=(const Discrepancy& right)
	{
		CopyFrom(right);
		return *this;
	}

	void CopyFrom(const Discrepancy& src)
	{
		m_Type = src.m_Type;
		m_Side = src.m_Side;
	}
	
	DISCREPANCY_TYPE GetType() const
	{
		return m_Type;
	}
	
	TYPE* GetSide() const
	{
		return m_Side;
	}
	
	
private:
	DISCREPANCY_TYPE m_Type;
	TYPE* m_Side;
	
		
	
	
};

#endif