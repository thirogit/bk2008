#ifndef _INVMATCHER_H_
#define _INVMATCHER_H_

#include "RawDataMatcher.h"
#include "MatchNullInt.h"
#include "MatchIntRange.h"
#include "MatchDate.h"

class CInvDataMatcher : public CRawDataMatcher<Invoice>
{
public:
	
	void FindInvNo(int InvNoFrom,int InvNoTo);
	int GetInvNo(Invoice *pDoc);
	
	void FindExtras(const CString& extras);
	CString GetExtras(Invoice* pDoc);
	
	void FindInvDate(const CDateTime& invDt1,const CDateTime& invDt2,TIMESPAN spanType);
	CDateTime GetInvDate(Invoice* pDoc);
	
	void FindVATRate(double vat,CMP_ACTION_TYPE cmpAction);
	double GetVATRate(Invoice* pDoc);
	
	void FindTotal(double totalValue,CMP_ACTION_TYPE cmpAction);
	double GetTotalValue(Invoice* pDoc);

	void FindPaidBool(bool bPaid);
	bool GetPaidBool(Invoice* pInv);

	void FindPaidDate(const CDateTime& paidDt1,const CDateTime& paidDt2,TIMESPAN spanType);
	CDateTime GetPaidDate(Invoice* pInv);

	void FindPayway(Invoice::PAYWAYS payway);
	Invoice::PAYWAYS GetPayway(Invoice* pInv);
	
	void FindOnlyCorrects();
	InvoiceTypes GetInvoiceType(Invoice* pDoc);

	
};




#endif


	
	





