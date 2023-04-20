#include "stdafx.h"
#include "ZebraException.h"

IMPLEMENT_DYNAMIC(ZebraException, CBaseException)


ZebraException::ZebraException(const CString &errorMsg) 
{
	error = errorMsg;
}

ZebraException::ZebraException(const ZebraException &src)
{
	error = src.error;
}