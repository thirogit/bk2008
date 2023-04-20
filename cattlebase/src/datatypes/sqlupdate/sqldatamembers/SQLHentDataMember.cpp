#include "stdafx.h"
#include "SQLHentDataMember.h"

SQLHentDataMember::SQLHentDataMember(HentDataMember* pHentMember) :
		SQLDataMember(pHentMember) 
{
}
 CString SQLHentDataMember::GetSQLStringValue(HentDataMember* pHentMember)
 {
	Hent* pHent = pHentMember->GetValue();
	if(pHent)
	{
		return FormatInt(pHent->GetId());
	}
	else
		return GetSQLNull();
	
}