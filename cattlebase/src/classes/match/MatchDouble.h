#ifndef _MATCHDOUBLE_H_
#define _MATCHDOUBLE_H_

#include "MatchLimit.h"

template<class TYPE,class MATCHER>
class MatchDouble : public MatchLimit<TYPE,MATCHER,double,double>
{
public:
	MatchDouble(MATCHER *pMatcher,double value2Match, CMP_ACTION_TYPE cmpaction, double (MATCHER::*getDouble)(TYPE *pData)) :
	  MatchLimit(pMatcher,value2Match,cmpaction,getDouble) {};
	
};

#endif


	
	





