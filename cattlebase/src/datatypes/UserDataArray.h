#ifndef __USERDATAARRAY_H__
#define __USERDATAARRAY_H__

#include "UserData.h"
#include "UserDataComparator.h"
#include "../DataArray.h"

class  CUserDataArray : public CDataArray<UserData,UserDataComparator> 
{
public:
	UserData *GetPtrByUserId(int Id);
	UserData *GetPtrByUserName(CString &uname);
};




#endif