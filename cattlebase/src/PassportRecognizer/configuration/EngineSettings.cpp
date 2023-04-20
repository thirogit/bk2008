#include "stdafx.h"
#include "EngineSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


EngineSettings::EngineSettings()
{

}
	
const CString& EngineSettings::GetEnginePath() const
{
	return m_EnginePath;
}

const CString& EngineSettings::GetLicenseFilePath() const
{
	return m_LicenseFilePath;
}

const CString& EngineSettings::GetDeveloperSN() const
{
	return m_DeveloperSN;
}

const CString& EngineSettings::GetProjectId() const
{
	return m_ProjectId;
}

void EngineSettings::SetEnginePath(const CString& sEnginePath)
{
	m_EnginePath = sEnginePath;
}

void EngineSettings::SetLicenseFilePath(const CString& sLicenseFilePath)
{
	m_LicenseFilePath = sLicenseFilePath;
}

void EngineSettings::SetDeveloperSN(const CString& sDeveloperSN)
{
	m_DeveloperSN = sDeveloperSN;
}

void EngineSettings::SetProjectId(const CString& sProjectId)
{
	m_ProjectId  = sProjectId;
}

