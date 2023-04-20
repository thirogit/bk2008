// Code93.h: interface for the CCode93 class.
//
//	Copyright 2002 Neil Van Eps
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODE93_H__9559F813_425A_11D6_8807_00104B2B0208__INCLUDED_)
#define AFX_CODE93_H__9559F813_425A_11D6_8807_00104B2B0208__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Barcode.h"

class CCode93 : public CBarcode  
{
public:
	CCode93();
	virtual ~CCode93();
	void BitmapToClipboard();
	void DrawBitmap();
private:
	void	ASCIItoCode93Sequence( long nASCIINumber,long *nFirstNumber, long *nSecondNumber);
	void	DrawCheckDigits();
	void	DrawPattern(CString csPattern);
	CString	RetrievePattern( long c );

};

#endif // !defined(AFX_CODE93_H__9559F813_425A_11D6_8807_00104B2B0208__INCLUDED_)
