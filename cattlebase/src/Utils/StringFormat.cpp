#include "stdafx.h"
#include "StringFormat.h"

CStringFormat::CStringFormat(LPCSTR szFormat,...)
{	
  va_list vl;
  va_start(vl,szFormat);
  m_String.FormatV(szFormat,vl);
  va_end(vl);
}

CStringFormat::operator CString()  const
{
	return m_String;
}
CStringFormat::operator LPCSTR() const
{
	return (LPCSTR)m_String;
}

