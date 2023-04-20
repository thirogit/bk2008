///////////////////////////////////////////////////////////////////////////////
//
//		CTraceFile
//		----------
//		Implements ITrace as logging to file
//
////Holger Kloos, 2003/////////////////////////////////////////////////////////


#include "stdafx.h"
#include "TraceFile.h"


//  CTraceFile
//	----------


CTraceFile::CTraceFile()
{
	m_bEnabled = FALSE;
}

CTraceFile::~CTraceFile()
{
	if (m_bEnabled)
		m_File.Close();

	m_bEnabled = FALSE;
}

BOOL CTraceFile::OpenTraceFile(const char* sFileName)
{
	m_bEnabled = m_File.Open(sFileName, CFile::modeCreate | CFile::modeWrite); 
	return m_bEnabled;
}

void CTraceFile::MakeTrace(const char* sTrace, ...)
{
	CString str;

	va_list argList;
	va_start(argList, sTrace);
	str.FormatV(sTrace, argList);
	va_end(argList);

	str += "\r\n";

	if (m_bEnabled)
		m_File.Write((LPCTSTR)str, str.GetLength());

	TRACE(str);
}
