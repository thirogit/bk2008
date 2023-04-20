#ifndef __COUNTACTION_H__
#define __COUNTACTION_H__

#include <set>

template<class ACTION,class MATCHER,class DATA>
class CCountAction : public ACTION
{
public:
	CCountAction(MATCHER *pMatcher) :
	  m_pMatcher(pMatcher),m_Count(0) {};
	virtual bool operator()(DATA* pElement)
	{	
		if(m_skipSet.find(pElement) == m_skipSet.end())
			if(m_pMatcher->Matches(pElement)) m_Count++;

		return true;
	}
	int GetCount()
	{
		return m_Count;
	}	
	void Reset(bool bWithSkip = true)
	{
		m_Count = 0;
		if(bWithSkip)
			m_skipSet.clear();
	}
	void AddSkip(DATA *pSkipPtr)
	{
		m_skipSet.insert(pSkipPtr);
	}
private:
	std::set<DATA*> m_skipSet;


	MATCHER *m_pMatcher;
	int m_Count;
};



#endif


	
	





