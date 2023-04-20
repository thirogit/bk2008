#ifndef __DATAMEMBERPTR_H__
#define __DATAMEMBERPTR_H__

#include "DataMember.h"

template<class PTRTYPE>
class DataMemberPtr : public DataMember<PTRTYPE,PTRTYPE>
{
public:
	DataMemberPtr(PTRTYPE initValue = PTRTYPE()) : DataMember<PTRTYPE,PTRTYPE>(initValue) 
	{
	}
	virtual void Rollback()
	{
		DataMember<PTRTYPE,PTRTYPE>::Rollback();
		m_previousValue = PTRTYPE();
			
	}
	virtual void Commit()
	{
		DataMember<PTRTYPE,PTRTYPE>::Commit();
		m_previousValue = PTRTYPE();
	}
	
};


#endif