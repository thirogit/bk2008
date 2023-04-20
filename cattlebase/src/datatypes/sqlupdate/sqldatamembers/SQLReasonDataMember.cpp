#include "stdafx.h"
#include "SQLReasonDataMember.h"

SQLReasonDataMember::SQLReasonDataMember(ReasonDataMember* pReasonMember) :
		SQLDataMember(pReasonMember) 
{
}
 CString SQLReasonDataMember::GetSQLStringValue(ReasonDataMember* pReasonMember)
 {
	Reason* pReason = pReasonMember->GetValue();
	if(pReason)
	{
		return FormatInt(pReason->GetId());
	}
	else
		return GetSQLNull();
	
}