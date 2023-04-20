#ifndef __SQLUPDATEDATAMEMBERCOPY_H__
#define __SQLUPDATEDATAMEMBERCOPY_H__

#include "DataMemberCopy.h"

template<class DATA,class MEMBERTYPE,class UPDATEENUMTP>
class CSQLUpdateDataMemberCopy : public CDataMemberCopy<DATA,MEMBERTYPE,UPDATEENUMTP>
{	
public:
	CSQLUpdateDataMemberCopy(MEMBERTYPE DATA::*memberPtr,UPDATEENUMTP sqlUpdateValue) 
		: CDataMemberCopy<DATA,MEMBERTYPE,UPDATEENUMTP>(memberPtr,sqlUpdateValue)
	{
		
	};
	void DoSQLUpdateCopy(DATA* copyTo,DATA* copyFrom)
	{
		if(copyFrom->SQLUpdate.Check(m_sqlUpdateValue))
			DoCopy(copyTo,copyFrom);			
	};


};

#endif