#ifndef __SQLSTRINGINTDATAMEMBER_H__
#define __SQLSTRINGINTDATAMEMBER_H__

#include "../../Cow.h"
#include "../SQLDataMember.h"

class SQLStringDataMember : public SQLDataMember<StringDataMember>
{
public:
	SQLStringDataMember(StringDataMember* pStringMember,bool bEmptyNull = true);
	virtual CString GetSQLStringValue(StringDataMember* pStringMember);	
protected:
	bool m_bEmptyNull;
};


#endif