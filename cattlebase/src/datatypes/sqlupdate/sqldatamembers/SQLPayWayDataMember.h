#ifndef __SQLPAYWAYDATAMEMBER_H__
#define __SQLPAYWAYDATAMEMBER_H__

#include "../../Invoice.h"
#include "../SQLDataMember.h"

class SQLPayWayDataMember : public SQLDataMember<Invoice::PayWayDataMember>
{
public:
	SQLPayWayDataMember(Invoice::PayWayDataMember* pPayWayMember);
	virtual CString GetSQLStringValue(Invoice::PayWayDataMember* pPayWayMember);	
};


#endif