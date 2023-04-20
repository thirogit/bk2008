#ifndef __NEWRRINVOICESESSION_H__
#define __NEWRRINVOICESESSION_H__

#include "CowPriceInvoiceSession.h"
#include "InvoiceContext.h"

class NewRRInvoiceSession : public CowPriceInvoiceSession
{
public:
	NewRRInvoiceSession(CCowArray &cowArray,InvoiceContext* pInvoiceContext);   

protected:	
	Doc* m_RRInDoc;
	
protected:
	
	virtual bool CommitInvoice();	
	virtual CString GetSessionTitle() const;		
	virtual void AddItem();
	virtual void DelItem();	
	virtual bool UpdateCowsWithInvoice(CCowInvoiceEntryArray &cowInvEntries);


};

#endif