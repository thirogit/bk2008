// cmString.cpp: implementation of the cmString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


cmString::cmString()
{

}

cmString::cmString(TCHAR* s):CString(s)
{}

cmString::cmString(const TCHAR* s):CString(s)
{}

cmString::cmString(const cmString& s):CString(s)
{}

cmString::cmString(const CString& s):CString(s)
{}


cmString::~cmString()
{

}

const cmString& cmString::operator=(const TCHAR* s)
{
	CString::operator =(s);

	return *this;
}

const cmString& cmString::operator=(const cmString& s)
{
	CString::operator =(s);

	return *this;
}

const cmString& cmString::operator=(const CComBSTR& s)
{
	USES_CONVERSION;

#ifdef _UNICODE
	*this = W2T(s);
#else
	*this = W2A(s);
#endif

	return *this;
}

const cmString& cmString::operator=(BSTR& s)
{
	*this = CComBSTR(s);

	return *this;
}
const cmString& cmString::operator=(int value)
{
	Format(_T("%d"), value);

	return *this;
}

const cmString& cmString::operator=(long value)
{
	Format(_T("%ld"), value);

	return *this;
}

const cmString& cmString::operator=(const CString& s)
{
	CString::operator =(s);

	return *this;
}

cmString& cmString::operator+=(const TCHAR* s)
{
	CString::operator +=(s);
	
	return *this;
}

cmString& cmString::operator+=(const cmString& s)
{
	CString::operator +=(s);

	return *this;
}

cmString& cmString::operator+=(TCHAR s)
{
	CString::operator +=(s);

	return *this;
}

cmString& cmString::operator+=(const CString& s)
{
	CString::operator +=(s);

	return *this;
}

cmString::operator int() const
{
	int ret;

#ifdef _UNICODE
	ret = _wtoi((const TCHAR*)(*this));
#else
	ret = atoi((const char*)(*this));
#endif

	return ret;
}

cmString::operator long() const
{
	long ret;

#ifdef _UNICODE
	ret = _wtol((const TCHAR*)(*this));
#else
	ret = atol((const char*)(*this));
#endif

	return ret;
}

cmString::operator double() const
{
	double ret;

#ifdef _UNICODE
	ret = wcstod((const TCHAR*)(*this), NULL);
#else
	ret = strtod((const char*)(*this), NULL);
#endif

	return ret;
}

cmString::operator const char*() const
{
#ifdef _UNICODE
	USES_CONVERSION;

	return W2A(*this);
#else
	return (LPCTSTR)(CString)*this;
#endif
}

bool cmString::operator==(const CComBSTR& bstr) const
{
	USES_CONVERSION;

	bool ret;

#ifdef _UNICODE
	ret = (this->Compare((BSTR)bstr) == 0);
#else
	ret = (this->Compare(W2A(bstr)) == 0);
#endif
	
	return ret;
}

bool cmString::operator==(const TCHAR* s) const
{
	return CString::Compare(s) == 0;
}

bool cmString::operator==(const cmString& s) const
{
	return CString::Compare(s) == 0;
}

cmString operator +( const cmString& s1, const TCHAR* s2 )
{
	cmString ret;

	ret = s1;
	ret += s2;

	return ret;
}

cmString operator +(const TCHAR* s1, const cmString& s2)
{
	cmString ret;

	ret = s1;
	ret += s2;

	return ret;
}

cmString operator +(const cmString& s1, const cmString& s2)
{
	cmString ret;

	ret = s1;
	ret += s2;

	return ret;
}

cmString operator +(const cmString& s1, const CString& s2)
{
	cmString ret;

	ret = s1;
	ret += s2;

	return ret;
}

cmString operator +(const CString& s1, const cmString& s2)
{
	cmString ret;

	ret = s1;
	ret += s2;

	return ret;
}

const TCHAR* cmString::c_str() const
{
	return CString::operator LPCTSTR();
}

bool cmString::IsDecimalNumber() const
{
	bool ret = true;

	for (int i = 0; i < GetLength(); i++) {
#ifdef _UNICODE
		if (!iswdigit(GetAt(i))) {
			ret = false;
		}
#else
		if (!isdigit(GetAt(i))) {
			ret = false;
		}
#endif
	}

	return ret;
}

bool cmString::IsHexadecimalNumber(const cmString& prefix) const
{
	if (GetLength() <= prefix.GetLength())
		return false;

	if (Find(prefix) == -1)
		return false;

	bool ret = true;

	for (int i = prefix.GetLength(); i < GetLength(); i++) {
#ifdef _UNICODE
		if (!iswxdigit(GetAt(i))) {
			ret = false;
		}
#else
		if (!isxdigit(GetAt(i))) {
			ret = false;
		}
#endif
	}

	return ret;
}

long cmString::HexStringToNumber(const cmString& prefix) const
{
	if (!IsHexadecimalNumber(prefix))
		return 0;

	long value;
	cmString strValue;

	strValue = Mid(prefix.GetLength());
	
#ifdef _UNICODE
	swscanf(strValue.c_str(), _T("%X"), &value);
#else
	sscanf(strValue.c_str(), _T("%X"), &value);
#endif
	
	return value;
}

cmString cmString::FormatDecimalNumber(int digits, int precision, double value)
{
	cmString fmt;
	cmString ret;
	
	fmt.Format(_T("%%0%d.%df"), digits, precision);

	ret.Format(fmt, value);

	return ret;
}

cmString cmString::FormatHexNumber(const cmString& hexPrefix, int digits, long value, bool lowerCase)
{
	cmString fmt;
	cmString ret;
	
	if (lowerCase)
		fmt.Format(_T("%s%%%dx"), (const TCHAR*)hexPrefix, digits);
	else
		fmt.Format(_T("%s%%%dX"), (const TCHAR*)hexPrefix, digits);

	ret.Format(fmt, value);

	return ret;
}

bool cmString::IsHexChar(TCHAR c)
{
	int ret;

#ifdef _UNICODE
	ret = iswxdigit(c);
#else
	ret = isxdigit(c);
#endif

	return (ret != 0);
}

bool cmString::IsDecChar(TCHAR c)
{
	int ret;

#ifdef _UNICODE
	ret = iswdigit(c);
#else
	ret = isdigit(c);
#endif

	return (ret != 0);
}