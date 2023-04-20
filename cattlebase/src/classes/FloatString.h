#ifndef __FLOATSTRING_H__
#define __FLOATSTRING_H__

#include "../Utils/StringFormat.h"

class CFloatString : public CStringFormat
{
public:
	CFloatString(double dFloat, int precision);
	operator CString() const;
	operator LPCSTR() const;
};

#endif