#include "stdafx.h"
#include "StrException.h"

IMPLEMENT_DYNAMIC(CStrException, CException)

CStrException::CStrException(const CStrException& other)
{
	errorStr = other.errorStr;		 
}

CStrException::CStrException(DWORD nErrorCode)
{
	errorStr = ErrorString(nErrorCode);           
}

CStrException::~CStrException() 
{
}

CStrException::CStrException(const char *errorFmt,...)
{
	va_list va;
	va_start(va,errorFmt);
	errorStr.FormatV(errorFmt,va);
	va_end(va);
	
}

//***************************************************************************************
CString CStrException::ErrorString(DWORD err)
{
     CString Error;
     LPTSTR s;
     if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            err,
            0,
            (LPTSTR)&s,
            0,
            NULL) == 0)
     { /* failed */
     // Unknown error code %08x (%d)
     CString fmt;
     CString t;
     fmt.LoadString(IDS_UNKNOWN_ERROR);
     t.Format(fmt, err, LOWORD(err));
     Error = t;
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
	return CString("");
} // ErrorString
//***************************************************************************************