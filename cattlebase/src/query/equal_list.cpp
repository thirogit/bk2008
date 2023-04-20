#include "stdafx.h"
#include "equal_list.h"

#include <sstream>

SQLEqualList::SQLEqualList()
{
}

void SQLEqualList::Add(SQLEqualList &list2add,const char* fieldPrefix/* = NULL*/) 
{
	POSITION pos = list2add.eq_list.GetStartPosition();
	CString field;
	SQLString value;
	while(pos)
	{
		list2add.eq_list.GetNextAssoc(pos,field,value);
		if(fieldPrefix) field.Insert(0,fieldPrefix);
		eq_list.SetAt(field,value);
	}
}
void SQLEqualList::Add(const char* szFiledName,SQLString &value)
{
	eq_list.SetAt(CString(szFiledName),value);
};

CString SQLEqualList::equal_list()
{
	std::stringstream s;
	POSITION pos = eq_list.GetStartPosition();
	CString key;
	SQLString val;
	
	while(pos)
	{
		eq_list.GetNextAssoc(pos,key,val);
		s << key << "=" << val.quotedstr();  
		if(pos) s << ",";
	}
	return CString(s.str().c_str());
		
}
