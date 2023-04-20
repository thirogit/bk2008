#include "stdafx.h"
#include "StrException.h"

IMPLEMENT_DYNAMIC(CStrException, CBaseException)

CStrException::CStrException(const CStrException& other)
{
	error = other.error;		 
}

CStrException::CStrException(DWORD nErrorCode)
{
	error = ErrorString(nErrorCode);           
}



CStrException::CStrException(const char *errorFmt,...)
{
	va_list va;
	va_start(va,errorFmt);
	error.FormatV(errorFmt,va);
	va_end(va);
	
}

//***************************************************************************************
CString CStrException::ErrorString(DWORD dwWin32ErrorCode)
{
     CString Error;
     LPTSTR s;
     if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            dwWin32ErrorCode,
            0,
            (LPTSTR)&s,
            0,
            NULL) == 0)
     { /* failed */
     // Unknown error code %08x (%d)  
     Error.Format(IDS_UNKNOWN_ERROR,LOWORD(dwWin32ErrorCode));
    } /* failed */
     else
    { /* success */
     LPTSTR p = _tcschr(s, _T('\r'));
     if(p != NULL)
        { /* lose CRLF */
         *p = _T('\0');
        } /* lose CRLF */
     Error = s;
     ::LocalFree(s);
    } /* success */
    return Error;
	
} 
//***************************************************************************************