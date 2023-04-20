#include "stdafx.h"
#include "FloatFormat.h"
#include <math.h>

FloatFormat::FloatFormat()  : m_Precision(0),m_Separator('\0')
{
}

FloatFormat::FloatFormat(int precision,char sperator) : m_Precision(precision),m_Separator(sperator)
{

}

FloatFormat::FloatFormat(int precision) :  m_Precision(precision),m_Separator('.')
{
}

FloatFormat::FloatFormat(const FloatFormat& src)
{
	m_Precision = src.m_Precision;
	m_Separator = src.m_Separator;
}

char FloatFormat::GetDecimalPoint() const
{
	return m_Separator;
}

int FloatFormat::GetPrecision() const
{
	return m_Precision;
}

CString FloatFormat::Format(double dValue) const
{
	char fmtBuffer[100];
	cfltcvt(dValue,fmtBuffer,m_Precision,m_Separator);

	return fmtBuffer;
}

#define CVTBUFSIZE        (80)

void FloatFormat::cfltcvt(double value, char *buffer, int precision, char decimal_point)
{
  int decpt, sign, pos;
  char *digits = NULL;
  char cvtbuf[CVTBUFSIZE];
  int capexp = 0;

     digits = cvt(value, precision, &decpt, &sign, cvtbuf,0);
    if (sign) *buffer++ = '-';
    if (*digits)
    {
      if (decpt <= 0)
      {
        *buffer++ = '0';
        *buffer++ = decimal_point;
        for (pos = 0; pos < -decpt; pos++) *buffer++ = '0';
        while (*digits) *buffer++ = *digits++;
      }
      else
      {
        pos = 0;
        while (*digits)
        {
          if (pos++ == decpt) *buffer++ = decimal_point;
          *buffer++ = *digits++;
        }
      }
    }
    else
    {
      *buffer++ = '0';
      if (precision > 0)
      {
        *buffer++ = decimal_point;
        for (pos = 0; pos < precision; pos++) *buffer++ = '0';
      }
    }
 

  *buffer = '\0';
}


char * FloatFormat::cvt(double arg, int ndigits, int *decpt, int *sign, char *buf, int eflag)
{
  int r2;
  double fi, fj;
  char *p, *p1;

  if (ndigits < 0) ndigits = 0;
  if (ndigits >= CVTBUFSIZE - 1) ndigits = CVTBUFSIZE - 2;
  r2 = 0;
  *sign = 0;
  p = &buf[0];
  if (arg < 0)
  {
    *sign = 1;
    arg = -arg;
  }
  arg = modf(arg, &fi);
  p1 = &buf[CVTBUFSIZE];

  if (fi != 0) 
  {
    p1 = &buf[CVTBUFSIZE];
    while (fi != 0) 
    {
      fj = modf(fi / 10, &fi);
      *--p1 = (int)((fj + .03) * 10) + '0';
      r2++;
    }
    while (p1 < &buf[CVTBUFSIZE]) *p++ = *p1++;
  } 
  else if (arg > 0)
  {
    while ((fj = arg * 10) < 1) 
    {
      arg = fj;
      r2--;
    }
  }
  p1 = &buf[ndigits];
  if (eflag == 0) p1 += r2;
  *decpt = r2;
  if (p1 < &buf[0]) 
  {
    buf[0] = '\0';
    return buf;
  }
  while (p <= p1 && p < &buf[CVTBUFSIZE])
  {
    arg *= 10;
    arg = modf(arg, &fj);
    *p++ = (int) fj + '0';
  }
  if (p1 >= &buf[CVTBUFSIZE]) 
  {
    buf[CVTBUFSIZE - 1] = '\0';
    return buf;
  }
  p = p1;
  *p1 += 5;
  while (*p1 > '9') 
  {
    *p1 = '0';
    if (p1 > buf)
      ++*--p1;
    else 
    {
      *p1 = '1';
      (*decpt)++;
      if (eflag == 0) 
      {
        if (p > buf) *p = '0';
        p++;
      }
    }
  }
  *p = '\0';
  return buf;
}
