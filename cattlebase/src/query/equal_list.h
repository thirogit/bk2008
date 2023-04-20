#ifndef __EQUAL_LIST_H__
#define __EQUAL_LIST_H__

#include "sql_string.h"
#include "../missing/MapEx.h"

class SQLEqualList
{
public:
	SQLEqualList();    
	void Add(SQLEqualList &list2add,const char* fieldPrefix = NULL);
	void Add(const char* szFiledName,SQLString &value);
	CString equal_list();
private:
	CMap<CString,const CString&,SQLString,SQLString&> eq_list;

};


#endif