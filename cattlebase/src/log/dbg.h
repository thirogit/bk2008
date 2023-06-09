/*
 * Copyright (c) 1998-2000 by KVA Software
 * All rights reserved.	
 *
 * File:      dbg.h
 * Contents:  CDebugPrintf class definition
 * Author:    Vladimir Kirienko
 *
 *            http://www.uran.net/kva
 *            kirienko@geocities.com
 */
#ifndef ______DBG_H______
#define ______DBG_H______


//#define USE_STL
#undef  USE_STL

//#define USE_MFC
#undef  USE_MFC


#undef USE_DEBUG_PRINTF

#if defined(_DEBUG) || defined(DEBUG_PRINTF)
#define USE_DEBUG_PRINTF
#endif




#ifdef USE_DEBUG_PRINTF

#ifdef USE_STL
#include <string>
using namespace std;
#endif


/*
 *	Macro definitions
 *
 */
#define IFDEBUG( doit )	doit

#define PRINTF			debug.printf
#define SHOW_CONSOLE	debug.ShowConsole();
#define HIDE_CONSOLE	debug.HideConsole();
#define SHOW_LASTERROR	debug.ShowLastError();
#define CLOSE_LOG		debug.CloseLog();


/*-----------------------------------------------------------------------
 *  Class        : CDebugPrintf
 *  Prototype    : class CDebugPrintf
 *  Description  : 
 *  Parent class : 
 *  History      : 
 */
class CDebugPrintf
{
public:
	CDebugPrintf();
	virtual ~CDebugPrintf();

	void printf( const char *fmt, ... );

#ifdef USE_STL
	void printf( string& str ) { printf( str.c_str() ); }
	void printf( string* str ) { printf( str->c_str() ); }
#endif

#ifdef USE_MFC
	void printf( CString& str ) { printf( str.GetBuffer(1024) ); }
	void printf( CString* str ) { printf( str->GetBuffer(1024) ); }
#endif

	void CloseLog();

	void ShowConsole(); 
	void HideConsole(); 
	void ShowLastError();

private:
	HANDLE			 m_hFile;		// log-file handle
	HANDLE			 m_hDebugMutex;	// allow to check that log-file is open by other process 
	CRITICAL_SECTION m_hLock;		// lock for thread-safe access

	DWORD			 m_dwStartTime; // application start time 
	bool			 m_bUseConsole; // use or not log-console with log-file
};

extern CDebugPrintf debug;



#else

#define IFDEBUG( doit )	

#define SLASH /
#define COMMENT ;SLASH/

#define PRINTF			COMMENT
#define SHOW_CONSOLE	COMMENT
#define HIDE_CONSOLE	COMMENT
#define SHOW_LASTERROR	COMMENT
#define CLOSE_LOG		COMMENT

#endif


#endif


