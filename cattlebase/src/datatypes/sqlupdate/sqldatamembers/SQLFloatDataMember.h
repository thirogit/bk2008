#ifndef __SQLFLOATDATAMEMBER_H__
#define __SQLFLOATDATAMEMBER_H__

#include "../../DataMembersDefs.h"
#include "../SQLDataMember.h"

class SQLFloatDataMember : public SQLDataMember<FloatDataMember>
{
public:
	SQLFloatDataMember(FloatDataMember* pFloatMember,int precision);
	virtual CString GetSQLStringValue(FloatDataMember* pFloatMember);
private:
	int m_precision;
};


#endif