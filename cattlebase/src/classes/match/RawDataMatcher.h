#ifndef __RAWDATAMATCHER_H__
#define __RAWDATAMATCHER_H__

#include "Matcher.h"
#include "MatchDate.h"
#include "MatchType.h"
#include "MatchPtr.h"

template<class TYPE>
class CRawDataMatcher : public  Matcher<TYPE>
{
public:
	
	void FindMarked()
	{
		AddMatch(new MatchType<TYPE,CRawDataMatcher<TYPE>,bool >(this,true,&CRawDataMatcher<TYPE>::GetMarked));
	}
	bool GetMarked(TYPE *pData)
	{
		return pData->bMark;
	}

	void FindCreationTime(const CDateTime& creationTime1,const CDateTime& creationTime2,TIMESPAN spanType)
	{
		AddMatch(new MatchDate<TYPE,CRawDataMatcher<TYPE> >(this,creationTime1,creationTime2,spanType,&CRawDataMatcher<TYPE>::GetCreationTime));
	}

	CDateTime GetCreationTime(TYPE* pData)
	{
		return pData->creation.eventDt;
	}

	void FindLastModificationTime(const CDateTime& lastModificationTime1,const CDateTime& lastModificationTime2,TIMESPAN spanType)
	{
		AddMatch(new MatchDate<TYPE,CRawDataMatcher<TYPE> >(this,lastModificationTime1,lastModificationTime2,spanType,
			&CRawDataMatcher<TYPE>::GetLastModificationTime));
	}

	CDateTime GetLastModificationTime(TYPE* pData)
	{
		return pData->modification.eventDt;
	}

	void FindCreator(UserData* pCreator)
	{
		AddMatch(new MatchPtr<TYPE,CRawDataMatcher<TYPE>,UserData>(this,pCreator,false,&CRawDataMatcher<TYPE>::GetCreator));
	}

	UserData* GetCreator(TYPE* pData)
	{
		return pData->creation.user;
	}

	void FindLastModifier(UserData* pLastModifier)
	{
		AddMatch(new MatchPtr<TYPE,CRawDataMatcher<TYPE>,UserData>(this,pLastModifier,false,&CRawDataMatcher<TYPE>::GetLastModifier));
	}

	UserData* GetLastModifier(TYPE* pData)
	{
		return pData->modification.user;
	}

};


#endif


	
	





