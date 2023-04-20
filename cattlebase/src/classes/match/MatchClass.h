#ifndef _MATCHCLASS_H_
#define _MATCHCLASS_H_

template<class TYPE,class MATCHER,class CLASS>
class MatchClass : public Match<TYPE>
{
public:
	MatchClass(MATCHER *pMatcher,const CLASS& class2Match, CLASS (MATCHER::*getClass)(TYPE *pData)) :
	  m_pMatcher(pMatcher),m_class2Match(class2Match),m_getClass(getClass) {};
	virtual bool DoMatch(TYPE *pData)
	{
		return m_class2Match == (m_pMatcher->*m_getClass)(pData);
	}
private:
	CLASS m_class2Match;
	CLASS (MATCHER::*m_getClass)(TYPE *pData);
	MATCHER *m_pMatcher;
	
};

#endif


	
	





