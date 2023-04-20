#include "stdafx.h"
#include "FloatString.h"


CFloatString::CFloatString(double dFloat, int precision) : CStringFormat("%.*f",max(0,precision),dFloat)
{

}

CFloatString::operator CString()  const
{
	return CStringFormat::operator CString();
}
CFloatString::operator LPCSTR() const
{
	return CStringFormat::operator LPCSTR();
}

