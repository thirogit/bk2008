#ifndef __VERSIONINFOLIST_H__
#define __VERSIONINFOLIST_H__

#include "../src/SortArray.h"
#include "../UpdateInfoEdit/src/VersionInfoClassTypes.h"

class CVersionInfoList : public CSortArray<CVersionInfo> 
{
public:
	virtual COMPARE_RESULT Compare(UINT sortBy,CVersionInfo *DataLeft,CVersionInfo *DataRight);
	void Sort(bool bAscending);
};




#endif