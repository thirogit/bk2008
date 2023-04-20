#ifndef __SQLWEIGHTDATAMEMBER_H__
#define __SQLWEIGHTDATAMEMBER_H__

#include "../../DataMembersDefs.h"
#include "../SQLDataMember.h"

class SQLWeightDataMember : public SQLDataMember<WeightDataMember>
{
public:
	SQLWeightDataMember(WeightDataMember* pWeightMember,int precision);
	virtual CString GetSQLStringValue(WeightDataMember* pWeightMember);
private:
	int m_precision;
};


#endif