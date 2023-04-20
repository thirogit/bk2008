#ifndef __RELATIVEPATHFINDER_H__
#define __RELATIVEPATHFINDER_H__

class RelativePathFinder 
{
public:
	static const CString GetRelativePath(const CString& targetPath,const CString& basePath);
	static bool IsSubPath(const CString& path,const CString &rootPath);
};

#endif