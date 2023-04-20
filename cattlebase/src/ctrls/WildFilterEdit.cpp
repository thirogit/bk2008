#include "stdafx.h"
#include "WildFilterEdit.h"



CWildFilterEdit::CWildFilterEdit(int nAlphaChars,int nDigitChars) : CFilterEdit()
{
	char szAlphaFmt[] = "^[a-zA-Z*?]{0,%d}",szAlphaBuff[20],
		szDigitFmt[] = "[0-9*?]{0,%d}",szDigitBuff[20],
		szRegExBuff[40] = {0};

	if(nAlphaChars)
	{
		sprintf(szAlphaBuff,szAlphaFmt,nAlphaChars);
        strcpy(szRegExBuff,szAlphaBuff);
	}

	sprintf(szDigitBuff,szDigitFmt,nDigitChars);
	strcat(szRegExBuff,szDigitBuff);

	SetRegEx(szRegExBuff);
}

CString CWildFilterEdit::GetWindowText()
{
	CString rString;
	CFilterEdit::GetWindowText(rString);
	rString.MakeUpper();
	return rString;
}






