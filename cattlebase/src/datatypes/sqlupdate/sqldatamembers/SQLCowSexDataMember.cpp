#include "stdafx.h"
#include "SQLCowSexDataMember.h"

SQLCowSexDataMember::SQLCowSexDataMember(CowSexDataMember* pCowSexMember) :
		SQLDataMember(pCowSexMember) 
{
}
CString SQLCowSexDataMember::GetSQLStringValue(CowSexDataMember* pCowSexMember)
{
	CowSex cowSex = pCowSexMember->GetValue();
	return FormatInt((int)cowSex.getCOWSEX());
}