#include "stdafx.h"
#include "IntString.h"


CIntString::CIntString(int iInt) : CStringFormat("%d",iInt)
{
}

CIntString::CIntString(UINT uInt) : CStringFormat("%u",uInt)
{

}

CIntString::operator CString()  const
{
	return CStringFormat::operator CString();
}
CIntString::operator LPCSTR() const
{
	return CStringFormat::operator LPCSTR();
}

