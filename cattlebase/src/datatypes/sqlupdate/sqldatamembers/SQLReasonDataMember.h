#ifndef __SQLREASONDATAMEMBER_H__
#define __SQLREASONDATAMEMBER_H__

#include "../../Doc.h"
#include "../SQLDataMember.h"

class SQLReasonDataMember : public SQLDataMember<ReasonDataMember>
{
public:
	SQLReasonDataMember(ReasonDataMember* pReasonMember);
	virtual CString GetSQLStringValue(ReasonDataMember* pReasonMember);	
};


#endif