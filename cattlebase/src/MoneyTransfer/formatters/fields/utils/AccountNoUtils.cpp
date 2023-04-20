#include "stdafx.h"
#include "AccountNoUtils.h"

CString AccountNoUtils::GetBankNo(const CString& sAccountNo)
{
	if(sAccountNo.GetLength() == 26)
	{
		return sAccountNo.Mid(2,8);
	}
	return "########";

}