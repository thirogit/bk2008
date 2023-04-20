#include "stdafx.h"
#include "CowEanValidator.h"
#include "../../missing/util.h"
#include "../TempStringLoader.h"

bool CCowEanValidator::IsValid(const CString& anArg)
{
	return CUtil::InCowEANValid((LPCSTR)anArg);
}
CString CCowEanValidator::TellMeWhy(const CString& anArg)
{
	
	if(IsHardViolation(anArg))
	{
		return CTempStringLoader(IDS_ERRNOTACOWEAN);
	}

	if(!IsValid(anArg))
	{
		return CTempStringLoader(IDS_BADCTRLDIGIT);
	}

	return CString();
	
}
