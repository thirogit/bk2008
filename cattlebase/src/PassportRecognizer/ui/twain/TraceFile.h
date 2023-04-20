///////////////////////////////////////////////////////////////////////////////
//
//		CTraceFile
//		----------
//		Implements ITrace as logging to file
//
////Holger Kloos, 2003/////////////////////////////////////////////////////////


#ifndef __TRACEFILE_H__
#define __TRACEFILE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tracer.h"


class CTraceFile : public ITracer
{
public:
	CTraceFile();
	virtual ~CTraceFile();

	BOOL OpenTraceFile(const char* sFileName);
	virtual void MakeTrace(const char* sTrace, ...);

private:
	CFile	m_File;
	BOOL	m_bEnabled;
};

#endif
