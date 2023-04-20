#ifndef _MATCHDATE_H_
#define _MATCHDATE_H_

#include "../types/DateTime.h"
#include "Matcher.h"

enum TIMESPAN 
{ 
	SPAN_BETWEEN, 
	SPAN_EXCEPT, 
	SPAN_BEFORE, 
	SPAN_AFTER 
};

template<class TYPE,class MATCHER>
class MatchDate : public Match<TYPE>
{
public:
	MatchDate(MATCHER *pMatcher,const CDateTime& timeh,const CDateTime& timel,TIMESPAN timeSpan,
		CDateTime (MATCHER::*getDate)(TYPE *pData)) :
	  m_pMatcher(pMatcher),m_getDate(getDate),m_timeHigh(timeh),m_timeLow(timel),m_timeSpan(timeSpan) {};


	virtual bool DoMatch(TYPE *pData)
	{
		CDateTime date2Match = (m_pMatcher->*m_getDate)(pData);
		if(date2Match.IsNull()) return false;

		return TimeInSpan(date2Match,m_timeHigh,m_timeLow,m_timeSpan);
		
	}
private:
	CDateTime m_timeHigh;
	CDateTime m_timeLow;
	TIMESPAN m_timeSpan;

	CDateTime (MATCHER::*m_getDate)(TYPE *pData);

	MATCHER *m_pMatcher;
protected:
	bool TimeInSpan(const CDateTime &timeVal,const CDateTime& timeLow,const CDateTime& timeHigh,TIMESPAN span)
	{
		switch(span)
		{
			case SPAN_BETWEEN:
		
			if(timeVal >= timeLow && timeVal <= timeHigh)
				return true;
			break;		
			
			case SPAN_EXCEPT:
			if(timeVal < timeLow || timeVal > timeHigh)
				return true;
			break;
			case SPAN_AFTER:
			if( timeLow < timeVal )
				return true;
			break;
			case SPAN_BEFORE:
			if(timeVal < timeLow)
				return true;
			break;
			

		}
		return false;
	}


};

#endif


	
	





