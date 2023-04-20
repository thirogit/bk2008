#ifndef __STRINGDATAMEMBERCOPY_H__
#define __STRINGDATAMEMBERCOPY_H__

#include "DataMemberCopy.h"

template<class DATA>
class CStringDataMemberCopy : public CDataMemberCopy<DATA,CString>
{	
public:
	CStringDataMemberCopy(CString (DATA::*getter)() const,void (DATA::*changer)(const CString&)) 
		: CDataMemberCopy(getter,changer)
	{
		
	};
};

#endif