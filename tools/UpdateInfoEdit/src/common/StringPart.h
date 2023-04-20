#pragma once

class CStringPart
{
public:
	static CString StringPart(CString str, char cDelimiter, int nIndex);
	
	static int StringPartCount(CString str, char cDelimiter);
	
	static int StringPartBegin(CString str, char cDelimiter, int nIndex);
	
};