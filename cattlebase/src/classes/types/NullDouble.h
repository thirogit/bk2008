#ifndef __NULLDOUBLE_H__
#define __NULLDOUBLE_H__

#include "Nullable.h"
#include "../format/FloatFormat.h"

#define NULLDOUBLE_DEFAULT_PREC 6

class NullDouble : public Nullable
{
public:
	NullDouble();
	explicit NullDouble(double dValue);
	NullDouble(const NullDouble& copy);
	virtual void CopyFrom(const NullDouble& copy);
	virtual void	SetDouble(double dValue);
	virtual double  GetDouble(const double dIfNull = 0.0) const;
	virtual CString ToString(LPCSTR ifNull = NULL,unsigned int precision = NULLDOUBLE_DEFAULT_PREC) const;
    virtual CString ToString(LPCSTR ifNull,const FloatFormat& fmt) const;

	void MakeNull();
	bool IsNull() const;

	NullDouble& operator=(const double right); 
	

	friend bool operator==(const NullDouble &left ,const NullDouble &right);
	friend bool operator==(const NullDouble &left ,const double right);
	friend bool operator!=(const NullDouble &left ,const NullDouble &right);
	friend bool operator!=(const NullDouble &left ,const double right);


	friend bool operator>(const NullDouble &left ,const double right);
	friend bool operator<(const NullDouble &left ,const double right);
	friend bool operator>(const NullDouble &left ,const NullDouble &right);
	friend bool operator<(const NullDouble &left ,const NullDouble &right);
	friend bool operator>=(const NullDouble &left ,const double right);
	friend bool operator<=(const NullDouble &left ,const double right);
	friend bool operator>=(const NullDouble &left ,const NullDouble &right);
	friend bool operator<=(const NullDouble &left ,const NullDouble &right);
	friend bool operator>=(const double left,const NullDouble &right);
	friend bool operator<=(const double left,const NullDouble &right);
	
	friend NullDouble operator*(const NullDouble &left, const double right);

	friend NullDouble& operator+=(NullDouble &left ,NullDouble &right);
	friend NullDouble& operator+=(NullDouble &left ,const double right);

protected:
	double m_dValue;
};

#endif