// DrawUtils.cpp
//

#include "stdafx.h"
#include "DrawUtils.h"
#include "VMBitmap.h"

void DrawUtils::DrawGradient(CDC* pDC, CRect& rc, COLORREF topColor, COLORREF bottomColor, float perc)
{
	double rStart;
	double gStart;
	double bStart;
	double rEnd;
	double gEnd;
	double bEnd;
	double rStep;
	double gStep;
	double bStep;
	double r;
	double g;
	double b;
	int i;

	rStart = GetRValue(topColor);
	gStart = GetGValue(topColor);
	bStart = GetBValue(topColor);

	rEnd = GetRValue(bottomColor);
	gEnd = GetGValue(bottomColor);
	bEnd = GetBValue(bottomColor);
	
	rStep = (rEnd - rStart) / ((float)rc.Height() * perc);
	gStep = (gEnd - gStart) / ((float)rc.Height() * perc);
	bStep = (bEnd - bStart) / ((float)rc.Height() * perc);

	r = rStart;
	g = gStart;
	b = bStart;

	// draw top gradient strip
	//
	int top;

	top = rc.top + (int)((float)rc.Height() * perc);
	for (i = rc.top; i < top; i++) {
		CPen pen(PS_SOLID, 1, RGB(r, g, b));
		CPen* oldPen;
		
		oldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(rc.left, i);
		pDC->LineTo(rc.right, i);
		pDC->SelectObject(oldPen);

		r += rStep;
		g += gStep;
		b += bStep;

		pen.DeleteObject();
	}

	// draw bottom gradient strip
	//
	rStart = GetRValue(bottomColor);
	gStart = GetGValue(bottomColor);
	bStart = GetBValue(bottomColor);

	rEnd = GetRValue(topColor);
	gEnd = GetGValue(topColor);
	bEnd = GetBValue(topColor);
	
	rStep = (rEnd - rStart) / ((float)rc.Height() * (1 - perc));
	gStep = (gEnd - gStart) / ((float)rc.Height() * (1 - perc));
	bStep = (bEnd - bStart) / ((float)rc.Height() * (1 - perc));

	r = rStart;
	g = gStart;
	b = bStart;

	for (i = top; i < rc.bottom; i++) {
		CPen pen(PS_SOLID, 1, RGB(r, g, b));
		CPen* oldPen;
		
		oldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(rc.left, i);
		pDC->LineTo(rc.right, i);
		pDC->SelectObject(oldPen);

		r += rStep;
		g += gStep;
		b += bStep;

		pen.DeleteObject();
	}

}

void DrawUtils::DrawAppleGradient(	CDC* pDC, 
									const CRect& rc, 
									COLORREF topColor, 
									COLORREF middleColor1, 
									COLORREF middleColor2, 
									COLORREF bottomColor)
{
	CRect tmpRect;

	tmpRect = rc;
	tmpRect.bottom = tmpRect.top + tmpRect.Height() / 2;
	DrawUtils::DrawGradient(pDC, tmpRect, topColor, middleColor1, 1);
	
	tmpRect = rc;
	tmpRect.top += tmpRect.Height() / 2;
	DrawUtils::DrawGradient(pDC, tmpRect, middleColor2, bottomColor, 1);
}

void DrawUtils::DrawFilledRect(CDC* pDC, const CRect& rc, COLORREF bkColor)
{
	CBrush b(bkColor);

	pDC->FillRect(rc, &b);
}

CRect DrawUtils::DrawBitmapTransparent(CDC* pDC, UINT bmpResourceId, const CRect& rc, COLORREF transparentColor, UINT flags)
{
	CVMBitmap bmp;//, *poldbmp;
	CDC memdc;

	bmp.LoadBitmap(bmpResourceId);		
	
	// Copy (BitBlt) bitmap from memory DC to screen DC
	//
	int width;
	int height;
	int left;
	int top;

	left = rc.left;
	top = rc.top;

	if (flags & enBmp_Tile) {
		width = rc.Width();
		height = rc.Height();

		bmp.DrawTiles(pDC->GetSafeHdc(), left, top, width, height);		
	} else {
		if (rc.Width() == 0) {
			width = bmp.GetWidth();
			left = rc.left;
		} else {
			width = min(rc.Width(), bmp.GetWidth());

			if (flags & enBmp_AlignLeft)
				left = rc.left;
			else if (flags & enBmp_AlignCenter)
				left = rc.left + (rc.Width() - bmp.GetWidth()) / 2;
			else if (flags & enBmp_AlignRight)
				left = rc.right - bmp.GetWidth();
		}

		if (rc.Height() == 0) {
			height = bmp.GetHeight();
			top = rc.top;
		} else {
			height = min(rc.Height(), bmp.GetHeight());
			if (flags & enBmp_AlignTop)
				top = rc.top;
			else if (flags & enBmp_AlignVertCenter)
				top = rc.top + (rc.Height() - bmp.GetHeight()) / 2;
			else if (flags & enBmp_AlignBottom)
				top = rc.bottom - bmp.GetHeight();
		}
		bmp.DrawTransparent(pDC->GetSafeHdc(), left, top, transparentColor);
	}
	
	return CRect(left, top, left + width, top + height);	
}


