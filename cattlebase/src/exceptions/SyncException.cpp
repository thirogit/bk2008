#include "stdafx.h"
#include "SyncException.h"

IMPLEMENT_DYNAMIC(CSyncException, CBaseException)


CSyncException::CSyncException(SYNC_EXCEPTION_CODES errorCode,const CString &recordStr)
									: m_errorCode(errorCode),m_recordStr(recordStr)
{
}
CSyncException::CSyncException(const CSyncException &copy)
									: m_errorCode(copy.m_errorCode), m_recordStr(copy.m_recordStr)
{

}

SYNC_EXCEPTION_CODES CSyncException::GetSyncErrorCode()
{ 
	return m_errorCode; 
}
LPCSTR CSyncException::GetRecordsStr()
{ 
	return (LPCSTR)m_recordStr; 
}
