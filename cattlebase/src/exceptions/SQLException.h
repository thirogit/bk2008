#ifndef __SQLEXCEPTION_H__
#define __SQLEXCEPTION_H__

#include "BaseException.h"
#include <odbcinst.h>

class CSQLException : public CBaseException  
{
	DECLARE_DYNAMIC(CSQLException)
public:
	CSQLException(SQLHANDLE handle, int handleType = SQL_HANDLE_STMT);
	CSQLException(const char *errorStr,...);
	CSQLException(const CSQLException& other);
};



#endif