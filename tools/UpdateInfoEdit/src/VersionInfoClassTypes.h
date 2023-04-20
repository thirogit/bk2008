#ifndef __VERSIONINFOCLASSTYPES_H__
#define __VERSIONINFOCLASSTYPES_H__

#include "VersionInfoStructFileTypes.h"
#include <classes/types/DateTime.h>

class CVersionInfo
{
public:
	CVersionInfo();
	CVersionInfo(const VERSION_INFO* pVI);
	UINT Get_VERSION_INFO_size();
	void CreateVersionInfo(VERSION_INFO* pVI);

	const CString&		GetArchiveName() const;
	const CString&		GetVersionDesc() const;
	const CStringArray&	GetVerFiles() const;
	const UINT			GetVerNo() const;
	const CDateTime&	GetVersionDate() const;
	bool				GetOWArchName() const;
	int					GetVerFilesCount() const;
	void				CopyFrom(const CVersionInfo& src);

	void			SetArchiveName(const CString& archiveName);
	void			SetVersionDesc(const CString& versionDesc);
	void			SetVerNo(UINT verNo);
	void			SetOWArchName(bool bOWArchName);
	void			SetVersionDate(const CDateTime& verDate);

	void			AddVerFile(const CString& relFilePath);
	void			AddVerFiles(const CStringArray& verFiles);
protected:
	void			SaveDate(DAYDATE* pOutDayDate,const CDateTime& inDayDate);
	void			ReadDate(CDateTime& outDayDate,const DAYDATE* pInDayDate);
	
private:
	CStringArray m_verFiles;
	CString m_archiveName;
	CString m_verDesc;
	UINT	m_verNo;
	bool	m_owArchiveName;
	CDateTime m_versionDate;
};


#endif //__VERSIONINFODATATYPES_H__