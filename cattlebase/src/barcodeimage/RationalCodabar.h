// RationalCodabar.h: interface for the CRationalCodabar class.
//
//	Copyright 2002 Neil Van Eps
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RATIONALCODABAR_H__FDB6DE66_1A22_4654_BF67_C6C7B0D16413__INCLUDED_)
#define AFX_RATIONALCODABAR_H__FDB6DE66_1A22_4654_BF67_C6C7B0D16413__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Barcode.h"

class CRationalCodabar : public CBarcode  
{
public:
	void DrawBitmap();
	void BitmapToClipboard();
	CRationalCodabar();
	virtual ~CRationalCodabar();

private:
	CString RetrievePattern(char c);
	void DrawPattern(CString csPattern);
};

#endif // !defined(AFX_RATIONALCODABAR_H__FDB6DE66_1A22_4654_BF67_C6C7B0D16413__INCLUDED_)
