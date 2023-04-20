#include "stdafx.h"
#include "RecognitionOperationResult.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


RecognitionOperationResult::RecognitionOperationResult(RecognitionOperationStatus status) : m_Status(status)
{

}
RecognitionOperationResult::RecognitionOperationResult(RecognitionOperationStatus status,const CString& message) : m_Status(status),m_Message(message)
{
}

RecognitionOperationResult::RecognitionOperationResult(const RecognitionOperationResult& src)
{
	CopyFrom(src);
}

RecognitionOperationResult& RecognitionOperationResult::operator=(const RecognitionOperationResult& right)
{
	CopyFrom(right);
	return *this;
}

void RecognitionOperationResult::CopyFrom(const RecognitionOperationResult& src)
{
	m_Status = src.m_Status;
	m_Message = src.m_Message;
}

const CString& RecognitionOperationResult::GetOperationMessage() const
{
	return m_Message;
}

RecognitionOperationStatus RecognitionOperationResult::GetStatus() const
{
	return m_Status;
}

