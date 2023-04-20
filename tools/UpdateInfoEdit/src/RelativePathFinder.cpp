#include "stdafx.h"
#include "RelativePathFinder.h"
#include <boost/filesystem/path.hpp>

using namespace boost::filesystem; 

bool RelativePathFinder::IsSubPath(const CString& path,const CString &rootPath)
{

}

const CString RelativePathFinder::GetRelativePath(const CString& targetPath,const CString& basePath)
{
	CString _target = targetPath;
	CString _base = basePath;

	path target((LPCSTR)_target,no_check);
	path base((LPCSTR)_base,no_check);

	path::iterator target_it = target.begin();
	path::iterator base_it = base.begin();
	int commonIndex = 0;
	while(target_it != target.end() && base_it != base.end())
	{
		
		std::string target_part = *target_it;
		std::string base_part = *base_it;

		if(_strcmpi(target_part.c_str(),base_part.c_str()) == 0)
		{
			commonIndex++;
		}
		

		target_it++;
		base_it++;
	}
		


        //// find common path
        //String[] target = targetPath.split(pathSeparator);
        //String[] base = basePath.split(pathSeparator);

        //String common = "";
        //int commonIndex = 0;
        //for (int i = 0; i < target.length && i < base.length; i++) {

        //        if (target[i].equals(base[i])) {
        //                common += target[i] + pathSeparator;
        //                commonIndex++;
        //        }
        //}


        //String relative = "";
        //// is the target a child directory of the base directory?
        //// i.e., target = /a/b/c/d, base = /a/b/
        //if (commonIndex == base.length) {
        //        relative = "." + pathSeparator + targetPath.substring(common.length());
        //}
        //else {
        //        // determine how many directories we have to backtrack
        //        for (int i = 1; i <= commonIndex; i++) {
        //                relative += ".." + pathSeparator;
        //        }
        //        relative += targetPath.substring(common.length());
        //}

        //return relative;
	return CString();
}

    

