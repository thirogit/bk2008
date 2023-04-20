#include "stdafx.h"
#include "FBSvrPool.h"
#include "../../exceptions/SQLException.h"
#include "../../missing/util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace std;

const char DB_FILE[] = "CATTLE.FDB";
const char DB_LOGIN[] = "SYSDBA";
const char DB_PASSWORD[]  = "masterkey";
const char DB_CLIENT[] = "fb\\fbembed.dll";


FBSvrPool::FBSvrPool()
{
	this->keepalivequery="select 0 from RDB$DATABASE";
	this->keepalivetimeout=300000;

	if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle))
	{
		throwSQLError(SQL_HANDLE_ENV, sqlenvhandle);
	}

	if(SQL_SUCCESS!=SQLSetEnvAttr(sqlenvhandle,SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
	{ 
		throwSQLError(SQL_HANDLE_ENV, sqlenvhandle);
	}
}

DbConnection* FBSvrPool::MyGetConcreteConnection()
{

	SQLHANDLE sqlconnectionhandle;

	if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle))
	{
		throwSQLError( SQL_HANDLE_DBC, sqlconnectionhandle);
		return 0;
	}

	char sqlstring[1024];

	CString sAppDir = CUtil::GetAppDir();

	sprintf_s(sqlstring,sizeof(sqlstring),
		"DRIVER=Firebird/InterBase(r) driver;UID=%s;PWD=%s;CLIENT=%s\\%s;DBNAME=%s\\%s", 
		DB_LOGIN, DB_PASSWORD,(LPCSTR)sAppDir,DB_CLIENT,(LPCSTR)sAppDir,DB_FILE);
	
	try
	{
		SQLCHAR retconstring[1024];
		switch(SQLDriverConnect (sqlconnectionhandle, 
				NULL, 
				(SQLCHAR*)sqlstring,
				SQL_NTS, 
				retconstring, 
				1024, 
				NULL,
				SQL_DRIVER_NOPROMPT
				))
		{
			case SQL_INVALID_HANDLE:
			case SQL_ERROR:
				throwSQLError(SQL_HANDLE_DBC, sqlconnectionhandle);
				return 0;
			case SQL_SUCCESS_WITH_INFO:
				throwSQLError( SQL_HANDLE_DBC, sqlconnectionhandle);
			default:
				return new DbConnection(sqlconnectionhandle);
		}
	}
	catch(CSQLException* e)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
		throw e;
	}
}

void FBSvrPool::MyReleaseConcreteConnection(DbConnection* dbConnection)
{
	SQLHANDLE sqlconnectionhandle = dbConnection->GetConnectionHandle();
	SQLDisconnect(sqlconnectionhandle);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
	delete dbConnection;
}

int FBSvrPool::MyPingConcreteConnection(DbConnection* dbConnection)
{
	SQLHANDLE sqlconnectionhandle = dbConnection->GetConnectionHandle();
	SQLHANDLE sqlstatementhandle;
	if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle)){
		throwSQLError(SQL_HANDLE_STMT, sqlstatementhandle);
		return -1;
	}
	if(SQL_SUCCESS!=SQLExecDirect(sqlstatementhandle, (SQLCHAR*)(LPCSTR)keepalivequery, SQL_NTS)){
		throwSQLError( SQL_HANDLE_STMT, sqlstatementhandle);
		SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle );
		return -1;
	}
	SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle );
	return 0;
}

int FBSvrPool::MyGetPingTimeout(){
	return keepalivetimeout;
}

void FBSvrPool::throwSQLError(const unsigned int handletype, const SQLHANDLE& handle)
{
	CString exceptionMsg;
	SQLCHAR sqlstate[1024];
	SQLCHAR sqlMessage[1024];
	if(SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, sqlMessage, 1024, NULL))
	{
		exceptionMsg += "Message: ";
		exceptionMsg += (char*)sqlMessage;
		exceptionMsg += "\nSQLSTATE: ";
		exceptionMsg += (char*)sqlstate;
	}
	else
		exceptionMsg += "Could not get error details";

	throw new CSQLException((LPCSTR)exceptionMsg);
}