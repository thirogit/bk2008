#ifndef __INTSTRING_H__
#define __INTSTRING_H__

#include "../Utils/StringFormat.h"

class CIntString : public CStringFormat
{
public:
	CIntString(int iInt);
	CIntString(UINT uInt);
	operator CString() const;
	operator LPCSTR() const;
};

#endif