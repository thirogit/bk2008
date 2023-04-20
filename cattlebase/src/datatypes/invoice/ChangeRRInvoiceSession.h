#ifndef __CHANGERRINVOICESESSION_H__
#define __CHANGERRINVOICESESSION_H__

#include "CowPriceInvoiceSession.h"
#include "RRInvoiceContext.h"

class CChangeRRInvoiceSession : public CowPriceInvoiceSession
{
public:
	CChangeRRInvoiceSession(RRInvoice *pInvoice,InvoiceContext* pInvoiceContext);   
protected:
	virtual bool CommitInvoice();	
	virtual CString GetSessionTitle() const;		
	virtual void AddItem();
	virtual void DelItem();
	

	bool DeleteRRInvoiceForCows(CCowArray& cows);
	bool UpdateCowsWithRRInvoice(CCowArray& cows,RRInvoice* pInvoice);

	CCowArray m_cowsAdded;
	CCowArray m_cowsDeleted;
	RRInvoice *m_Inv2Edit;
	Doc* m_RRInDoc;
	
	

};

#endif