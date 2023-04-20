#ifndef _MATCHRANGE_H_
#define _MATCHRANGE_H_

template<class DATATYPE,class MATCHER,typename RANGETYPE>
class MatchRange : public Match<DATATYPE>
{
public:
	MatchRange(MATCHER *pMatcher,RANGETYPE min,RANGETYPE max, RANGETYPE prec,RANGETYPE (MATCHER::*getValue)(TYPE *pData)) :
	  m_pMatcher(pMatcher),m_min(min),m_max(max),m_prec(prec),m_getValue(getValue) {};
	virtual bool DoMatch(TYPE *pData)
	{
		RANGETYPE value = (m_pMatcher->*m_getValue)(pData);
		return (value < (limit+eqlprec)) && (value > (limit-eqlprec));

		return m_minInt >= value && value <= m_maxInt;
	}
private:
	RANGETYPE m_min;
	RANGETYPE m_max;
	RANGETYPE m_prec;
	RANGETYPE (MATCHER::*m_getValue)(TYPE *pData);
	MATCHER *m_pMatcher;

};

#endif


	
	





