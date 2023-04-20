#include "stdafx.h"
#include "Weight.h"
#include <math.h>
Weight::Weight()
{
	m_dWeight = 0.0;
}

Weight::Weight(double dWeight) 
{
	SetDouble(dWeight);
}

Weight::Weight(const Weight& copy)
{	
	CopyFrom(copy);
}

void Weight::CopyFrom(const Weight& copy)
{
	m_dWeight = copy.m_dWeight;
}


void Weight::SetDouble(double dWeight)
{
	m_dWeight = dWeight;	
}

double  Weight::GetDouble() const
{
	return m_dWeight;
}

CString Weight::ToString(unsigned int precision/* = WEIGHT_DEFAULT_PREC*/) const
{
	int goodPrecision = precision < 2 ? 2 : precision; 
	CString s;

	s.Format("%.*f",goodPrecision,GetDouble());
	return s;
}

CString Weight::ToString(const FloatFormat& fmt) const
{
	return fmt.Format(GetDouble());
}

Weight& Weight::operator=(double dWeight)
{
	m_dWeight = dWeight;
	return *this;
}


Weight Weight::operator/(double dFactor)
{
	return Weight(GetDouble()/dFactor);
}


Weight& Weight::operator+=(const Weight& right)
{
	m_dWeight += right.m_dWeight;
	return *this;
}

Weight& Weight::operator+=(const double right)
{
	m_dWeight += right;
	return *this;
}

bool operator==(const Weight &left ,const Weight &right)
{
	return left.GetDouble() == right.GetDouble();
	/*double diff = fabs(left.GetDouble() - right.GetDouble());
	return diff < 0.001;*/
}


bool operator!=(const Weight &left ,const Weight &right)
{	
	return !(left == right);
}


bool operator>(const Weight &left,const double right)
{
	return left.GetDouble() > right;
}

bool operator<(const Weight &left,const double right)
{
	return left.GetDouble() < right;
}

Weight operator-(const Weight &left,const Weight& right)
{
	return Weight(left.GetDouble() - right.GetDouble());
}

Weight operator+(const Weight &left,const Weight& right)
{
	return Weight(left.GetDouble() + right.GetDouble());
}

Money operator*(const Money& left,const Weight& right)
{
	return left*right.GetDouble();
}

Money operator*(const Weight& left,const Money& right)
{
	return operator*(right,left);
}

Money operator/(const Money& left,const Weight& right)
{
	return left/right.GetDouble();
}

bool operator>(const Weight &left ,const Weight &right)
{
	return left.GetDouble() > right.GetDouble();
}
bool operator<(const Weight &left ,const Weight &right)
{
	return  right > left;
}

bool operator<=(const Weight &left ,const double right)
{
	return left.GetDouble() <= right;
}