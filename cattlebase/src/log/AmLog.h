#pragma once

#include <stdio.h>
#include <windows.h>

#define AMLOG_SETFILENAME(filename) g_AmLog.SetLogFilePath(filename)
#define AMLOG_SETLOGLEVEL_NOTHING g_AmLog.SetLogLevel(CAmLoglevelNothing)

#define AMLOG_SETLOGLEVEL_ERROR g_AmLog.SetLogLevel(CAmLoglevelError)
#define AMLOG_SETLOGLEVEL_INFO g_AmLog.SetLogLevel(CAmLoglevelInfo)
#define AMLOG_SETLOGLEVEL_DEVELOPERINFO g_AmLog.SetLogLevel(CAmLoglevelDeveloperInfo)

#define AMLOGINFO g_AmLog.Lock();\
	g_AmLog.SetLineNumber(__LINE__);\
	g_AmLog.SetSourceFileName(__FILE__);\
	g_AmLog.LogInfo

#define AMLOGERROR g_AmLog.Lock();\
	g_AmLog.SetLineNumber(__LINE__);\
	g_AmLog.SetSourceFileName(__FILE__);\
	g_AmLog.LogError

#define AMLOGDEVINFO g_AmLog.Lock();\
	g_AmLog.SetLineNumber(__LINE__);\
	g_AmLog.SetSourceFileName(__FILE__);\
	g_AmLog.LogDevInfo

typedef enum CAmLogLevels
{
	CAmLoglevelNothing,
	CAmLoglevelError,
	CAmLoglevelInfo,
	CAmLoglevelDeveloperInfo
}CAmLogLevels;

class CAmLog
{
protected:
	CRITICAL_SECTION m_cs;
	FILE *m_File;
	TCHAR m_FileName[MAX_PATH];
	CAmLogLevels m_LogLevel;
	char m_SourceFile[MAX_PATH];
	int m_LineNumber;
	void LogNow(TCHAR *LoglevelName, TCHAR *LogString);
	void ReplaceCRLF(TCHAR *s);
public:
	CAmLog();
	~CAmLog();
	void Lock() {EnterCriticalSection(&m_cs);};
	void SetLineNumber(int line) {m_LineNumber = line;};
	void SetSourceFileName(char *filename);
	bool SetLogFilePath(LPCSTR szFilePath);
	void SetLogLevel(CAmLogLevels LogLevel);
	void LogError(TCHAR *format, ...);
	void LogInfo(TCHAR *format, ...);
	void LogDevInfo(TCHAR *format, ...);
};

extern CAmLog g_AmLog;










