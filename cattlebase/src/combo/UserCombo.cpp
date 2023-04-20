#include "stdafx.h"
#include "UserCombo.h"
#include "global_arrays.h"


CUserCombo::CUserCombo() : CBasePtrCombo<CComboBox,UserData>(ary_users.GetSize())
{
}

UserData *CUserCombo::GetItemText(int item,CString &itemText)
{
	UserData *itemUser = ary_users[item];
	itemText = itemUser->username;
	return itemUser;
}