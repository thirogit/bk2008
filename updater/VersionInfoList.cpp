#include "stdafx.h"
#include "VersionInfoList.h"

COMPARE_RESULT CVersionInfoList::Compare(UINT sortBy,CVersionInfo *DataLeft,CVersionInfo *DataRight)
{
	return DataLeft->GetVerNo() - DataRight->GetVerNo();
}

void CVersionInfoList::Sort(bool bAscending)
{
	SortBy(0,bAscending);
}