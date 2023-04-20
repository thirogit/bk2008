#include "stdafx.h"
#include "SQLDateTimeDataMember.h"

SQLDateTimeDataMember::SQLDateTimeDataMember(DateTimeDataMember* pDateTimeMember,bool bShortDate) :
		SQLDataMember(pDateTimeMember),m_bShortDate(bShortDate) 
{
}
CString SQLDateTimeDataMember::GetSQLStringValue(DateTimeDataMember* pDateTimeMember)
{
	CDateTime dateTime = pDateTimeMember->GetValue();

	if(!dateTime.IsNull())
	{	
		if(m_bShortDate)
			return QuoteString(dateTime.GetDateInFmt(CDateTime::YYYYdotMMdotDD));
		else
			return QuoteString(dateTime.GetDateInFmt(CDateTime::YYYYdotMMdotDD_HHcolonMMcolonSS));
	}
	else
		return GetSQLNull();

}