#ifndef __CURSOR_H__
#define __CURSOR_H__

#include <boost/shared_ptr.hpp>
#include "NewPtrFlushArray.h"

class CursorFetchLimit
{
public:
	CursorFetchLimit(int minFetch,int maxFetch) : m_MinFetch(minFetch),m_MaxFetch(maxFetch) {};
	int GetMaxFetch() const { return m_MinFetch; }
	int GetMinFetch() const { return m_MinFetch; } 
private:
	int m_MinFetch;
	int m_MaxFetch;
};

template<class T>
class Cursor
{
protected:
	Cursor(int count,CursorFetchLimit& limits) : m_Count(count),m_Limits(limits) {};
public:
	inline int GetCount() { return m_Count; };
	inline const CursorFetchLimit& GetLimits() const { return m_Limits; };	
	virtual int FetchMany(int howMany,NewPtrFlushArray<T>& bucket) = 0;
private:
	int m_Count;
	CursorFetchLimit m_Limits;
};

#endif