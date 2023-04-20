#ifndef __PNG_H__
#define __PNG_H__

#include "CGdiPlusBitmap.h"

class CPng 
{
public:
	bool LoadPNG(const CString& sPNGPath);
	bool LoadPNG(UINT nIDResource,LPCSTR pszResourceType = "PNG");
	HBITMAP GetBitmap24(COLORREF bkgColor);
	//HBITMAP GetBitmap32();
private:
	CGdiPlusBitmapResource m_PNG;
};
#endif