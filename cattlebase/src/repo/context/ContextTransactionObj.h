#ifndef __CONTEXTTRANSACTIONOBJ_H__
#define __CONTEXTTRANSACTIONOBJ_H__

#include "ContextTransaction.h"

class ContextTransactionObj : public ContextTransaction
{
public:	
	ContextTransactionObj(ContextTransaction* pDelegate);
	ContextTransactionObj(const ContextTransactionObj& src);
	ContextTransactionObj& operator=(const ContextTransactionObj& right);
		
	virtual HentSession* CheckoutHent(UINT hentId);
	virtual HentSession* CreateHent();

	virtual void Commit();
	virtual void Rollback();
	virtual void Close();
protected:
	void CopyFrom(const ContextTransactionObj& src);
private:
	ContextTransaction* m_pDelegate;
};
	

#endif