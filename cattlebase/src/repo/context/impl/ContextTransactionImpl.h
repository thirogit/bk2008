#ifndef __CONTEXTTRANSACTIONIMPL_H__
#define __CONTEXTTRANSACTIONIMPL_H__


#include "../../NewPtrFlushArray.h"
#include "../ContextTransaction.h"
#include "../Context.h"

class ContextTransactionImpl : public ContextTransaction
{
public:
	ContextTransactionImpl(Context* pOwner);
	~ContextTransactionImpl();
	
	virtual HentSession* CheckoutHent(UINT hentId);
	virtual HentSession* CreateHent();


	virtual void Commit();
	virtual void Rollback();
	virtual void Close();
protected:
	HentSession* FindHentSession(UINT hentId);
	void DiscardSession(HentSession* pSession);

private:
	Context* m_pOwner;

	NewPtrFlushArray<HentSession> mHentSessions;
	
};



#endif