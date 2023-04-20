#include "stdafx.h"
#include "amlog.h"
#include <time.h>
#include <crtdbg.h>

CAmLog g_AmLog;

CAmLog::CAmLog()
{
	m_FileName[0] = '\0';
	m_LogLevel = CAmLoglevelNothing;
	m_File = NULL;
	InitializeCriticalSection(&m_cs);
}

CAmLog::~CAmLog()
{
	if (m_File != NULL)
	{
		fclose(m_File);
	}
	DeleteCriticalSection(&m_cs);
}

bool CAmLog::SetLogFilePath(LPCSTR szFilePath)
{
	if (m_File != NULL)
	{
		fclose(m_File);
	}
	strcpy(m_FileName,szFilePath);
	//open the logfile...
	m_File = _tfopen(m_FileName, _T("ab"));
	_ASSERTE(m_File != NULL);
	if (m_File == NULL)
	{
		return false;
	}

	return true;
}

void CAmLog::SetSourceFileName(char *filename)
{
	//strip the path from the filename...
	char *mid = filename + strlen(filename);
	while (mid > filename)
	{
		if (*(--mid) == '\\')
		{
			mid++;
			break;
		}
	}
	//store the filename...
	strcpy(m_SourceFile, mid);
}

void CAmLog::SetLogLevel(CAmLogLevels LogLevel)
{
	m_LogLevel = LogLevel;
}

void CAmLog::LogNow(TCHAR *LoglevelName, TCHAR *LogString)
{
	if (m_File == NULL)
	{
		_ASSERTE(!"Filename is not set...");
		return;
	}

	//get the current date and time, and format it to the format we wanna use...
	time_t now;
	time(&now);
	struct tm *tmnow = localtime(&now);
	char strnow[25];
	strftime(strnow, 24, "%Y-%m-%d %H:%M:%S", tmnow);

#ifdef _UNICODE
	if (m_LogLevel == CAmLoglevelDeveloperInfo)
	{
		fprintf(m_File, "%s\t%S\t%s, %d\t%S\r\n", strnow, LoglevelName, m_SourceFile, m_LineNumber, LogString);
	}
	else
	{
		fprintf(m_File, "%s\t%S\t%S\r\n", strnow, LoglevelName, LogString);
	}
#else
	if (m_LogLevel == CAmLoglevelDeveloperInfo)
	{
		fprintf(m_File, "%s\t%s\t%s, %d\t%s\r\n", strnow, LoglevelName, m_SourceFile, m_LineNumber, LogString);
	}
	else
	{
		fprintf(m_File, "%s\t%s\t%s\r\n", strnow, LoglevelName, LogString);
	}
#endif
#ifdef AMLOG_TRACE
	TCHAR mid[1025] = {0};
	wsprintf(mid, _T("%s\r\n"), LogString);
	OutputDebugString(mid);
#endif
#ifdef AMLOG_STDOUT
	TCHAR mid2[1025] = {0};
	wsprintf(mid2, _T("%s\r\n"), LogString);
	wprintf(mid2);
#endif

	fflush(m_File);
}

void CAmLog::ReplaceCRLF(TCHAR *s)
{
	TCHAR *mid = s;
	while (*mid != '\0')
	{
		switch (*mid)
		{
		case '\r':
			*mid = '|';
			break;
		case '\n':
			*mid = '|';
			break;
		}
		mid++;
	}
}

void CAmLog::LogInfo(TCHAR *format, ...)
{
	if (m_LogLevel == CAmLoglevelDeveloperInfo || m_LogLevel == CAmLoglevelInfo)
	{
		//never corrupt the last error value...
		DWORD LastError = GetLastError();
		//do the actual logging...
		TCHAR mid[1025] = {0}; //the wvsprintf function never puts more than 1024 bytes in a string...
		va_list args;
		va_start(args, format);
		wvsprintf(mid, format, args);
		ReplaceCRLF(mid);
		LogNow(_T("Info"), mid);
		va_end(args);
		SetLastError(LastError);
	}
	LeaveCriticalSection(&m_cs);
}

void CAmLog::LogError(TCHAR *format, ...)
{
	if (m_LogLevel == CAmLoglevelDeveloperInfo || m_LogLevel == CAmLoglevelInfo || m_LogLevel == CAmLoglevelError)
	{
		//never corrupt the last error value...
		DWORD LastError = GetLastError();
		//do the actual logging...
		TCHAR mid[1025] = {0}; //the wvsprintf function never puts more than 1024 bytes in a string...
		va_list args;
		va_start(args, format);
		wvsprintf(mid, format, args);
		ReplaceCRLF(mid);
		LogNow(_T("Error"), mid);
		va_end(args);
		SetLastError(LastError);
	}
	LeaveCriticalSection(&m_cs);
}

void CAmLog::LogDevInfo(TCHAR *format, ...)
{
	if (m_LogLevel == CAmLoglevelDeveloperInfo)
	{
		//never corrupt the last error value...
		DWORD LastError = GetLastError();
		//do the actual logging...
		TCHAR mid[1025] = {0}; //the wvsprintf function never puts more than 1024 bytes in a string...
		va_list args;
		va_start(args, format);
		wvsprintf(mid, format, args);
		ReplaceCRLF(mid);
		LogNow(_T("DevInfo"), mid);
		va_end(args);
		SetLastError(LastError);
	}
	LeaveCriticalSection(&m_cs);
}



















