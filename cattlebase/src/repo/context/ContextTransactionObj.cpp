#include "stdafx.h"
#include "ContextTransactionObj.h"


ContextTransactionObj::ContextTransactionObj(ContextTransaction* pDelegate) : m_pDelegate(pDelegate)
{
}

ContextTransactionObj::ContextTransactionObj(const ContextTransactionObj& src)
{
	CopyFrom(src);
}

ContextTransactionObj& ContextTransactionObj::operator=(const ContextTransactionObj& right)
{
	CopyFrom(right);
	return *this;
}

HentSession* ContextTransactionObj::CheckoutHent(UINT hentId)
{
	return m_pDelegate->CheckoutHent(hentId);
}

HentSession* ContextTransactionObj::CreateHent()
{
	return m_pDelegate->CreateHent();
}


void ContextTransactionObj::CopyFrom(const ContextTransactionObj& src)
{
	 m_pDelegate = src.m_pDelegate;
}

void ContextTransactionObj::Commit()
{
	m_pDelegate->Commit();
}

void ContextTransactionObj::Rollback()
{
	m_pDelegate->Rollback();
}

void ContextTransactionObj::Close()
{
	m_pDelegate->Close();
}



