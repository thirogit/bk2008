#ifndef __TYPEDEQUIVALENCE_H__
#define __TYPEDEQUIVALENCE_H__

#include "Equivalence.h"
#include "Discrepancy.h"

template<class TYPE>
class TypedEquivalence : public Equivalence
{
public:
	TypedEquivalence(TYPE* pLocal,TYPE* pRemote) : m_pLocal(pLocal),m_pRemote(pRemote),m_Discrepancy(*this)
	{
	}
	
	TYPE* GetLocal() const
	{
		return m_pLocal;
	}
	
	TYPE* GetRemote() const
	{
		return m_pRemote;
	}

	void ReconcileLocal()
	{
		switch(m_Discrepancy.GetType())
		{
			case HAS_BOTH:
				m_pRemote->CopyFrom(*m_pLocal);
				break;
			case MISSING_REMOTE:
				m_pRemote = new TYPE(*m_pLocal);
				break;
		}

		m_Discrepancy = Discrepancy< TYPE >(*this);
				
	}

	void ReconcileRemote()
	{
		switch(m_Discrepancy.GetType())
		{
			case HAS_BOTH:
				m_pLocal->CopyFrom(*m_pRemote);
				break;
			case MISSING_LOCAL:
				m_pLocal = new TYPE(*m_pRemote);
				break;
		}
	}

	const Discrepancy< TYPE >& GetDiscrepancy() const
	{
		return m_Discrepancy;
	}
		
	virtual ~TypedEquivalence()
	{
		if(m_pRemote)
			delete m_pRemote;

		if(m_pLocal)
			delete m_pLocal; 
	}

private:
	TYPE* m_pLocal;
	TYPE* m_pRemote;
	Discrepancy< TYPE > m_Discrepancy;
};

#endif