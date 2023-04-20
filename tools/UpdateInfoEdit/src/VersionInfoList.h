#ifndef __VERSIONINFOLIST_H__
#define __VERSIONINFOLIST_H__

#include <SortArray.h>
#include "SaveVersionInfo.h"

class CVersionInfoList : public CSortArray<CSaveVersionInfo> 
{
public:
	bool ContainsVerNo(UINT verNo);
	bool ContainsArchiveName(const CString& archiveName);
	UINT GetMaxVerNo();
	void CreateVIHeader(VERSION_INFO_HEADER* pHeader);
	UINT GetAllVInfosSize();
	virtual COMPARE_RESULT Compare(UINT sortBy,CSaveVersionInfo *DataLeft,CSaveVersionInfo *DataRight);
	//void Sort();
};




#endif