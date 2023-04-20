#ifndef __STREXCEPTION_H__
#define __STREXCEPTION_H__

#include "BaseException.h"

class CStrException : public CBaseException  
{
	DECLARE_DYNAMIC(CStrException)
public:
	
	CStrException(DWORD nErrorCode);
	CStrException(const char *errorFmt,...);
	CStrException(const CStrException& other);
	static CString ErrorString(DWORD dwWin32ErrorCode);

};

#endif