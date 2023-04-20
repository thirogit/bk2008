// cmString.h: interface for the cmString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMSTRING_H_)
#define _CMSTRING_H_

#include <atlbase.h>

class cmString : public CString 
{
public:
	cmString();
	
	cmString(TCHAR* s);
	cmString(const TCHAR* s);
	cmString(const CString& s);

	cmString(const cmString& s);
	
	virtual ~cmString();

	const cmString& operator=(const TCHAR* s);
	const cmString& operator=(const cmString& s);
	const cmString& operator=(const CComBSTR& s);
	const cmString& operator=(const CString& s);
	const cmString& operator=(BSTR& s);
	const cmString& operator=(int value);
	const cmString& operator=(long value);
	
	cmString& operator+=(const TCHAR* s);
	cmString& operator+=(const cmString& s);
	cmString& operator+=(TCHAR s);
	cmString& operator+=(const CString& s);

	operator int() const;
	operator long() const;
	operator double() const;
	operator const char*() const;

	bool operator==(const CComBSTR& bstr) const;
	bool operator==(const TCHAR* s) const;
	bool operator==(const cmString& s) const;

	const TCHAR* c_str() const;

	bool IsDecimalNumber() const;
	bool IsHexadecimalNumber(const cmString& prefix = _T("0x")) const;
	long HexStringToNumber(const cmString& prefix = _T("0x")) const;

	static cmString FormatDecimalNumber(int digits, int precision, double value);
	static cmString FormatHexNumber(const cmString& hexPrefix, int digits, long value, bool lowerCase = true);
	
	static bool IsHexChar(TCHAR c);
	static bool IsDecChar(TCHAR c);

	friend cmString operator +(const cmString& s1, const TCHAR* s2 );
	friend cmString operator +(const TCHAR* s1, const cmString& s2);
    friend cmString operator +(const cmString& s1, const cmString& s2);
	friend cmString operator +(const cmString& s1, const CString& s2);
	friend cmString operator +(const CString& s1, const cmString& s2);
};



#endif // !defined(_CMSTRING_H_)
