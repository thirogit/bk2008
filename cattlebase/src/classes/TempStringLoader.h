#ifndef __TEMPSTRINGLOADER_H__
#define __TEMPSTRINGLOADER_H__


class CTempStringLoader
{
public:
	CTempStringLoader(UINT uResourceID);
	operator CString() const;
	operator LPCSTR() const;

protected:
	CString m_String;
};

#endif