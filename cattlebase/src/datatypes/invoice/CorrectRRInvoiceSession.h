#ifndef __CORRECTRRINVOICESESSION_H__
#define __CORRECTRRINVOICESESSION_H__

#include "CowPriceInvoiceSession.h"
#include "RRInvoiceContext.h"

class CCorrectRRInvoiceSession : public CowPriceInvoiceSession
{
public:
	CCorrectRRInvoiceSession(RRInvoice *pInvoice,InvoiceContext* pInvoiceContext);   
protected:
	virtual bool CommitInvoice();	
	virtual CString GetSessionTitle() const;		
	virtual void AddItem();
	virtual void DelItem();

	RRInvoice *m_Inv2Edit;	
};

#endif