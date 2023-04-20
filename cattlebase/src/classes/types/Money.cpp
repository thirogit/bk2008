#include "stdafx.h"
#include "Money.h"

#include <math.h>         /* For fmod, modf, floor, log10                 */
#include <limits>
#include <float.h>

#define SCALE (100.0)

Money::Money(const double x) : NullDouble(x * SCALE) 
{
}

Money::Money() 
{
	
}      

Money::Money(const Money& copy)
{
	CopyFrom(copy);
}

Money::Money(const NullDouble& copy)
{
	CopyFrom(copy*SCALE);
}

double Money::round() const          
 {
	double dummy;                              
	modf(m_dValue + (m_dValue < 0 ? -.5 : .5),&dummy);
	return dummy;
 }


bool Money::doubleEquals(const double d1,const double d2,const double eps/* = 0.5*/)
{
	double diff = fabs(d1-d2);
	return (diff < fabs(eps));
}
double  Money::GetDouble(const double ifNull/* = ZEROMONEY*/) const
{
	if(IsNull())
		return ifNull;
	
	return m_dValue/SCALE;
}

void Money::SetDouble(double dValue)
{
	m_dValue = dValue*SCALE;
}


bool Money::Split(double &integer,double &fraction) const
{
	if(IsNull())
		return false;

	fraction = modf(round()/ SCALE, &integer);
	return true;
}

short Money::MoneyCents() const   
{
	if(IsNull())
		return 0;

	double intPart; 
	double fraction;

	Split(intPart,fraction);

	return short(fraction * 100);
}
double Money::MoneyInt() const
{
	if(IsNull())
		return ZEROMONEY;

	double intPart; 
	double fraction;
	Split(intPart,fraction);
	return intPart;

}

CString Money::ToString(LPCSTR ifNull,const FloatFormat& fmt) const
{
	return NullDouble::ToString(ifNull,fmt);
}

CString Money::ToString(LPCSTR ifNull/* = NULL*/,unsigned int precision/* = MONEY_DEFAULT_PREC*/) const
{
	return NullDouble::ToString(ifNull,precision);
}

Money& Money::operator+= (const Money& rs) 
{
	if(IsNull() || rs.IsNull())
		MakeNull();
	else
		m_dValue += rs.m_dValue;

	return *this;
}
Money& Money::operator-= (const Money& rs) 
{
	if(IsNull() || rs.IsNull())
		MakeNull();
	else
		m_dValue -= rs.m_dValue;
	return *this;
}

Money& Money::operator*= (const double rs) 
{
	if(!IsNull())
		m_dValue = m_dValue*rs;
	return *this;
}

Money& Money::operator/= (const double rs) 
{
	if(!IsNull())
		m_dValue = m_dValue/rs;
	return *this;
}

Money  Money::operator- ()   const   
{
	Money  result;  
	if(!IsNull())
		result.m_dValue = -m_dValue;  
	
	return result;
}

Money  Money::operator+ ()   const              
{
	return *this; 
}

Money  Money::operator+ (const Money&  rs)        const 
{
	return Money(*this) += rs;
}
Money  Money::operator- (const Money&  rs)        const 
{
	return Money(*this) -= rs;
}


Money  Money::operator* (const double rs)        const 
{
	return Money(*this) *= rs;
}
Money  Money::operator/ (const double rs)        const 
{
	return Money(*this) /= rs;
}

double  Money::operator/ (const Money& rs)         const 
{
	if(IsNull() || rs.IsNull())
		return ZEROMONEY;

	return m_dValue / rs.m_dValue;
}

Money& Money::operator=(double right)
{
	SetDouble(right);
	return *this;
}

//
//bool Money::operator>=(const Money& right) const
//{
//	if(IsNull() && right.IsNull()) return true;
//	if(IsNull() || right.IsNull()) return false;
//
//	return	m_dValue   >= right.m_dValue;
//}
//bool Money::operator<=(const Money& right) const
//{
//	if(IsNull() && right.IsNull()) return true;
//	if(IsNull() || right.IsNull()) return false;
//
//	return m_dValue <= right.m_dValue;
//}
//bool Money::operator>(const Money& right) const
//{
//	if(IsNull())
//	{
//		return false;
//	}
//	else
//		if(right.IsNull()) return true;
//
//	return m_dValue > right.m_dValue;
//		
//}
//bool Money::operator<(const Money& right) const
//{	
//	return !operator>(right);
//}
//bool Money::operator==(const Money& right) const
//{
//	if(IsNull() && right.IsNull()) return true;
//	if(right.IsNull() || IsNull()) return false;
//
//	return m_dValue == right.m_dValue;
//}
//bool Money::operator!=(const Money& right) const
//{
//	return !(*this == right);
//}

Money  operator*  (const Percent& ls, const Money&  rs)
{
	if(rs.IsNull())
		return Money();
	
	return Money(rs.GetDouble()*ls.GetDouble());
}

Money  operator*  (const Money& ls, const Percent& rs)
{
	return rs*ls;
}

Money  operator*  (const double ls, const Money&  rs)
{
	if(rs.IsNull())
		return Money();

	return Money(rs.GetDouble()*ls);
}