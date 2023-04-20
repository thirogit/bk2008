#include "stdafx.h"
#include "SQLHentTypeDataMember.h"

SQLHentTypeDataMember::SQLHentTypeDataMember(HentTypeDataMember* pHentTypeMember) :
		SQLDataMember(pHentTypeMember) 
{
}
 CString SQLHentTypeDataMember::GetSQLStringValue(HentTypeDataMember* pHentTypeMember)
 {
	HentType* pHentType = pHentTypeMember->GetValue();
	if(pHentType)
	{
		return FormatInt(pHentType->henttypeid);
	}
	else
		return GetSQLNull();
	
}