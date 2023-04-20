#ifndef __UTIL_H__
#define __UTIL_H__

#include <afx.h>

class CUtil
{
private:
	CUtil();
public:
	static CString GetAppDir();
	static double myatof(LPCSTR szfloatStr);
	static bool IsDots(LPCSTR str);
	static bool DeleteDirectory(LPCSTR sPath);
	static CString ByteSize2Str(ULONGLONG size);
	static bool InCowEANValid(LPCSTR szCowEAN);
	static bool IsFarmNoValid(LPCSTR szFarmNo);
	static bool IsNumber(const CString& str);
	static CString UnPolishString(const CString &aString);
	static bool IsAlpha(const CString& str);
};


#endif