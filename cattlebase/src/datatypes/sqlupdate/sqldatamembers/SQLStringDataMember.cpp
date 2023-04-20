#include "stdafx.h"
#include "SQLStringDataMember.h"

SQLStringDataMember::SQLStringDataMember(StringDataMember* pStringMember,bool bEmptyNull/* = true*/) :
		SQLDataMember(pStringMember) ,m_bEmptyNull(bEmptyNull)
{
}
 CString SQLStringDataMember::GetSQLStringValue(StringDataMember* pStringMember)
 {
	CString sString = pStringMember->GetValue();
	if(sString.IsEmpty() && m_bEmptyNull)
	{
		return GetSQLNull();
	}
	else
	{
		sString.Replace("'","''");
		return QuoteString(sString);
	}
		
}