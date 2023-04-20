#ifndef __FINDACTION_H__
#define __FINDACTION_H__


template<class ACTION,class MATCHER,class DATA>
class CFindAction : public ACTION
{
public:
	CFindAction(bool bSaveMark,MATCHER *pMatcher) :
	  m_bSaveMark(bSaveMark),m_pMatcher(pMatcher),m_foundCount(0) {};
	virtual bool operator()(DATA* pElement)
	{	
		bool bFound = m_pMatcher->Matches(pElement);
		pElement->bMark = bFound || (m_bSaveMark && pElement->bMark);
		if(bFound) m_foundCount++;
		return true;
	}
	int GetFoundCount()
	{
		return m_foundCount;
	}	
private:
	bool m_bSaveMark;
	MATCHER *m_pMatcher;
	int m_foundCount;
};



#endif


	
	





