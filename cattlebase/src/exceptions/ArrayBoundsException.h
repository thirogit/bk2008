#ifndef __ARRAYBOUNDSEXCEPTION_H__
#define __ARRAYBOUNDSEXCEPTION_H__

#include "BaseException.h" 


class CArrayBoundsException : public CBaseException
{
	DECLARE_DYNAMIC(CArrayBoundsException)
public:
	CArrayBoundsException(int arraySize,int badIndex);	
	CArrayBoundsException(const CArrayBoundsException &copy);
private:
	int m_arraySize;
	int m_badIndex;
	
};

#endif