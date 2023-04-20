#include "stdafx.h"
#include "Percent.h"
#include "../../Utils/StringFormat.h"

Percent::Percent() 
{
	MakeNull();
}


Percent::Percent(double dPercent) : NullDouble(dPercent)
{

}
Percent::Percent(const Percent& copy)
{
	CopyFrom(copy);
}

Percent& Percent::operator=(const Percent &right)
{
	CopyFrom(right);
	return *this;
}

Percent& Percent::operator=(double dPercent)
{
	SetDouble(dPercent);
	return *this;
}

CString Percent::To100String(LPCSTR szIfNull/* = NULL*/,int prec/* = 2*/) const
{
	if(IsNull())
	{
		if(szIfNull == NULL)
			return "";
		else
			return szIfNull;
	}
	else
	{
		int validPrecision = prec;
		if(validPrecision > MAX_PRECISION)
			validPrecision = MAX_PRECISION;
		return CStringFormat("%.*f",validPrecision,m_dValue*100.0);
	}
}


bool Percent::operator>=(const Percent& right)
{
	if(IsNull() && right.IsNull()) return true;
	if(IsNull() || right.IsNull()) return false;

	return	m_dValue   >= right.m_dValue;
}
bool Percent::operator<=(const Percent& right)
{
	if(IsNull() && right.IsNull()) return true;
	if(IsNull() || right.IsNull()) return false;

	return m_dValue <= right.m_dValue;
}
bool Percent::operator>(const Percent& right)
{
	if(IsNull())
	{
		return false;
	}
	else
		if(right.IsNull()) return true;

	return m_dValue > right.m_dValue;
		
}
bool Percent::operator<(const Percent& right)
{	
	return !operator>(right);
}
bool Percent::operator==(const Percent& right)
{
	if(IsNull() && right.IsNull()) return true;
	if(right.IsNull() || IsNull()) return false;

	return m_dValue == right.m_dValue;
}
bool Percent::operator!=(const Percent& right)
{
	return !(*this == right);
}
