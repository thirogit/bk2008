#ifndef __DATAMEMBERCOPY_H__
#define __DATAMEMBERCOPY_H__


template<class DATA,class DATAMEMBERTYPE>
class CDataMemberCopy : public CDataCopy<DATA>
{	
public:
	CDataMemberCopy(DATAMEMBERTYPE (DATA::*getter)() const,void (DATA::*changer)(const DATAMEMBERTYPE&)) 
		: m_getter(getter),m_changer(changer)
	{
		
	};
	CDataMemberCopy(const CDataMemberCopy& copyFrom)
	{
		m_getter = copyFrom.m_getter;
		m_changer = copyFrom.m_changer;
	}
	
	void DoCopy(DATA* copyTo,DATA* copyFrom)
	{
		(copyTo->*m_changer)((copyFrom->*m_getter)());		
	};
	virtual ~CDataMemberCopy() 
	{
	}

protected:
	DATAMEMBERTYPE (DATA::*m_getter)() const;
	void (DATA::*m_changer)(const DATAMEMBERTYPE&);
	

};

#endif