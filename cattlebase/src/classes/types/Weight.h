#ifndef __WEIGHT_H__
#define __WEIGHT_H__

#include "NullDouble.h"
#include "Money.h"
#include "../format/FloatFormat.h"

#define WEIGHT_DEFAULT_PREC 3
#define ZEROWEIGHT (0.0)

class Weight 
{
public:
	Weight();
	explicit Weight(double dWeight);
	Weight(const Weight& copy);
	virtual void CopyFrom(const Weight& copy);
	virtual void	SetDouble(double dWeight);
	virtual double  GetDouble() const;
	virtual CString ToString(unsigned int precision = WEIGHT_DEFAULT_PREC) const;
	virtual CString ToString(const FloatFormat& fmt) const;
	bool Equals(const Weight& weight) const;
	
	friend bool operator==(const Weight &left ,const Weight &right);
	friend bool operator!=(const Weight &left ,const Weight &right);

	friend bool operator>(const Weight &left,const double right);
	friend bool operator<(const Weight &left,const double right);

	friend bool operator>(const Weight &left ,const Weight &right);
	friend bool operator<(const Weight &left ,const Weight &right);

	friend bool operator<=(const Weight &left ,const double right);
	friend bool operator>=(const Weight &left ,const double right);

	Weight& operator+=(const Weight& right);
	Weight& operator+=(const double right);
	Weight& operator=(double dWeight);
	Weight operator/(double dFactor);

	friend Weight operator-(const Weight &left,const Weight& right);
	friend Weight operator+(const Weight &left,const Weight& right);
	friend Money operator*(const Money& left,const Weight& right);
	friend Money operator*(const Weight& left,const Money& right);
	friend Money operator/(const Money& left,const Weight& right);
protected:
	double m_dWeight;
};

#endif