#include "stdafx.h"
#include "RecognitionException.h"

IMPLEMENT_DYNAMIC(RecognitionException, CException)

RecognitionException::RecognitionException(const RecognitionException& src)
{
	m_sError = src.m_sError;
}

RecognitionException::RecognitionException(const CString& sError) : m_sError(sError)
{
}

const CString& RecognitionException::GetError() const
{
	return m_sError;
}
