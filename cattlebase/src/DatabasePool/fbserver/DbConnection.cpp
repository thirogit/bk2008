#include "stdafx.h"
#include "DbConnection.h"
#include "../../exceptions/SQLException.h"

#include <sql.h>
#include <sqlext.h>



DbConnection::DbConnection(SQLHANDLE sqlConnectionHandle) : m_Connection(sqlConnectionHandle)
{

}

DbConnection::~DbConnection()
{
}

SQLHANDLE DbConnection::GetConnectionHandle() const
{
	return m_Connection;
}

void DbConnection::Commit()
{
	bool commitSucceeded = SQL_SUCCEEDED(SQLEndTran( SQL_HANDLE_DBC, m_Connection, SQL_COMMIT ));

	if(!commitSucceeded)
	{
        throw new CSQLException( m_Connection, SQL_HANDLE_DBC );
	}
	
}


void DbConnection::Rollback()
{
	bool rollbackSucceeded = SQL_SUCCEEDED(SQLEndTran( SQL_HANDLE_DBC, m_Connection, SQL_ROLLBACK ));

	if(!rollbackSucceeded)
	{
		throw new CSQLException( m_Connection, SQL_HANDLE_DBC );
	}
}


bool DbConnection::Delete(const char *stmt,bool commit/* = false*/)
{	
	HSTMT statement = GetStatement(stmt);
	
	SQLRETURN result = ExecuteStatement(statement);
	if(result == SQL_NO_DATA)
	{
		FreeStatement(statement);
		return true;
	}

	if(!SQL_SUCCEEDED(result))
	{
		CSQLException * e = new CSQLException(statement);
		FreeStatement(statement);
		throw e;
	}

	FreeStatement(statement);

	if(commit) 
		Commit();

	return true;
}

bool DbConnection::Insert(const char *stmt,bool commit/* = false*/)
{
	HSTMT statement = GetStatement(stmt);
	
	SQLRETURN ret = ExecuteStatement (statement);
	if(!SQL_SUCCEEDED(ret))
	{	
		CSQLException * e = new CSQLException(statement);
		FreeStatement(statement);
		throw e;
	}

	FreeStatement(statement);

	if(commit) 
		Commit();

	return true;
}

bool DbConnection::Update(const char *stmt,bool commit/* = false*/)
{
	HSTMT statement = GetStatement(stmt);
	
	SQLRETURN ret = ExecuteStatement (statement);
	if(!SQL_SUCCEEDED(ret))
	{	
		CSQLException * e = new CSQLException(statement);
		FreeStatement(statement);
		throw e;
	}

	FreeStatement(statement);

	if(commit) 
		Commit();

	return true;
}



HSTMT DbConnection::GetStatement(const char *stmt)
{
	HSTMT statement = NULL;
	SQLRETURN ret = SQLAllocHandle (SQL_HANDLE_STMT, m_Connection, &statement);
	if(!SQL_SUCCEEDED(ret))
	{	
		throw new CSQLException (m_Connection, SQL_HANDLE_DBC);
	}

	ret = SQLPrepare (statement, (UCHAR*) stmt, SQL_NTS);
	if(!SQL_SUCCEEDED(ret))
	{	
		CSQLException* e = new CSQLException (statement);
		FreeStatement (statement);
	    throw e;
	}
	return statement;
}

void DbConnection::FreeStatement(HSTMT hStmt)
{
	SQLFreeHandle (SQL_HANDLE_STMT, hStmt);
}


SQLRETURN DbConnection::ExecuteStatement(HSTMT hStmt)
{
	return SQLExecute (hStmt);
}
