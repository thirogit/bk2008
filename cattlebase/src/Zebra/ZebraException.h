#ifndef __ZEBRAEXCEPTION_H__
#define __ZEBRAEXCEPTION_H__

#include "../exceptions/BaseException.h" 

class ZebraException : public CBaseException
{
	DECLARE_DYNAMIC(ZebraException)
public:
	ZebraException(const CString &errorMsg);
	ZebraException(const ZebraException &src);
};

#endif