/*Righteous Ninja AKA P.S. Ching codediaries.blogspot.com*/
#ifndef SQLSVRPOOL_H
#define SQLSVRPOOL_H
#include <windows.h>
#include <iostream>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>

#include "..\CommonDatabasePool.h"
#include "DbConnection.h"

class FBSvrPool : public CommonDatabasePool<DbConnection>{
public:
	FBSvrPool();
protected:
	void throwSQLError(const unsigned int, const SQLHANDLE&);
	DbConnection* MyGetConcreteConnection();
	void MyReleaseConcreteConnection(DbConnection*);
	int MyPingConcreteConnection(DbConnection*);
	int MyGetPingTimeout();
	
private:
	SQLHANDLE sqlenvhandle;
	CString keepalivequery;
	unsigned int keepalivetimeout;
	unsigned int hostport;
};

#endif
