#include "stdafx.h"
#include "UserDataArray.h"
#include "UserDataComparator.h"

UserData *CUserDataArray::GetPtrByUserId(int Id)
{
	UserData *temp;
	for(int i = 0;i< GetSize();i++)
	{
		temp = (UserData*)GetAt(i); 
        if(temp->userid == Id) return temp;
	}
return NULL;
	
};

UserData *CUserDataArray::GetPtrByUserName(CString &uname)
{
	UserData *temp;
	for(int i = 0;i< GetSize();i++)
	{
		temp = (UserData*)GetAt(i); 
        if(temp->username == uname) return temp;
	}
	return NULL;

};