CRect DrawUtils::DrawBitmapTransparent(CDC* pDC, UINT bmpResourceId, const CPoint& topLeft, COLORREF transparentColor, UINT flags)
{
	CRect rc;

	rc.left = topLeft.x;
	rc.top = topLeft.y;
	rc.right = topLeft.x;
	rc.bottom = topLeft.y;

	return DrawBitmapTransparent(pDC, bmpResourceId, rc, transparentColor, flags);
}

CRect DrawUtils::DrawBitmap(CDC* pDC, UINT bmpResourceId, const CRect& rc, UINT flags)
{
	CVMBitmap bmp;//, *poldbmp;
	CDC memdc;

	bmp.LoadBitmap(bmpResourceId);		
	
	// Copy (BitBlt) bitmap from memory DC to screen DC
	//
	int width;
	int height;
	int left;
	int top;

	left = rc.left;
	top = rc.top;

	if (flags & enBmp_Tile) {
		width = rc.Width();
		height = rc.Height();

		bmp.DrawTiles(pDC->GetSafeHdc(), left, top, width, height);		
	} else {
		if (rc.Width() == 0) {
			width = bmp.GetWidth();
			left = rc.left;
		} else {
			width = min(rc.Width(), bmp.GetWidth());

			if (flags & enBmp_AlignLeft)
				left = rc.left;
			else if (flags & enBmp_AlignCenter)
				left = rc.left + (rc.Width() - bmp.GetWidth()) / 2;
			else if (flags & enBmp_AlignRight)
				left = rc.right - bmp.GetWidth();
		}

		if (rc.Height() == 0) {
			height = bmp.GetHeight();
			top = rc.top;
		} else {
			height = min(rc.Height(), bmp.GetHeight());
			if (flags & enBmp_AlignTop)
				top = rc.top;
			else if (flags & enBmp_AlignVertCenter)
				top = rc.top + (rc.Height() - bmp.GetHeight()) / 2;
			else if (flags & enBmp_AlignBottom)
				top = rc.bottom - bmp.GetHeight();
		}
		bmp.Draw(pDC->GetSafeHdc(), left, top, width, height);
	}
	
	return CRect(left, top, left + width, top + height);
}


CRect DrawUtils::DrawBitmap(CDC* pDC, UINT bmpResourceId, const CPoint& topLeft, UINT flags)
{
	CRect rc;

	rc.left = topLeft.x;
	rc.top = topLeft.y;
	rc.right = topLeft.x;
	rc.bottom = topLeft.y;

	return DrawBitmap(pDC, bmpResourceId, rc, flags);
}



void DrawUtils::DrawString(	CDC* pDC, 
							const cmString& str, 
							CRect& rc, 
							UINT format,
							cmString fontName,
							int fontSize, 
							COLORREF textColor, 
							bool fontBold)
{
	CFont font;
		
	font.CreateFont(fontSize, 
					 0,										/* width */ 
					 0,										/* escapement */
					 0,										/* orientation */
					 fontBold ? FW_BOLD : FW_NORMAL,		/* weight*/
					 0,										/* italic*/
					 0,										/* underline */
					 0,										/* strikeout */
					 ANSI_CHARSET,							/* charset */
					 OUT_DEFAULT_PRECIS,					/* out precision */
					 CLIP_DEFAULT_PRECIS,					/* clip precision */
					 DEFAULT_QUALITY,						/* quality */
					 DEFAULT_PITCH,							/* pitch family */
					 fontName);	

	DrawString(pDC, str, rc, format, textColor, &font);
}

void DrawUtils::DrawString(CDC* pDC, const cmString& str, CRect& rc, UINT format, COLORREF textColor, CFont* font)
{
	CFont* oldFont;

	oldFont = pDC->SelectObject(font);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(textColor);
	pDC->DrawText(str, rc, format);

	pDC->SelectObject(oldFont);
}

CSize DrawUtils::GetBitmapSize(UINT bmpResourceId)
{
	CVMBitmap bmp;
	CSize size(0,0);
	BOOL ret;

	ret = bmp.LoadBitmap(bmpResourceId);
	if (ret)
		size = CSize(bmp.GetWidth(), bmp.GetHeight());
	
	return size;
}

void DrawUtils::DrawIcon(CDC* pDC, UINT icoResourceId, const CPoint& topLeft, int size)
{
	CImageList imageList;

	imageList.Create(size, size, ILC_COLOR24 | ILC_MASK, 4, 4);

	imageList.Add(AfxGetApp()->LoadIcon(icoResourceId));

	imageList.Draw(pDC, 0, topLeft, ILD_TRANSPARENT);

}

void DrawUtils::DrawRect(CDC* pDC, const CRect& rc, COLORREF borderColor)
{
	CPen pen(PS_SOLID, 1, borderColor);
	CPen* oldPen;

	oldPen = pDC->SelectObject(&pen);
	
	pDC->MoveTo(rc.TopLeft());
	pDC->LineTo(rc.right, rc.top);
	pDC->LineTo(rc.right, rc.bottom);
	pDC->LineTo(rc.left, rc.bottom);
	pDC->LineTo(rc.TopLeft());
	
	pDC->SelectObject(oldPen);

	pen.DeleteObject();
}