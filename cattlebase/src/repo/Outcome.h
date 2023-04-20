#ifndef __OUTCOME_H__
#define __OUTCOME_H__


template<class OUTCOME_TYPE>
class Outcome
{
public:
	Outcome(OUTCOME_TYPE* pOutcome,bool bFallback) : m_pOutcome(pOutcome),m_bFallback(bFallback)
	{
	}
	
	Outcome(const Outcome<OUTCOME_TYPE>& src)
	{
		CopyFrom(src);
	}
	
	Outcome& operator=(const Outcome<OUTCOME_TYPE>& right)
	{
		CopyFrom(right);
		return *this;			
	}
	
	void CopyFrom(const Outcome<OUTCOME_TYPE>& src)
	{
		m_pOutcome = src.m_pOutcome;
		m_bFallback = src.m_bFallback;
	}
	
	OUTCOME_TYPE* GetOutcome() const
	{
		return m_pOutcome;
	}
	
	bool IsDoFallback() const
	{
		return m_bFallback;
	}
	
private:
	OUTCOME_TYPE* m_pOutcome;
	bool m_bFallback;
};


	

#endif