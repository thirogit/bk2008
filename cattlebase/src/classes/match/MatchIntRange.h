#ifndef _MATCHINTRANGE_H_
#define _MATCHINTRANGE_H_

template<class TYPE,class MATCHER>
class MatchIntRange : public Match<TYPE>
{
public:
	MatchIntRange(MATCHER *pMatcher,int min,int max, int (MATCHER::*getInt)(TYPE *pData)) :
	  m_pMatcher(pMatcher),m_minInt(min),m_maxInt(max),m_getInt(getInt) {};
	virtual bool DoMatch(TYPE *pData)
	{
		int value = (m_pMatcher->*m_getInt)(pData);
		return m_minInt <= value && value <= m_maxInt;
	}
private:
	int m_minInt;
	int m_maxInt;
	int (MATCHER::*m_getInt)(TYPE *pData);
	MATCHER *m_pMatcher;

};

#endif


	
	





