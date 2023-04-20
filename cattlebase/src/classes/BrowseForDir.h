#ifndef __BROSEFORDIR_H__
#define __BROSEFORDIR_H__

class CBrowseForDir
{
public:
	static bool BrowseForDir(CString &strDirPath, const char* szCaption = NULL, CWnd* hOwner = NULL);
};

#endif