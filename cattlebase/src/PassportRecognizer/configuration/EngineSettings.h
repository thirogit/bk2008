#ifndef __ENGINESETTINGS_H__
#define __ENGINESETTINGS_H__


class EngineSettings
{
public:
	EngineSettings();
		
	const CString& GetEnginePath() const;
	const CString& GetLicenseFilePath() const;
	const CString& GetDeveloperSN() const;
	const CString& GetProjectId() const;
	
	void SetEnginePath(const CString& sEnginePath);
	void SetLicenseFilePath(const CString& sLicenseFilePath);
	void SetDeveloperSN(const CString& sDeveloperSN);
	void SetProjectId(const CString& sProjectId);
	

private:
	CString m_EnginePath;
	CString m_LicenseFilePath;
	CString m_DeveloperSN;
	CString m_ProjectId;
	
};

#endif