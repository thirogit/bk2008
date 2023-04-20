#ifndef __DBCONNECTION_H__
#define __DBCONNECTION_H__

#include <sqltypes.h>

class DbConnection
{
public:
	DbConnection(SQLHANDLE sqlConnectionHandle);
	virtual ~DbConnection();
	bool Delete(const char *stmt,bool commit = false);
	bool Insert(const char *stmt,bool commit = false);
	bool Update(const char *stmt,bool commit = false);
	void Commit();
	void Rollback();
	
	HSTMT GetStatement(const char *stmt);
	void FreeStatement(HSTMT hStmt);
	SQLRETURN ExecuteStatement(HSTMT hStmt);
	SQLHANDLE GetConnectionHandle() const;

private:

	SQLHANDLE m_Connection;
};

#endif