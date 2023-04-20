#ifndef __NEWVATINVOICESESSION_H__
#define __NEWVATINVOICESESSION_H__

#include "CowPriceInvoiceSession.h"
#include "InvoiceContext.h"

class NewVATInvoiceSession : public CowPriceInvoiceSession
{
public:
	NewVATInvoiceSession(InvoiceContext* pInvoiceContext,CDocDataArray &docArray);   	

protected:	
	virtual bool CommitInvoice();	
	virtual CString GetSessionTitle() const;		
	virtual void AddItem();
	virtual void DelItem();	

};

#endif