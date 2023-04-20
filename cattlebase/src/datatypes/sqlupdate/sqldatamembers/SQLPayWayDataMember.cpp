#include "stdafx.h"
#include "SQLPayWayDataMember.h"

SQLPayWayDataMember::SQLPayWayDataMember(Invoice::PayWayDataMember* pPayWayMember) :
		SQLDataMember(pPayWayMember) 
{
}
CString SQLPayWayDataMember::GetSQLStringValue(Invoice::PayWayDataMember* pPayWayMember)
{
	CString sPayWay;
	sPayWay.AppendChar(pPayWayMember->GetValue());
	return QuoteString(sPayWay);
}