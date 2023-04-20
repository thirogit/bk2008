#include "stdafx.h"
#include "CrashDumpHandler.h"
#include "missing/util.h"
#include "missing/paths.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <windows.h>
#include <dbghelp.h>
#include "VersionNo.h"
#include "log.h"

#pragma comment (lib,"dbghelp.lib")

LONG WINAPI CustomUnhandledExceptionFilter(PEXCEPTION_POINTERS pExInfo)
{
   if (EXCEPTION_BREAKPOINT == pExInfo->ExceptionRecord->ExceptionCode)
   {
	   // Breakpoint. Don't treat this as a normal crash.
	   return EXCEPTION_CONTINUE_SEARCH;
   }


   CString dmpDestDir =  paths_ConcatPath(CUtil::GetAppDir(),"CRASH");
   paths_CreateIfNeeded(dmpDestDir);

   CString dmpPath,dmpFileName;

   int i = 0;
   do
   {
		if(i > 0)
		{
			dmpFileName.Format("crash_%d_%d.dmp",VERSION_EXTEND,i);			
		}
		else
			dmpFileName.Format("crash_%d.dmp",VERSION_EXTEND);

		dmpPath = paths_ConcatPath(dmpDestDir,dmpFileName);
			
		i++;
   }
   while(boost::filesystem::exists(boost::filesystem::path((LPCSTR)dmpPath)));

    
   // Create a file for the minidump
   HANDLE hFile = CreateFile(
      (LPCSTR)dmpPath,
      GENERIC_WRITE,
      0,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL);

   // Now write the dump to file
   MINIDUMP_EXCEPTION_INFORMATION eInfo;
   eInfo.ThreadId = GetCurrentThreadId();
   eInfo.ExceptionPointers = pExInfo;
   eInfo.ClientPointers = FALSE;

   MiniDumpWriteDump(
      GetCurrentProcess(),
      GetCurrentProcessId(),
      hFile,
      MiniDumpWithIndirectlyReferencedMemory,
      &eInfo,
	  NULL,
      NULL);

   CloseHandle(hFile);

   LOG("Saving crash dump file: %s",(LPCSTR)dmpPath);
   
 //  CString errorMsg;
  // errorMsg.Format(IDS_UNHANDELED_EXCEPTION_MSG,(LPCSTR)dmpFileName);
   //AfxMessageBox(errorMsg);

   return EXCEPTION_CONTINUE_SEARCH;
}

CCrashDumpHandler::CCrashDumpHandler()
{
	SetUnhandledExceptionFilter(CustomUnhandledExceptionFilter);
	SetErrorMode(SEM_FAILCRITICALERRORS);
}

CCrashDumpHandler::~CCrashDumpHandler()
{
}

