#include "stdafx.h"
#include "FarmNoValidator.h"
#include "../../missing/util.h"
#include "../TempStringLoader.h"

bool CFarmNoValidator::IsValid(const CString& anArg)
{
	return CUtil::IsFarmNoValid((LPCSTR)anArg);
}
CString CFarmNoValidator::TellMeWhy(const CString& anArg)
{
	if(IsHardViolation(anArg))
	{
		return CTempStringLoader(IDS_ERRNOTAFARMNO);
	}

	if(!IsValid(anArg))
	{
		return CTempStringLoader(IDS_BADCTRLDIGIT);
	}

	return CString();
	
}

