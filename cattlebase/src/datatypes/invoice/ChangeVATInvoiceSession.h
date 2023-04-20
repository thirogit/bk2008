#ifndef __CHANGEVATINVOICESESSION_H__
#define __CHANGEVATINVOICESESSION_H__

#include "CowPriceInvoiceSession.h"
#include "InvoiceContext.h"

class CChangeVATInvoiceSession : public CowPriceInvoiceSession
{
public:
	CChangeVATInvoiceSession(VATInvoice *pInvoice,InvoiceContext* pInvoiceContext);   
protected:	
	virtual bool CommitInvoice();	
	virtual CString GetSessionTitle() const;		
	virtual void AddItem();
	virtual void DelItem();

	bool UpdateDocsWithInvoice(CDocDataArray &docs,VATInvoice* pUpdateInv);
	VATInvoice *m_Inv2Edit;	
	CDocDataArray m_docsAdded;
	CDocDataArray m_docsDeleted;
};

#endif