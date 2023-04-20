#ifndef __DISCREPANCYDECISION_H__
#define __DISCREPANCYDECISION_H__


template<class DECISION,DECISION defaultDecision>
class DiscrepancyDecision
{
public:	
	DiscrepancyDecision(DECISION decision = defaultDecision,bool bRemember = false) : 
			m_bRemember(bRemember),
			m_Decision(decision)
	{
	}
	
	DiscrepancyDecision(const DiscrepancyDecision& src)
	{
		CopyFrom(src);
	}
	
	DiscrepancyDecision& operator=(const DiscrepancyDecision& right)
	{
		CopyFrom(right);
		return *this;
	}

	void CopyFrom(const DiscrepancyDecision& src)
	{
		m_bRemember = src.m_bRemember;
		m_Decision = src.m_Decision;
	}

	bool IsDoRemember() const
	{
		return m_bRemember;
	}
	
	DECISION GetDecision() const
	{
		return m_Decision;
	}


private:
	bool m_bRemember;
	DECISION m_Decision;

};

#endif