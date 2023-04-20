#ifndef __PTRDATAMEMBERCOPY_H__
#define __PTRDATAMEMBERCOPY_H__

#include "DataCopy.h"


template<class DATA,class PTRDATAMEMBERTYPE>
class CPtrDataMemberCopy : public CDataCopy<DATA>
{	
public:
	CPtrDataMemberCopy(PTRDATAMEMBERTYPE* (DATA::*getter)() const,void (DATA::*changer)(PTRDATAMEMBERTYPE*)) 
		: m_getter(getter),m_changer(changer)
	{
		
	};
	CPtrDataMemberCopy(const CPtrDataMemberCopy& copyFrom)
	{
		m_getter = copyFrom.m_getter;
		m_changer = copyFrom.m_changer;
	}
	
	void DoCopy(DATA* copyTo,DATA* copyFrom)
	{
		(copyTo->*m_changer)((copyFrom->*m_getter)());		
	};
	virtual ~CPtrDataMemberCopy() 
	{
	}

protected:
	PTRDATAMEMBERTYPE* (DATA::*m_getter)() const;
	void (DATA::*m_changer)(PTRDATAMEMBERTYPE*);
	

};

#endif