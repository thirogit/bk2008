#include "StdAfx.h"
#include "HttpVersionCfg.h"
#include "../missing/util.h"
#include "../missing/paths.h"


CHttpVersionCfg CHttpVersionCfg::m_instance;

CHttpVersionCfg::CHttpVersionCfg() : m_xmlLoaded(false)
{
}

CHttpVersionCfg* CHttpVersionCfg::GetInstance()
{
	if(!m_instance.m_xmlLoaded)
	{
		cmString xmlFilePath = paths_ConcatPath(CUtil::GetAppDir(),CString("updatecfg.xml"));

		//m_instance.m_xmlCfg.SetBaseURL("http://localhost:8000/");
		//m_instance.m_xmlCfg.SetVersionInfoFileName("version.info");
		//m_instance.m_xmlCfg.SetZipsDirName("zips/");

		//m_instance.m_xmlCfg.SaveToXmlFile(xmlFilePath);
		m_instance.m_xmlLoaded = m_instance.m_xmlCfg.LoadFromXmlFile(xmlFilePath);

		if(!m_instance.m_xmlLoaded)
			return NULL;
	}
	return &m_instance;
}
CString CHttpVersionCfg::GetHttpVersionInfoURL()
{
	CString baseURL = TrimRightSlash(m_xmlCfg.GetBaseURL());
	CString versionInfoFile = TrimRightSlash(m_xmlCfg.GetVersionInfoFileName());

	return baseURL + "/" + versionInfoFile;
}
CString CHttpVersionCfg::GetHttpVersionZipURL(const CString& zipFileName)
{
	CString baseURL = TrimRightSlash(m_xmlCfg.GetBaseURL());
	CString zipsDirName = TrimLeftSlash(TrimRightSlash(m_xmlCfg.GetZipsDirName()));
	
	return baseURL + "/" + zipsDirName + "/" + TrimRightSlash(zipFileName);
}

CString CHttpVersionCfg::TrimLeftSlash(const CString& slashStr)
{
	return CString(slashStr).TrimLeft('/');

}
CString CHttpVersionCfg::TrimRightSlash(const CString& slashStr)
{
	return CString(slashStr).TrimRight('/');
}