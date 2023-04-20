#include "stdafx.h"

#include "DocMatcher.h"
#include "MatchString.h"
#include "MatchPtr.h"
#include "MatchClass.h"
#include "MatchType.h"
#include <limits.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CDocDataMatcher::FindDocNo(int mindocno,int maxdocno)
{
	AddMatch(new MatchIntRange<Doc,CDocDataMatcher>(this,mindocno,maxdocno,&CDocDataMatcher::GetDocNo));
}
int CDocDataMatcher::GetDocNo(Doc *pDoc)
{
	return pDoc->GetId();
}

void CDocDataMatcher::FindHent(const Hent* pHent)
{
	AddMatch(new MatchPtr<Doc,CDocDataMatcher,Hent>(this,pHent,false,&CDocDataMatcher::GetHent));
}
Hent* CDocDataMatcher::GetHent(Doc* pDoc)
{
	return pDoc->GetHent();
}

void CDocDataMatcher::FindPlateNo(const CString& plateNo)
{
	AddMatch(new MatchString<Doc,CDocDataMatcher>(this,plateNo,&CDocDataMatcher::GetPlateNo));
}
CString CDocDataMatcher::GetPlateNo(Doc* pDoc)
{
	return pDoc->GetPlateNo();
}

void CDocDataMatcher::FindExtras(const CString& extras)
{
	AddMatch(new MatchString<Doc,CDocDataMatcher>(this,extras,&CDocDataMatcher::GetExtras));
}
CString CDocDataMatcher::GetExtras(Doc* pDoc)
{
	return pDoc->GetExtras();
}

void CDocDataMatcher::FindReason(const Reason* pReason)
{
	AddMatch(new MatchPtr<Doc,CDocDataMatcher,Reason>(this,pReason,false,&CDocDataMatcher::GetReason));
}
Reason* CDocDataMatcher::GetReason(Doc* pDoc)
{
	return pDoc->GetReason();
}

void CDocDataMatcher::FindLoadDate(const CDateTime& loadDt1,const CDateTime& loadDt2,TIMESPAN spanType)
{
		AddMatch(new MatchDate<Doc,CDocDataMatcher>(this,loadDt1,loadDt2,spanType,&CDocDataMatcher::GetLoadDate));
}

CDateTime CDocDataMatcher::GetLoadDate(Doc* pDoc)
{
	return pDoc->GetLoadDate();
}

void CDocDataMatcher::FindDocDate(const CDateTime& docDt1,const CDateTime& docDt2,TIMESPAN spanType)
{
		AddMatch(new MatchDate<Doc,CDocDataMatcher>(this,docDt1,docDt2,spanType,&CDocDataMatcher::GetDocDate));
}
CDateTime CDocDataMatcher::GetDocDate(Doc* pDoc)
{
	return pDoc->GetDocDate();
}

void CDocDataMatcher::FindAmount(int amount,CMP_ACTION_TYPE amount_span)
{
	int mini,maxi;
	switch(amount_span)
	{
		case CMP_PRECISE:
			mini = maxi = amount;
			break;
		case CMP_ATMOST:
			mini = 0;
			maxi = amount;
			break;
		case CMP_ATLEAST:
			mini = amount;
			maxi = INT_MAX;
			break;
	}
	AddMatch(new MatchIntRange<Doc,CDocDataMatcher>(this,mini,maxi,&CDocDataMatcher::GetAmount));
}

int CDocDataMatcher::GetAmount(Doc *pDoc)
{
	return pDoc->GetCowCount();
}



void CDocDataMatcher::FindWithInvoice(bool bWithInvoice)
{
	AddMatch(new MatchType<Doc,CDocDataMatcher,bool>(this,bWithInvoice,&CDocDataMatcher::GetHasInvoice));
}

bool CDocDataMatcher::GetHasInvoice(Doc *pDoc)
{
	return pDoc->HasInvoice();
}