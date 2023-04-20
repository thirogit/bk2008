#ifndef __SQLDOCDATAMEMBER_H__
#define __SQLDOCDATAMEMBER_H__

#include "../../Cow.h"
#include "../SQLDataMember.h"

class SQLDocDataMember : public SQLDataMember<DocDataMember>
{
public:
	SQLDocDataMember(DocDataMember* pDocMember);
	virtual CString GetSQLStringValue(DocDataMember* pDocMember);	
};


#endif