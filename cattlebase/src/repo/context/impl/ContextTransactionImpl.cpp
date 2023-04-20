#include "stdafx.h"
#include "ContextTransactionImpl.h"



ContextTransactionImpl::ContextTransactionImpl(Context* pOwner) : m_pOwner(pOwner)
{
}

ContextTransactionImpl::~ContextTransactionImpl()
{
}


HentSession* ContextTransactionImpl::CheckoutHent(UINT hentId)
{	
	return NULL;
}

HentSession* ContextTransactionImpl::CreateHent()
{
	return NULL;
}

void ContextTransactionImpl::Commit()
{
}

void ContextTransactionImpl::Rollback()
{
}

void ContextTransactionImpl::Close()
{
}



HentSession* ContextTransactionImpl::FindHentSession(UINT hentId)
{
	HentSession* pSession = NULL;
	NewPtrFlushArray<HentSession>::iterator sessionIt = mHentSessions.begin();
	while(sessionIt != mHentSessions.end())
	{
		pSession = (*sessionIt);
		if(pSession->GetId() == hentId)
			break;
	}
	return pSession;

}
