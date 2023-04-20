// XTokenString.h  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// This software is released into the public domain.  You are free to use
// it in any way you like, except that you may not sell this source code.
//
// This software is provided "as is" with no expressed or implied warranty.
// I accept no liability for any damage or loss of business that this
// software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef  XTOKENSTRING_H 
#define  XTOKENSTRING_H 

int XTokenString(LPCTSTR lpszString, 
				 LPCTSTR lpszDelimiters,
				 CStringArray& saTokens,
				 int nMaxTokens = 0,
				 BOOL bTrimToken = FALSE,
				 BOOL bEnableEscapedChars = FALSE,
				 BOOL bEnableDoubleQuote = FALSE,
				 BOOL bReturnEmptyToken = FALSE);

#endif // XTOKENSTRING_H 
