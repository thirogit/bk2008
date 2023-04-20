#ifndef __DATETIMEDATAMEMBERCOPY_H__
#define __DATETIMEDATAMEMBERCOPY_H__

#include "DataMemberCopy.h"

template<class DATA>
class CDateTimeDataMemberCopy : public CDataMemberCopy<DATA,CDateTime>
{	
public:
	CDateTimeDataMemberCopy(CDateTime (DATA::*getter)() const,void (DATA::*changer)(const CDateTime&)) 
		: CDataMemberCopy(getter,changer)
	{
		
	};
};

#endif