#ifndef _MATCHNULLDOUBLE_H_
#define _MATCHNULLDOUBLE_H_

#include "MatchNullable.h"

template<class TYPE,class MATCHER>
class MatchNullDouble : public MatchNullable<TYPE,MATCHER,double,NullDouble>
{
public:
	MatchNullDouble(MATCHER *pMatcher,double double2Match, CMP_ACTION_TYPE cmpaction, NullDouble (MATCHER::*getNullDouble)(TYPE *pData)) :
	  MatchNullable(pMatcher,double2Match,cmpaction,getNullDouble) {};
	
};

#endif


	
	





