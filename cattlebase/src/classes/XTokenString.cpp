// XTokenString.cpp  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// Description:
//     XTokenString implements a function to extract tokens from a string,
//     similar to strtok().  The tokens are returned in the CStringArray
//     passed by the caller. For more details, please see article at 
//     www.codeproject.com.
//
// History
//     Version 1.0 - 2005 August 2
//     - Initial public release
//
// Public APIs:
//          NAME                           DESCRIPTION
//     ---------------  -----------------------------------------------------
//     XTokenString()   Extract tokens from string
//
// License:
//     This software is released into the public domain.  You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XTokenString.h"

///////////////////////////////////////////////////////////////////////////////
//
// XTokenString()
//
// Purpose:     XTokenString parses lpszString to find tokens delimited by 
//              lpszDelimiters.  The tokens are returned in the saTokens
//              array, until a maximum of nMaxTokens are found (0 = no max.).
//              Options may be used to select whitespace trimming, handling of 
//              escape characters, double quotes, and empty tokens.
//
// Parameters:  lpszString          - address of string containing tokens
//              lpszDelimiters      - set of delimiter characters
//              saTokens            - array to hold returned tokens
//              nMaxTokens          - max no. of tokens to return;  default
//                                    is 0, meaning no limit.
//              bTrimToken          - TRUE = trim left and right whitespace
//                                    from returned token.  Default = FALSE.
//              bEnableEscapedChars - TRUE = enable handling of escape character
//                                    sequences such as \".  Default = FALSE.
//              bEnableDoubleQuote  - TRUE = enable handling of double quotes.
//                                    Default = FALSE.
//              bReturnEmptyToken   - TRUE = return empty tokens (delimiters 
//                                    with no content).  Used mostly for
//                                    CSV-type records, where number of fields
//                                    is fixed.  Default = FALSE.
//
// Returns:     int - number of tokens returned;  0 if error.
//
int XTokenString(LPCTSTR lpszString, 
				 LPCTSTR lpszDelimiters,
				 CStringArray& saTokens,
				 int nMaxTokens /*= 0*/,
				 BOOL bTrimToken /*= FALSE*/,
				 BOOL bEnableEscapedChars /*= FALSE*/,
				 BOOL bEnableDoubleQuote /*= FALSE*/,
				 BOOL bReturnEmptyToken /*= FALSE*/)
{
	// Note:  normally tokens will be allowed to accumulate;
	//        If you do not want to accumulate, uncomment
	//        the following line

	//saTokens.RemoveAll();

	ASSERT(lpszString && (lpszString[0] != _T('\0')));
	if ((lpszString == NULL) || (lpszString[0] == _T('\0')))
		return 0;

	ASSERT(lpszDelimiters && (lpszDelimiters[0] != _T('\0')));
	if ((lpszDelimiters == NULL) || (lpszDelimiters[0] == _T('\0')))
		return 0;

	BOOL bEndedWithDelim = FALSE;
	const TCHAR * cp = lpszString;

	if (!bReturnEmptyToken)
		cp += _tcsspn(cp, lpszDelimiters);			// skip leading delimiters

	// loop to find all tokens

	while (*cp)
	{
		const TCHAR * pos = NULL;
		CString strToken = _T("");

		// skip leading whitespace;  normally this will not cause a problem,
		// since it is unlikely that you would want to return empty tokens
		// when whitespace is the delimiter
		while ((*cp == _T(' ')) || (*cp == _T('\t')))
			strToken += *cp++;

		BOOL bInQuote = FALSE;

		if (bEnableDoubleQuote && (*cp == _T('"')))
		{
			bInQuote = TRUE;
			++cp;							// skip " char
		}

		// loop to accumulate token
		for (pos = cp; *pos; pos++)
		{
			// check for end of token - either a quote or a delimiter
			if (bInQuote)
			{
				if (*pos == _T('"'))
					break;
			}
			else
			{
				if (_tcsspn(pos, lpszDelimiters) != 0)
					break;
			}
		
			if (bEnableEscapedChars && (*pos == _T('\\')))
			{
				pos++;						// skip '\'
				if (*pos == _T('\0'))
					break;
			}
			strToken += *pos;
		}
		cp = pos;

		// check if string ended with delimiter
		if (_tcsspn(cp, lpszDelimiters))
			bEndedWithDelim = TRUE;
		else
			bEndedWithDelim = FALSE;

		// skip ending delimiter/quote
		if (*cp)
			cp++;

		if (bInQuote)
		{
			// if quote ended with delimiter, skip it
			if (_tcsspn(cp, lpszDelimiters))
				cp++;
		}

		if (bTrimToken)
		{
			strToken.TrimLeft();
			strToken.TrimRight();
		}

		if ((!strToken.IsEmpty()) || bReturnEmptyToken)
		{
			// either token is not empty OR caller wants empty tokens

			if ((nMaxTokens > 0) && (saTokens.GetSize() >= nMaxTokens))
			{
				TRACE(_T("XTokenString:  ERROR - too many tokens (max is %d)\n"), nMaxTokens);
				break;
			}

			TRACE(_T("XTokenString:  adding <%s>\n"), strToken);
			saTokens.Add(strToken);
		}
	}

	// add empty token if caller wants it
	if (bReturnEmptyToken && bEndedWithDelim)
		saTokens.Add(_T(""));

	return saTokens.GetSize();
}
