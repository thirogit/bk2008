#ifndef __MONEY_H__
#define __MONEY_H__

#include "NullDouble.h"
#include "Percent.h" 


#define MONEY_DEFAULT_PREC 2
#define ZEROMONEY (0.0)

class Money : public NullDouble
{
public:
	explicit Money(const double x);
	Money();
	Money(const Money& copy);
	Money(const NullDouble& copy);


	short MoneyCents() const;
	double MoneyInt() const;

	Money& operator+= (const Money& rs);
	Money& operator-= (const Money& rs);
	Money& operator*= (const double rs);
	Money& operator/= (const double rs);

	Money  operator- ()   const;
	Money  operator+ ()   const;

	Money  operator+ (const Money& rs) const;
	Money  operator- (const Money& rs) const;
	Money  operator* (const double rs) const;
	Money  operator/ (const double rs) const;
	double  operator/ (const Money& rs) const;
	Money& operator=(double right);

	/*bool operator== (const Money& rs) const;
	bool operator<  (const Money& rs) const;
	bool operator<= (const Money& rs) const;
	bool operator!= (const Money& rs) const;
	bool operator>  (const Money& rs) const;
	bool operator>= (const Money& rs) const;*/

	virtual double  GetDouble(const double ifNull = ZEROMONEY) const;
	virtual void	SetDouble(double dValue);	
	virtual CString ToString(LPCSTR ifNull = NULL,unsigned int precision = MONEY_DEFAULT_PREC) const;
    virtual CString ToString(LPCSTR ifNull,const FloatFormat& fmt) const;

	bool Split(double &integer,double &fraction) const;
	
protected:
	bool doubleEquals(const double d1,const double d2,const double eps = 0.5);
	double round() const; 	
	
};


//Money  operator+  (const double ls, const Money&  rs);
Money  operator*  (const Percent& ls, const Money&  rs);
Money  operator*  (const Money& ls, const Percent& rs);
Money  operator*  (const double ls, const Money&  rs);
//Money  operator-  (const double ls, const Money&  rs);
//bool  operator== (const double ls, const Money& rs);
//bool  operator<  (const double ls, const Money& rs);
//bool  operator<= (const double ls, const Money& rs);
//bool  operator!= (const double ls, const Money& rs);
//bool  operator>  (const double ls, const Money& rs);
//bool  operator>= (const double ls, const Money& rs);

#endif
