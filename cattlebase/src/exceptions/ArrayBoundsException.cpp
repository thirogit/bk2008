#include "stdafx.h"
#include "ArrayBoundsException.h"

IMPLEMENT_DYNAMIC(CArrayBoundsException, CBaseException)


CArrayBoundsException::CArrayBoundsException(int arraySize,int badIndex) :
	m_arraySize(arraySize),m_badIndex(badIndex)
{
	error.Format("ARRAY SIZE = %d, INDEX %d WAS REQUESTED",m_arraySize,m_badIndex);
}
CArrayBoundsException::CArrayBoundsException(const CArrayBoundsException &copy)
									:	m_arraySize(copy.m_arraySize),
										m_badIndex(copy.m_badIndex)
{
	error = copy.error;
}

