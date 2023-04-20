#ifndef __STRINGFORMAT_H__
#define __STRINGFORMAT_H__

class CStringFormat
{
public:
	CStringFormat(LPCSTR szFormat,...);
	operator CString() const;
	operator LPCSTR() const;
protected:
	CString m_String;
};

#endif