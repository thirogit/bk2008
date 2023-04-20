#ifndef __DATETIMEEXCEPTION_H__
#define __DATETIMEEXCEPTION_H__

#include "BaseException.h"

class CDateTimeException : public CBaseException  
{
	DECLARE_DYNAMIC(CDateTimeException)
public:
	CDateTimeException(const CDateTimeException& other);
	CDateTimeException(LPCTSTR szErrorStr);
};

#endif