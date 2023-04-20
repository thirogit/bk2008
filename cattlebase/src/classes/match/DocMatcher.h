#ifndef _DOCMATCHER_H_
#define _DOCMATCHER_H_

#include "RawDataMatcher.h"
#include "MatchNullInt.h"
#include "MatchIntRange.h"
#include "MatchDate.h"

class CDocDataMatcher : public CRawDataMatcher<Doc>
{
public:
	
	void FindDocNo(int mindocno,int maxdocno);
	int GetDocNo(Doc *pDoc);
	
	void FindHent(const Hent* pHent);
	Hent* GetHent(Doc* pDoc);

	void FindPlateNo(const CString& plateNo);
	CString GetPlateNo(Doc* pDoc);

	void FindExtras(const CString& extras);
	CString GetExtras(Doc* pDoc);
	
	void FindReason(const Reason* pReason);
	Reason* GetReason(Doc* pDoc);

	void FindLoadDate(const CDateTime& loadDt1,const CDateTime& loadDt2,TIMESPAN spanType);
	CDateTime GetLoadDate(Doc* pDoc);
	
	void FindDocDate(const CDateTime& docDt1,const CDateTime& docDt2,TIMESPAN spanType);
	CDateTime GetDocDate(Doc* pDoc);
	
	void FindAmount(int amount,CMP_ACTION_TYPE amount_span);
	int GetAmount(Doc *pDoc);

	void FindWithInvoice(bool bWithInvoice);	
	bool GetHasInvoice(Doc *pDoc);

};




#endif


	
	





