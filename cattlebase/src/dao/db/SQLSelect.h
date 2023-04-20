#ifndef __SQLSELECT_H__
#define __SQLSELECT_H__

#include "../../classes/types/DateTime.h"
#include "../../classes/types/NullDouble.h"
#include <sql.h>
#include "../../DatabasePool\fbserver\DbConnection.h"
#include "Transaction.h"

#define MAX_COLUMNNAME_LEN    64     // Max size of table names, from afxdb.h

class SQLSelect
{
protected:
	struct Column
	{
		char	name [MAX_COLUMNNAME_LEN];
		SWORD	sqlType;
		SQLSMALLINT	nullable;
		int		datalength;					
	
	};
public:
	
	SQLSelect();
	SQLSelect(Transaction transaction);
	~SQLSelect();

	int SelectScalar(const char *stmt,int ifNull = 0,int ifError = -1);
	void Select(const char *stmt);
	
	int numCols();

	int ColIndex(const char* szColName);
    const char* ColName(int nCol);
    
    int getColInt(int nCol, int iNullValue=0);
    int getColInt(const char* szCol, int iNullValue=0);

	CString getColStr(int nCol);

    bool getColStr(int nCol, char* szStr, int &len);
	bool getColStr(int nCol, CString &Str);

	bool getColStr(const char* szCol, char* szStr, int &len);
	bool getColStr(const char* szCol, CString &Str);

	CDateTime getColDateTime(int nCol);
	CDateTime getColDateTime(const char* szCol);

	double getColFloat(int nCol,double fNull = 0.0);
	double getColFloat(const char* szColdouble,double fNull = 0.0);
	
	NullDouble getColNullDouble(const char* szCol);
	NullDouble getColNullDouble(int nCol);

   /* const unsigned char* getColBlob(int nCol, int& nLen);
    const unsigned char* getColBlob(const char* szCol, int& nLen);*/
   
    bool nextRow();
protected:
	void ReadStmt();
	void throwColIndexOutOfBounds(int nCol);
	void throwInvalidColumnName(const char* szColName);
	void throwStmtError();
	void GetStatement(const char *stmt);
	SQLRETURN ExecuteStatement ();
	void FreeStatement ();
	DbConnection* GetConnection();
	
protected:
	Column *columns;
	int numberColumns;
	HSTMT m_statement;
	DbConnection* m_pDbConnection;

};



#endif