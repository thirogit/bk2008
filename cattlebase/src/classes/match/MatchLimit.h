#ifndef _MATCHLIMIT_H_
#define _MATCHLIMIT_H_

#include "Matcher.h"

enum CMP_ACTION_TYPE
{
	CMP_PRECISE = 0,
	CMP_ATMOST = 1,
	CMP_ATLEAST = -1
};


template<class DATATYPE,class MATCHER,class LIMITTYPE,class VALUETYPE>
class MatchLimit : public Match<DATATYPE>
{
public:
	MatchLimit(MATCHER *pMatcher,LIMITTYPE limit, CMP_ACTION_TYPE cmpaction, VALUETYPE (MATCHER::*getValue)(DATATYPE *pData)) :
	  m_pMatcher(pMatcher),m_limit(limit),m_cmpaction(cmpaction), m_getValue(getValue) {};
	virtual bool DoMatch(DATATYPE *pData)
	{
		VALUETYPE value = (m_pMatcher->*m_getValue)(pData);
		
		return cmplimit(m_cmpaction,m_limit,value);
	}
protected:
	bool  cmplimit(CMP_ACTION_TYPE cmpaction,const LIMITTYPE& limit,const VALUETYPE& value)
	{
		switch(cmpaction)
		{
			case CMP_ATLEAST:
				return value >= limit;
			case CMP_ATMOST:
				return value <= limit;
			default:
				return value == limit;
		}
		return false;
	}

protected:
	LIMITTYPE m_limit;
	CMP_ACTION_TYPE m_cmpaction;
	VALUETYPE (MATCHER::*m_getValue)(DATATYPE *pData);
	MATCHER *m_pMatcher;

};

#endif


	
	





