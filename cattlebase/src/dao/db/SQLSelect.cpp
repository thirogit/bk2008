#include "stdafx.h"
#include "SQLSelect.h"
#include "../../exceptions/SQLException.h"
#include "SQLSingleConnectionPool.h"
#include <sqlext.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SQLSelect::SQLSelect(Transaction transaction) : columns(NULL),numberColumns(0),m_statement(NULL)
{
	m_pDbConnection = transaction->GetConnection();
}

SQLSelect::SQLSelect() : columns(NULL),numberColumns(0),m_statement(NULL),m_pDbConnection(NULL)
{
	

}

DbConnection* SQLSelect::GetConnection()
{
	if(!m_pDbConnection)
	{
		m_pDbConnection = SQLSingleConnectionPool::GetInstance()->GetConnectionFromPool();
		if(!m_pDbConnection)
			throw new CSQLException("Cannot get db connection.");
	}
	return m_pDbConnection;
}

int  SQLSelect::SelectScalar(const char *stmt,int ifNull/* = 0*/,int ifError/* = -1*/)
{
	SQLSelect res;
	int iRet = ifNull;
	try
	{
		res.Select(stmt);
		if(!res.nextRow()) return ifNull;
		iRet = res.getColInt(0,ifNull);
	} 
	catch(CSQLException *er)
	{
		er->Delete();
		return ifError;
	}
	return iRet;
}

void SQLSelect::GetStatement(const char *stmt)
{
	if(m_statement != NULL)
		FreeStatement();

	DbConnection* pDbConnection = GetConnection();

	m_statement = pDbConnection->GetStatement(stmt);
	
}

SQLRETURN SQLSelect::ExecuteStatement ()
{
	DbConnection* pDbConnection = GetConnection();
	return pDbConnection->ExecuteStatement(m_statement);
}

void SQLSelect::FreeStatement ()
{
	DbConnection* pDbConnection = GetConnection();
	pDbConnection->FreeStatement(m_statement);
	m_statement = NULL;
}

void SQLSelect::throwStmtError()
{
	throw new CSQLException(m_statement);
}

void SQLSelect::Select(const char *stmt)
{
	GetStatement(stmt);
	
	SQLRETURN ret = ExecuteStatement ();
	if(!SQL_SUCCEEDED(ret))
	{	
		throwStmtError();
	}
	ReadStmt();
}

void SQLSelect::ReadStmt()
{
	SWORD	count;
	RETCODE ret = SQLNumResultCols (m_statement, &count);
	if(!SQL_SUCCEEDED(ret))
		throwStmtError();
		
	numberColumns = count;
	columns = new Column [numberColumns];
	Column *column = columns;

	SWORD nameLength;
	SQLUINTEGER dataLen;
	SQLSMALLINT decLen;


	for (int n = 1; n <= numberColumns; ++n, ++column)
		{
		
		ret = SQLDescribeCol (m_statement, n, 
					  (UCHAR*) column->name, sizeof (column->name), &nameLength,
					  &column->sqlType, 
					  &dataLen, 
					  &decLen, 
					  &column->nullable);
		if(!SQL_SUCCEEDED(ret))
			throwStmtError();

		//if(column->sqlType == SQL_DECIMAL || column->sqlType == SQL_NUMERIC)
			column->datalength = dataLen+decLen;
		//else column->datalength = dataLen;
		
		}

		//nextRow();
}
//***************************************************************************************
SQLSelect::~SQLSelect()
{
	if(columns)
		delete[] columns;
	FreeStatement();

	if(m_pDbConnection)
	{
		SQLSingleConnectionPool::GetInstance()->ReleaseConnectionToPool(m_pDbConnection);
	}
}	
//***************************************************************************************
int SQLSelect::numCols()
{
	return numberColumns;
}
//***************************************************************************************
 bool SQLSelect::nextRow()
 {
    if(!m_statement) throw new CSQLException("Object not initialized");
	RETCODE ret = SQLFetch (m_statement);
	if(ret == SQL_NO_DATA_FOUND) return false;

	if(!SQL_SUCCEEDED(ret))
		throwStmtError();
	return true;
 }
 //**************************************************************************************
