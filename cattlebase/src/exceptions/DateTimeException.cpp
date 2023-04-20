#include "stdafx.h"
#include "DateTimeException.h"

IMPLEMENT_DYNAMIC(CDateTimeException, CBaseException)

CDateTimeException::CDateTimeException(const CDateTimeException& other)
{	
	error = other.error;		 
}

CDateTimeException::CDateTimeException(LPCTSTR szErrorStr)
{
	error = szErrorStr;		 
}



