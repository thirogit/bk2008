#include "stdafx.h"
#include "SQLException.h"

IMPLEMENT_DYNAMIC(CSQLException, CBaseException)

CSQLException::CSQLException(const CSQLException& other)
{
	error = other.error;		 
}

CSQLException::CSQLException(SQLHANDLE handle, int handleType/*= SQL_HANDLE_STMT*/)
{
		UCHAR	sqlState [6], text [SQL_MAX_MESSAGE_LENGTH];
		SDWORD	nativeCode;
		SWORD	textLength;

		text [0] = 0;
		sqlState [0] = 0;
		sqlState [5] = 0;
		SQLGetDiagRec (handleType, handle, 1, sqlState, &nativeCode, text, sizeof (text) -1, &textLength);
		error.Format("%s: %s",sqlState,text);
           
}

CSQLException::CSQLException(const char *errorStr,...)
{
	va_list va;
	va_start(va,errorStr);
	error.FormatV(errorStr,va);
	va_end(va);
	
}
