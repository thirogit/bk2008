#ifndef _MATCHNULLABLE_H_
#define _MATCHNULLABLE_H_

#include "MatchLimit.h"

template<class DATATYPE,class MATCHER,class LIMITTYPE,class VALUETYPE>
class MatchNullable : public MatchLimit<DATATYPE,MATCHER,LIMITTYPE,VALUETYPE>
{
public:
	MatchNullable(MATCHER *pMatcher,LIMITTYPE limit, CMP_ACTION_TYPE cmpaction, VALUETYPE (MATCHER::*getValue)(DATATYPE *pData)) :
	 MatchLimit(pMatcher,limit,cmpaction,getValue) {};
	virtual bool DoMatch(DATATYPE *pData)
	{
		VALUETYPE value = (m_pMatcher->*m_getValue)(pData);
		if(value.IsNull()) return false;
		return cmplimit(m_cmpaction,m_limit,value);
	}
};

#endif


	
	





