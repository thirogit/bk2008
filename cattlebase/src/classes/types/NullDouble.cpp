#include "stdafx.h"
#include "NullDouble.h"
#include <limits>
#include <float.h>


NullDouble::NullDouble()
{
	MakeNull();
}

NullDouble::NullDouble(double dValue) 
{
	SetDouble(dValue);
}

NullDouble::NullDouble(const NullDouble& copy)
{	
	CopyFrom(copy);
}

void NullDouble::CopyFrom(const NullDouble& copy)
{
	m_dValue = copy.m_dValue;
}

void NullDouble::MakeNull()
{
	m_dValue = std::numeric_limits<double>::quiet_NaN();
}

bool NullDouble::IsNull() const
{
	return _isnan(m_dValue) != 0;
}

void NullDouble::SetDouble(double dValue)
{
	m_dValue = dValue;	
}

double  NullDouble::GetDouble(double dIfNull/* = 0.0*/) const
{
	if(IsNull())
		return dIfNull;

	return m_dValue;
}


CString NullDouble::ToString(LPCSTR ifNull/* = NULL*/,unsigned int precision/* = NULLDOUBLE_DEFAULT_PREC*/) const
{
	int goodPrecision = precision < 2 ? 2 : precision;
	return ToString(ifNull,FloatFormat( goodPrecision,'.'));
	//CString s;
	//s.Format("%.*f",goodPrecision,GetDouble());
	//return s;
}

CString NullDouble::ToString(LPCSTR ifNull,const FloatFormat& fmt) const
{
	if(IsNull())
	{
		if(ifNull != NULL)
			return ifNull;
		else
			return "";
	}
	return fmt.Format(GetDouble());

}


NullDouble& NullDouble::operator=(const double right)
{
	SetDouble(right);
	return *this;
}



NullDouble& operator +=(NullDouble &left ,NullDouble &right)
{
	if(!right.IsNull()) 
	{
		if(!left.IsNull())
			left.m_dValue += right.m_dValue;
		else left.m_dValue = right.m_dValue;
	}
	return left;
    
}

NullDouble& operator+=(NullDouble &left ,const double right)
{
	if(!left.IsNull())
	{
		left.m_dValue += right;
	}
	return left;
}

bool operator<(const NullDouble &left ,double right)
{
	if(left.IsNull())
		return false;

	return left.GetDouble() < right;
}

bool operator>(const NullDouble &left ,double right)
{
	if(left.IsNull())
		return false;

	return left.GetDouble() > right;
}

bool operator!=(const NullDouble &left ,const NullDouble &right)
{
	return !(left == right);
}

bool operator==(const NullDouble &left ,const NullDouble &right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();
	
	if(l && r) return true;
	else if(r || l) return false;

	return (left.m_dValue == right.m_dValue);
}


bool operator>(const NullDouble& left,const NullDouble& right)
{
	if(!left.IsNull()) 
	{
		if(right.IsNull()) return true; //nie-null zawsze wiekszy od null
		else return left.m_dValue > right.m_dValue; 
	}
	return false; //nawet jezeli right nie jest null to left=null > right=nie-null
			     // a gdy oba rowne null to takze zwracamy false bo nie pytamy sie o rownosc
}


bool operator<(const NullDouble &left ,const NullDouble &right)
{
	return right > left;
}


bool operator>=(const NullDouble &left ,const NullDouble &right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();

	if(l && r) return true;
    if(r) return true;
	if(l) return false;

	return left.m_dValue >= right.m_dValue;
}

bool operator<=(const NullDouble &left ,const NullDouble &right)
{
	return right >= left;
}

bool operator>=(const NullDouble &left ,double right)
{
	if(left.IsNull())
		return false;
	return left.GetDouble() >= right;
}

bool operator<=(const NullDouble &left ,double right)
{
	return right >= left;
}
	
bool operator>=(const double left,const NullDouble &right)
{
	if(right.IsNull())
		return true;
	return left >= right.GetDouble();
}

bool operator!=(const NullDouble &left ,const double right)
{
	if(left.IsNull())
		return true;

	return left.GetDouble() != right;
}

bool operator==(const NullDouble &left ,const double right)
{
	return left != right;
}

NullDouble operator*(const NullDouble &left, const double right)
{
	if(left.IsNull())
		return NullDouble();

	return NullDouble(left.GetDouble()*right);
}