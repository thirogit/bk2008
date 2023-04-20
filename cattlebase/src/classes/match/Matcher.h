#ifndef _MATCHER_H_
#define _MATCHER_H_

#include "DataTypes.h"

template<class TYPE>
class Match;

template<class TYPE>
class Matcher
{
public:
	void AddMatch(Match<TYPE> *pMatch)
	{
		m_matchers.Add(pMatch);
	}
	bool Matches(TYPE *pData)
	{
		for(int i = 0;i < m_matchers.GetSize();i++)
		{
			if(!m_matchers[i]->DoMatch(pData))
				return false;
		}
		return true;
	}

	void Reset()
	{
		m_matchers.FreeAndFlush();
	}

	virtual ~Matcher()
	{
		Reset();
	}

protected:
	PtrFlushArray< Match<TYPE> > m_matchers;

};

template<class TYPE>
class Match
{
public:
	virtual bool DoMatch(TYPE *pData) = 0;
	virtual ~Match() {};
};

#endif


	
	





