#include "stdafx.h"
#include "Transaction.h"
#include "SQLSingleConnectionPool.h"
#include "../../exceptions/SQLException.h"

TransactionObject::TransactionObject() : m_pDbConnection(NULL)
{
}

bool TransactionObject::Delete(const char *stmt,bool commit/* = false*/)
{
	return GetConnection()->Delete(stmt,commit);
}

bool TransactionObject::Insert(const char *stmt,bool commit/* = false*/)
{
	return GetConnection()->Insert(stmt,commit);
}

bool TransactionObject::Update(const char *stmt,bool commit/* = false*/)
{
	return GetConnection()->Update(stmt,commit);
}

void TransactionObject::Commit()
{
	return GetConnection()->Commit();
}

void TransactionObject::Rollback()
{
	return GetConnection()->Rollback();
}

DbConnection* TransactionObject::GetConnection()
{
	if(!m_pDbConnection)
	{
		m_pDbConnection = SQLSingleConnectionPool::GetInstance()->GetConnectionFromPool();
		if(!m_pDbConnection)
			throw new CSQLException("Cannot get db connection.");
	}
	return m_pDbConnection;
}

TransactionObject::~TransactionObject()
{
	if(m_pDbConnection)
	{
		SQLSingleConnectionPool::GetInstance()->ReleaseConnectionToPool(m_pDbConnection);
	}
}

	

