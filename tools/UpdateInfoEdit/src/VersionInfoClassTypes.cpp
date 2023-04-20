#include "stdafx.h"
#include "VersionInfoClassTypes.h"


CVersionInfo::CVersionInfo() : m_owArchiveName(false)
{
	
}

void CVersionInfo::CopyFrom(const CVersionInfo& src)
{
	SetArchiveName(src.GetArchiveName());
	SetVersionDesc(src.GetVersionDesc());
	SetVerNo(src.GetVerNo());
	m_verFiles.Append(src.GetVerFiles());
	m_versionDate = src.GetVersionDate();
}

const CString& CVersionInfo::GetArchiveName() const
{
	return m_archiveName;
}
const CString& CVersionInfo::GetVersionDesc() const
{
	return m_verDesc;
}

const UINT CVersionInfo::GetVerNo() const
{
	return m_verNo;
}

const CStringArray& CVersionInfo::GetVerFiles() const
{
	return m_verFiles;
}

void CVersionInfo::SetArchiveName(const CString& archiveName)
{
	m_archiveName = archiveName;
}
void CVersionInfo::SetVersionDesc(const CString& versionDesc)
{
	m_verDesc = versionDesc;
}
void CVersionInfo::SetVerNo(UINT verNo)
{
	m_verNo = verNo;
}

void  CVersionInfo::AddVerFile(const CString& relFilePath)
{
	m_verFiles.Add(relFilePath);
}
void  CVersionInfo::AddVerFiles(const CStringArray& verFiles)
{
	m_verFiles.Append(verFiles);
}

bool CVersionInfo::GetOWArchName() const
{
	return m_owArchiveName;
}

void CVersionInfo::SetOWArchName(bool bOWArchName)
{
	m_owArchiveName = bOWArchName;
}
int	CVersionInfo::GetVerFilesCount() const
{
	return m_verFiles.GetCount();
}

const CDateTime& CVersionInfo::GetVersionDate() const
{
	return m_versionDate;
}
void CVersionInfo::SetVersionDate(const CDateTime& verDate)
{
	m_versionDate = verDate;
}

void CVersionInfo::SaveDate(DAYDATE* pOutDayDate,const CDateTime& inDayDate)
{
	pOutDayDate->year = inDayDate.GetYear();
	pOutDayDate->month = inDayDate.GetMonth();
	pOutDayDate->day = inDayDate.GetDay();
}
void CVersionInfo::ReadDate(CDateTime& outDayDate,const DAYDATE* pInDayDate)
{
	outDayDate.SetYearMonthDay(	pInDayDate->year,
								pInDayDate->month,
								pInDayDate->day);
}

UINT CVersionInfo::Get_VERSION_INFO_size()
{
	ASSERT(GetVerFilesCount() > 0);
	return sizeof(VERSION_INFO) + MAX_FILECHANGED_PATH*(GetVerFilesCount()-1);
}
void CVersionInfo::CreateVersionInfo(VERSION_INFO* pVI)
{
	pVI->version_number = GetVerNo();
	strncpy(pVI->description,(LPCSTR)GetVersionDesc(),MAX_DESCRIPTION);
	strncpy(pVI->archive_name,(LPCSTR)GetArchiveName(),MAX_ARCHIVE_NAME);
	pVI->files_changed_count = GetVerFilesCount();
	pVI->size = Get_VERSION_INFO_size();
	SaveDate(&pVI->version_date,GetVersionDate());

	CStringArray filesChanged;
	filesChanged.Copy(GetVerFiles());

	for(int i = 0;i < filesChanged.GetCount();i++)
	{
		strncpy(pVI->files_changed[i],filesChanged[i],MAX_FILECHANGED_PATH);
	}
}

 CVersionInfo::CVersionInfo(const VERSION_INFO* pVI)
 {
	SetVerNo(pVI->version_number);
	SetVersionDesc(CString(pVI->description));
	SetArchiveName(CString(pVI->archive_name));
	CDateTime verDate;
	ReadDate(verDate,&pVI->version_date);
	SetVersionDate(verDate);
	
	for(int i = 0;i < pVI->files_changed_count;i++)
	{
		AddVerFile(CString(pVI->files_changed[i]));
	}

 }