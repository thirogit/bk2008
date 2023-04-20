#include "stdafx.h"
#include "SQLBooleanDataMember.h"

SQLBooleanDataMember::SQLBooleanDataMember(BooleanDataMember* pBooleanMember) :
		SQLDataMember(pBooleanMember) 
{
}

CString SQLBooleanDataMember::GetSQLStringValue(BooleanDataMember* pBooleanMember)
{
	 return FormatInt(pBooleanMember->GetValue() ? 1 : 0);	
}