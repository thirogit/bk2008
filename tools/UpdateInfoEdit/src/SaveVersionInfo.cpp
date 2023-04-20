#include "stdafx.h"
#include "SaveVersionInfo.h"


CSaveVersionInfo::CSaveVersionInfo(const CVersionInfo& verInfo)
{
	CopyFrom(verInfo);
	SetSave(false);
	SetUpload(false);
}
void CSaveVersionInfo::SetSave(bool bSave)
{
	m_bSave = bSave;
}
bool CSaveVersionInfo::IsSaved() const
{
	return m_bSave;
}


void CSaveVersionInfo::SetUpload(bool bUpload)
{
	m_bUpload = bUpload;
}
bool CSaveVersionInfo::IsUploaded() const
{
	return m_bUpload;
}

const CString& CSaveVersionInfo::GetTempZipFilePath() const
{
	return m_tempZipFilePath;
}
void CSaveVersionInfo::SetTempZipFilePath(const CString& zipFilePath)
{
	m_tempZipFilePath = zipFilePath;
}

