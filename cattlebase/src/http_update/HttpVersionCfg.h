#if !defined(__HTTPVERSIONCFG_H__)
#define __HTTPVERSIONCFG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../updaterCfg/prj/include/UpdaterCfgUpdaterCfg.h"

class CHttpVersionCfg
{
private:
	CHttpVersionCfg();
	CString TrimLeftSlash(const CString& slashStr);
	CString TrimRightSlash(const CString& slashStr);
	static CHttpVersionCfg m_instance;
	
	UpdaterCfgUpdaterCfg m_xmlCfg;
	bool m_xmlLoaded;
public:
	static CHttpVersionCfg* GetInstance();	
	CString GetHttpVersionInfoURL();
	CString GetHttpVersionZipURL(const CString& zipFileName);
};


#endif 
