#ifndef __CORRECTVATINVOICESESSION_H__
#define __CORRECTVATINVOICESESSION_H__

#include "CowPriceInvoiceSession.h"
#include "InvoiceContext.h"

class CCorrectVATInvoiceSession : public CowPriceInvoiceSession
{
public:
	CCorrectVATInvoiceSession(VATInvoice *pInvoice,InvoiceContext* pInvoiceContext);   
protected:
	virtual bool CommitInvoice();	
	virtual CString GetSessionTitle() const;		
	virtual void AddItem();
	virtual void DelItem();

	VATInvoice *m_Inv2Edit;
	


};

#endif