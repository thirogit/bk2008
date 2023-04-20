#include "stdafx.h"
#include "StringPart.h"

CString CStringPart::StringPart(CString str, char cDelimiter, int nIndex)
{
	if (str.Find(cDelimiter) == -1) return str;

	int iPos = 0;
	int iEnd;
	CString str2;

	while (nIndex--)
	{
		iPos = str.Find(cDelimiter, iPos) + 1;
		if (!iPos) return str2;
	}

	iEnd = str.Find(cDelimiter, iPos);
	if (iEnd == -1) iEnd = str.GetLength();

	str2 = str.Mid(iPos, iEnd - iPos);

	return str2;
}

int CStringPart::StringPartCount(CString str, char cDelimiter)
{
	if (str.IsEmpty()) return 0;
	return str.Remove(cDelimiter) + 1;
}

int CStringPart::StringPartBegin(CString str, char cDelimiter, int nIndex)
{
	if (str.Find(cDelimiter) == -1) return 0;
	if (str.IsEmpty()) return 0;
	if (nIndex == 0) return 0;

	int iPos = 0;
	while (nIndex--)
	{
		iPos = str.Find(cDelimiter, iPos) + 1;
		if (!iPos) return iPos;
	}

	return iPos;
}



