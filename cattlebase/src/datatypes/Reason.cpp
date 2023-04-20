#include "stdafx.h"
#include "Reason.h"

Reason::Reason(UINT Id) : reasonid(Id),inoutreason(REASON_BOTH) 
{
}

UINT Reason::GetId() const
{
	return reasonid;
}

CString Reason::GetReasonCode() const
{
	return reasoncode;
}

CString	Reason::GetReasonName() const
{
	return reasoncode;
}

Reason::REASON_INOUT_TYPE Reason::GetReasonType() const
{
	return inoutreason;
}


void Reason::SetReasonCode(const CString& sReasonCode)
{
	reasoncode = sReasonCode;
}
void Reason::SetReasonName(const CString& sReasonName)
{
	reasonname = sReasonName;
}
void Reason::SetReasonType(Reason::REASON_INOUT_TYPE reasonType)
{
	inoutreason = reasonType;
}

