#ifndef __SYNCEXCEPTION_H__
#define __SYNCEXCEPTION_H__

#include "BaseException.h" 

enum SYNC_EXCEPTION_CODES
{
	INVALIDRECORDFORMAT = 1,
	INVALIDRECORDPREFIX,
	INVALIDFIELD
};


class CSyncException : public CBaseException
{
	DECLARE_DYNAMIC(CSyncException)
public:
	CSyncException(SYNC_EXCEPTION_CODES errorCode,const CString &recordStr);
	SYNC_EXCEPTION_CODES GetSyncErrorCode();
	LPCSTR GetRecordsStr();
	CSyncException(const CSyncException &copy);
private:
	SYNC_EXCEPTION_CODES m_errorCode;
	CString m_recordStr;
};

#endif