#pragma once

class CPathUtil
{
public:
	static CString GetRelativePath(const CString& strPath, const CString& strReference);
	static bool IsSubPath(const CString& strPath, const CString& basePath);
};