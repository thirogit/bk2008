#include "stdafx.h"
#include "Code39Validator.h"
#include "../../missing/util.h"


bool CCode39Validator::IsSoftViolation(const CString& anArg)
{
	if(!IsHardViolation(anArg))
	{
		return !IsValid(anArg);
	}
	return false;
}
bool CCode39Validator::IsHardViolation(const CString& anArg)
{
	int iLen = anArg.GetLength();

	if(!(iLen == 14 || iLen == 12))
		return true;

	if(iLen == 14 && (!isalpha(anArg.GetAt(0)) || !isalpha(anArg.GetAt(1))))
		return true;
	
	
	if(iLen == 14 && !CUtil::IsNumber(anArg.Mid(2,12)))
		return true;
		
	if(iLen == 12 && !CUtil::IsNumber(anArg))
		return true;

	return false;

}