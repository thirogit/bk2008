#ifndef _MATCHTYPE_H_
#define _MATCHTYPE_H_

template<class DATATYPE,class MATCHER,typename TYPE>
class MatchType : public Match<DATATYPE>
{
public:
	MatchType(MATCHER *pMatcher,TYPE type2Match, TYPE (MATCHER::*getTypeVal)(DATATYPE *pData)) :
	  m_pMatcher(pMatcher),m_type2Match(type2Match),m_getTypeVal(getTypeVal) {};
	virtual bool DoMatch(DATATYPE *pData)
	{
		return m_type2Match == (m_pMatcher->*m_getTypeVal)(pData);
	}
private:
	TYPE m_type2Match;
	TYPE (MATCHER::*m_getTypeVal)(DATATYPE *pData);
	MATCHER *m_pMatcher;

};

#endif


	
	





