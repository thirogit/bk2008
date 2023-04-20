#ifndef _MATCHNULLINT_H_
#define _MATCHNULLINT_H_

#include "MatchNullable.h"

template<class TYPE,class MATCHER>
class MatchNullInt : public MatchNullable<TYPE,MATCHER,int,NullInt>
{
public:
	MatchNullInt(MATCHER *pMatcher,int int2Match, CMP_ACTION_TYPE cmpaction, const NullInt (MATCHER::*getNullInt)(TYPE *pData)) :
	  MatchNullable(pMatcher,int2Match,cmpaction,getNullDouble) {};

};

#endif


	
	





