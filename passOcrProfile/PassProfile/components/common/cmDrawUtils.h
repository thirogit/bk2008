// DrawUtils.h
//

#pragma once

#include "cmString.h"
#include <map>

class cmFontInfo;

using namespace std;


class cmDrawUtils
{
public:
	
	enum enBmp {
		enBmp_AlignLeft			= 1,
		enBmp_AlignRight		= 2,
		enBmp_AlignCenter		= 4,
		enBmp_AlignTop			= 8,
		enBmp_AlignVertCenter	= 16,
		enBmp_AlignBottom		= 32,
		enBmp_Tile				= 64
	};

	static void DrawVerticalGradient(CDC* pDC, CRect& rc, COLORREF topColor, COLORREF bottomColor, float perc);
	static void DrawHorizontalGradient(CDC* pDC, const CRect& rc, COLORREF leftColor, COLORREF rightColor, float perc);

	static void DrawAppleGradient(CDC* pDC, 
								  const CRect& rc, 
								  COLORREF topColor, 
								  COLORREF middleColor1, 
								  COLORREF middleColor2, 
								  COLORREF bottomColor);

	static void DrawFilledRect(CDC* pDC, const CRect& rc, COLORREF bkColor);

	static CRect DrawBitmap(CDC* pDC, UINT bmpResourceId, const CRect& rc = CRect(0, 0, 0, 0), UINT flags = enBmp_AlignLeft | enBmp_AlignTop);
	static CRect DrawBitmap(CDC* pDC, UINT bmpResourceId, const CPoint& topLeft, UINT flags);

	static CRect DrawBitmapTransparent(CDC* pDC, UINT bmpResourceId, const CRect& rc = CRect(0, 0, 0, 0), COLORREF transparentColor = RGB(0, 0, 0), UINT flags = enBmp_AlignLeft | enBmp_AlignTop);
	static CRect DrawBitmapTransparent(CDC* pDC, UINT bmpResourceId, const CPoint& topLeft, COLORREF transparentColor, UINT flags);

	static void DrawString(CDC* pDC, const cmString& str, CRect& rc, UINT format, cmString fontName = _T("tahoma"), int fontSize = -12, COLORREF textColor = 0, bool fontBold = false);
	static void DrawString(CDC* pDC, const cmString& str, CRect& rc, UINT format, COLORREF textColor, CFont* font);
	static void DrawString(CDC* pDC, const cmString& str, CRect& rc, UINT format, COLORREF textColor, const cmFontInfo& fontInfo);

	static CSize GetBitmapSize(UINT bmpResourceId);

	static void DrawIcon(CDC* pDC, UINT icoResourceId, const CPoint& topLeft, int size);
	static void DrawRect(CDC* pDC, const CRect& rc, COLORREF borderColor);

	static COLORREF MakeColorDarker(COLORREF color, BYTE offset);
	static COLORREF MakeColorBrighter(COLORREF color, BYTE offset);
	static COLORREF MakeColor(COLORREF baseColor, short maxColorOffset, short midColorOffset, short minColorOffset);
	static COLORREF MakeColor2(COLORREF baseColor, short offsetR, short offsetG, short offsetB);

	static void DrawLine(CDC* pDC, const CPoint& pt1, const CPoint& pt2, COLORREF color);

private:
	static HBITMAP CreateMask(CDC* pDC, COLORREF clrTrans, const CRect& pictureArea);

	static multimap<int, int> colors_;
};
