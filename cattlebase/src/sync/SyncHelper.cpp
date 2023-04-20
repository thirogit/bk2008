#include "stdafx.h"
//#include "global_arrays.h"
//#include <set>
#include "../missing/util.h"
#include "SyncHelper.h"


const char SyncStringFmtHelper::quotedFmt[] = "\"%s\"";

CString SyncStringFmtHelper::DecoceEscapeString(const CString & aString)
{
	char escapeChar = '\\';
	CString retStr;
	char oneChar;

	for(int i = 0,l = aString.GetLength();i<l;i++)
	{
		oneChar = aString.GetAt(i);
		if(oneChar == '\\') continue;
		retStr += oneChar;
	}
	return retStr;
}

void SyncStringFmtHelper::FormatDate(char *outBuff,int maxsize,const CDateTime & aDate)
{
	ASSERT(maxsize);

	if(aDate.IsNull())
		outBuff[0] = '\0';
	else
		strncpy(outBuff,aDate.GetDateInFmt(CDateTime::YYYYdashMMdashDD),maxsize);
	

}

CString SyncStringFmtHelper::EncodeDoubleQuotes(const CString & aString)
{
	int aLen = aString.GetLength();
	char aChar;
	CString outStr;
	char *outBuff = outStr.GetBuffer(aLen*2 + 1);
	int i,j;
	for(i = 0,j = 0;i < aLen;i++,j++)
	{
		aChar = aString.GetAt(i);
		if(aChar == '"')
		{
			outBuff[j++] = '"';
			outBuff[j] = '"';
		}
		else
			outBuff[j] = aChar;
	}
	outBuff[j] = '\0';
	outStr.ReleaseBuffer();
	return outStr;
}

CString SyncStringFmtHelper::FormatString(const CString & aString,bool unPolish/* = true*/)
{
	CString toEncode,sResult;
	
	if(!aString.IsEmpty())
	{
		if(unPolish)
			toEncode = CUtil::UnPolishString(aString);
		else
			toEncode = aString;

		sResult = EncodeSpecialCharacters(toEncode);
	}

	return sResult;
}

void SyncStringFmtHelper::FormatInt(char *outBuff,int maxsize,const NullInt & aInt)
{
	ASSERT(maxsize);

	if(aInt.IsNull())
		outBuff[0] = '\0';
	else
		aInt.ToString(outBuff,maxsize);

}

void SyncStringFmtHelper::FormatDouble(char *outBuff,int maxsize,const NullDouble& aDouble)
{
	ASSERT(maxsize);
	strncpy(outBuff,(LPCSTR)aDouble.ToString(NULL,5),maxsize);
}

CString SyncStringFmtHelper::EncodeSpecialCharacters(const CString& inString)
{
	CString outString;
	char c;
	for(int i = 0, l = inString.GetLength();i < l;i++)
	{
		c = inString.GetAt(i);
		if(IsSpecialCharacter(c))
			outString.AppendFormat("%%%02X",c);
		else
			outString.AppendChar(c);
	}
	return outString;

}

bool SyncStringFmtHelper::IsSpecialCharacter(char c)
{	
	const char szSpacialCharacters[] = {'"',',','%'};

	for(int i = 0;i < sizeof(szSpacialCharacters);i++)
	{
		if(c == szSpacialCharacters[i])
			return true;
	}
	return false;
}









