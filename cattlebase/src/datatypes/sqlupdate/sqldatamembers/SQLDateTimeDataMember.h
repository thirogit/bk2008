#ifndef __SQLDATETIMEDATAMEMBER_H__
#define __SQLDATETIMEDATAMEMBER_H__

#include "../../Cow.h"
#include "../SQLDataMember.h"

class SQLDateTimeDataMember : public SQLDataMember<DateTimeDataMember>
{
public:
	SQLDateTimeDataMember(DateTimeDataMember* pDateTimeMember,bool bShortDate);
	virtual CString GetSQLStringValue(DateTimeDataMember* pDateTimeMember);	
protected:
	bool m_bShortDate;
};


#endif