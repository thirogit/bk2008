#include "stdafx.h"
#include "VersionInfoList.h"


#include "stdafx.h"
#include "VersionInfoList.h"


bool CVersionInfoList::ContainsVerNo(UINT verNo)
{
	CSaveVersionInfo *svi = NULL;
	ArrayIterator it = iterator();
	while(it.hasNext())
	{
		svi = *(it++);
		if(svi->GetVerNo() == verNo)
			return true;
	}

	return false;
}

bool CVersionInfoList::ContainsArchiveName(const CString& archiveName)
{
	CSaveVersionInfo *svi = NULL;
	ArrayIterator it = iterator();
	while(it.hasNext())
	{
		svi = *(it++);
		if(svi->GetArchiveName() == archiveName)
			return true;
	}

	return false;
}


UINT CVersionInfoList::GetMaxVerNo()
{
	UINT maxVerNo = 0;
	CSaveVersionInfo *svi = NULL;
	ArrayIterator it = iterator();
	while(it.hasNext())
	{
		svi = *(it++);
		if(svi->GetVerNo() > maxVerNo)
			maxVerNo = svi->GetVerNo();
	}

	return maxVerNo;

}
void CVersionInfoList::CreateVIHeader(VERSION_INFO_HEADER* pHeader)
{
	memset(pHeader,0,sizeof(VERSION_INFO_HEADER));
	strncpy(pHeader->id,VERSION_INFO_HEADER_ID,sizeof(pHeader->id));
	pHeader->size = sizeof(VERSION_INFO_HEADER) + GetAllVInfosSize();
	pHeader->version_info_count = GetCount();
	pHeader->max_version_no = GetMaxVerNo();
}

UINT CVersionInfoList::GetAllVInfosSize()
{
	UINT viSize = 0;
	CSaveVersionInfo *svi = NULL;
	ArrayIterator it = iterator();
	while(it.hasNext())
	{
		svi = *(it++);
		viSize += svi->Get_VERSION_INFO_size();
	}

	return viSize;
}

COMPARE_RESULT CVersionInfoList::Compare(UINT sortBy,CSaveVersionInfo *DataLeft,CSaveVersionInfo *DataRight)
{
	return DataLeft->GetVerNo() - DataRight->GetVerNo();
}
//
//void CVersionInfoList::Sort()
//{
//	SortBy(0,true);
//}

