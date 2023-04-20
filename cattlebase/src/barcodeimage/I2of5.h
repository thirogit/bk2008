// I2of5.h: interface for the CI2of5 class.
//
//	Copyright 2002 Neil Van Eps
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_I2OF5_H__B02700B9_DD08_4B2C_B966_47F1275323B4__INCLUDED_)
#define AFX_I2OF5_H__B02700B9_DD08_4B2C_B966_47F1275323B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Barcode.h"

class CI2of5 : public CBarcode  
{
public:
	void BitmapToClipboard();
	void DrawBitmap();
	CI2of5();
	virtual ~CI2of5();
private:
	CString RetrievePattern(int nTwoDigitNumber);
	void DrawPattern(CString csCharPattern);

};

#endif // !defined(AFX_I2OF5_H__B02700B9_DD08_4B2C_B966_47F1275323B4__INCLUDED_)
