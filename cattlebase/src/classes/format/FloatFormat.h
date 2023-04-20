#ifndef __FLOATFORMAT_H__
#define __FLOATFORMAT_H__

class FloatFormat
{
public:
	FloatFormat();
	FloatFormat(int precision,char sperator);
	FloatFormat(int precision);
	FloatFormat(const FloatFormat& src);
	CString Format(double dValue) const;
	char GetDecimalPoint() const;
	int GetPrecision() const;
protected:
	static void cfltcvt(double value, char *buffer, int precision, char decimal_point);
	static char * cvt(double arg, int ndigits, int *decpt, int *sign, char *buf, int eflag);
protected:
	int  m_Precision;
	char m_Separator;
};

#endif