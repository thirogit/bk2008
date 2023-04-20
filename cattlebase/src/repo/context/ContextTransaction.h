#ifndef __CONTEXTTRANSACTION_H__
#define __CONTEXTTRANSACTION_H__

#include "HentSession.h"

class ContextTransaction
{
public:	
	
	virtual HentSession* CheckoutHent(UINT hentId) = 0;
	virtual HentSession* CreateHent() = 0;

	virtual void Commit() = 0;
	virtual void Rollback() = 0;
	virtual void Close() = 0;
};
	

#endif