int SQLSelect::ColIndex(const char* szColName)
{
	for(int i = 0;i< numberColumns;i++)
		if(!_strcmpi(szColName,columns[i].name))
			return (i);

	ASSERT(0 && "No column");

	throw new CSQLException("Invalid column: %s",szColName);
	
}
//***************************************************************************************
const char* SQLSelect::ColName(int nCol)
{
	if(nCol >= 0 && nCol < numberColumns)
		return columns[nCol].name;

	return NULL;
}
//***************************************************************************************
int SQLSelect::getColInt(int nCol, int iNullValue)
{
	throwColIndexOutOfBounds(nCol);
	SQLINTEGER result,Ind;
	
	int ret = SQLGetData (m_statement, nCol+1, SQL_C_LONG , &result,sizeof(result), &Ind);
	if(!SQL_SUCCEEDED(ret))
			throwStmtError();
	if(Ind == SQL_NULL_DATA) return iNullValue;
		
	return result;
}

void SQLSelect::throwInvalidColumnName(const char* szColName)
{
	throw new CSQLException("Unknown column '%s' in field list",szColName);
}

//***************************************************************************************
int SQLSelect::getColInt(const char* szCol, int iNullValue)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) throwInvalidColumnName(szCol);
	
	return getColInt(colIdx,iNullValue);
}
//***************************************************************************************
CString SQLSelect::getColStr(int nCol)
{
	CString colString;
	getColStr(nCol,colString);
	return colString;
}
//***************************************************************************************
bool SQLSelect::getColStr(int nCol, char* szStr, int &len)
{
	throwColIndexOutOfBounds(nCol);

	if(!szStr) throw new CSQLException("Null string pointer");

	SQLINTEGER Ind;
	
	
	int ret = SQLGetData (m_statement, nCol+1, SQL_C_CHAR , szStr,len, &Ind);
	if(!SQL_SUCCEEDED(ret))
			throwStmtError();
	
	if(Ind == SQL_NULL_DATA) 
	{
		len = 0;
		return true;
	}
	else len = Ind;


	return false;
}
void SQLSelect::throwColIndexOutOfBounds(int nCol)
{
	if(nCol  < 0 && nCol >= numberColumns)
		throw new CSQLException("Column index out of bounds");
}

//***************************************************************************************
bool SQLSelect::getColStr(int nCol, CString &Str)
{	
	throwColIndexOutOfBounds(nCol);

	int bufflen = columns[nCol].datalength+1;
	bool ret;
	char * buffer = new char[bufflen];
	ret = getColStr(nCol,buffer,bufflen);
	buffer[bufflen] = 0;
	Str = buffer;
	delete[] buffer;
	return ret;
}
//***************************************************************************************
bool SQLSelect::getColStr(const char* szCol, char* szStr, int &len)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) 	throwInvalidColumnName(szCol);

	return getColStr(colIdx,szStr,len);
}
//***************************************************************************************
bool SQLSelect::getColStr(const char* szCol, CString &Str)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) 	throwInvalidColumnName(szCol);
	return getColStr(colIdx,Str);

}
//***************************************************************************************
CDateTime SQLSelect::getColDateTime(int nCol)
{
	SQL_TIMESTAMP_STRUCT StatusTime;
	SQLINTEGER nullindicator;
    SQLGetData(m_statement, nCol+1, SQL_C_TYPE_TIMESTAMP,&StatusTime, sizeof(StatusTime), &nullindicator);

	if(nullindicator != SQL_NULL_DATA)
	{
		return CDateTime(StatusTime.year,StatusTime.month,StatusTime.day,StatusTime.hour,StatusTime.minute,StatusTime.second);
	}
	else
		return CDateTime();	
	
}
//***************************************************************************************
CDateTime SQLSelect::getColDateTime(const char* szCol)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) 	throwInvalidColumnName(szCol);
	return getColDateTime(colIdx);
}

NullDouble SQLSelect::getColNullDouble(const char* szCol)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) 	throwInvalidColumnName(szCol);
	return getColNullDouble(colIdx);
}

NullDouble SQLSelect::getColNullDouble(int nCol)
{
	SQLINTEGER nullindicator;
	 double dRet;
    SQLGetData(m_statement, nCol+1, SQL_DOUBLE,&dRet, sizeof(dRet), &nullindicator);
	return nullindicator != SQL_NULL_DATA ? NullDouble(dRet) : NullDouble();
}


double SQLSelect::getColFloat(int nCol,double fNull/* = 0.0*/)
{
	 SQLINTEGER nullindicator;
	 double dRet;
    SQLGetData(m_statement, nCol+1, SQL_DOUBLE,&dRet, sizeof(dRet), &nullindicator);
	return nullindicator != SQL_NULL_DATA ? dRet : fNull;


}
double SQLSelect::getColFloat(const char* szCol,double fNull/* = 0.0*/)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) 	throwInvalidColumnName(szCol);
	return getColFloat(colIdx);
}



