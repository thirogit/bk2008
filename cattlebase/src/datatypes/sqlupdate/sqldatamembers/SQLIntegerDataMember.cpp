#include "stdafx.h"
#include "SQLIntegerDataMember.h"

SQLIntegerDataMember::SQLIntegerDataMember(IntegerDataMember* pIntegerMember) :
		SQLDataMember(pIntegerMember)
{
}
 CString SQLIntegerDataMember::GetSQLStringValue(IntegerDataMember* pIntegerMember)
 {
	char szBuffer[17]; 
	int iValue = pIntegerMember->GetValue();
	_itoa(iValue,szBuffer,10);
	return CString(szBuffer);
}