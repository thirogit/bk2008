#pragma once

#include "VersionInfoClassTypes.h"

class CSaveVersionInfo : public CVersionInfo
{
public:
	CSaveVersionInfo(const CVersionInfo& verInfo);
		
	void SetSave(bool bSave);
	bool IsSaved() const;

	void SetUpload(bool bUpload);
	bool IsUploaded() const;

	const CString& GetTempZipFilePath() const;
	void SetTempZipFilePath(const CString& zipFilePath);

	
private:
	bool		m_bSave;
	bool		m_bUpload;
	CString		m_tempZipFilePath;

};
