#include "stdafx.h"
#include "SQLFloatDataMember.h"
#include "../../../Utils/StringFormat.h"

SQLFloatDataMember::SQLFloatDataMember(FloatDataMember* pFloatMember,int precision) :
		SQLDataMember(pFloatMember),m_precision(precision)
{
}
CString SQLFloatDataMember::GetSQLStringValue(FloatDataMember* pFloatMember)
{
	return CStringFormat("%.*f",max(2,m_precision),pFloatMember->GetValue());
}