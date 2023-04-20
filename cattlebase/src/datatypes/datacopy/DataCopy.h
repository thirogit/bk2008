#ifndef __DATACOPY_H__
#define __DATACOPY_H__

template<class DATA>
class CDataCopy
{
public:
	virtual void DoCopy(DATA* copyTo,DATA* copyFrom) = 0;
};

#endif