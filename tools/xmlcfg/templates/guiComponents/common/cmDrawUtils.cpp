// DrawUtils.cpp
//

#include "stdafx.h"
#include "cmDrawUtils.h"
#include "VMBitmap.h"
#include "cmFontInfo.h"


multimap<int, int> cmDrawUtils::colors_;

void cmDrawUtils::DrawVerticalGradient(CDC* pDC, CRect& rc, COLORREF topColor, COLORREF bottomColor, float perc)
{

	    TRIVERTEX        vert[2] ;
    GRADIENT_RECT    gRect;
    vert [0] .x      = rc.left;
    vert [0] .y      = rc.top;
    vert [0] .Red    = GetRValue(topColor) << 8;
    vert [0] .Green  = GetGValue(topColor) << 8;
    vert [0] .Blue   = GetBValue(topColor) << 8;
    vert [0] .Alpha  = 0x0000;

    vert [1] .x      = rc.right;
    vert [1] .y      = rc.bottom; 
    vert [1] .Red    = GetRValue(bottomColor) << 8;
    vert [1] .Green  = GetGValue(bottomColor) << 8;
    vert [1] .Blue   = GetBValue(bottomColor) << 8;
    vert [1] .Alpha  = 0x0000;

    gRect.UpperLeft  = 0;
    gRect.LowerRight = 1;
    ::GradientFill(pDC->m_hDC, vert,2,&gRect,1,GRADIENT_FILL_RECT_V);
}

void cmDrawUtils::DrawHorizontalGradient(CDC* pDC, const CRect& rc, COLORREF leftColor, COLORREF rightColor, float perc)
{
    TRIVERTEX        vert[2] ;
    GRADIENT_RECT    gRect;
    vert [0] .x      = rc.left;
    vert [0] .y      = rc.top;
    vert [0] .Red    = GetRValue(leftColor) << 8;
    vert [0] .Green  = GetGValue(leftColor) << 8;
    vert [0] .Blue   = GetBValue(leftColor) << 8;
    vert [0] .Alpha  = 0x0000;

    vert [1] .x      = rc.right;
    vert [1] .y      = rc.bottom; 
    vert [1] .Red    = GetRValue(rightColor) << 8;
    vert [1] .Green  = GetGValue(rightColor) << 8;
    vert [1] .Blue   = GetBValue(rightColor) << 8;
    vert [1] .Alpha  = 0x0000;

    gRect.UpperLeft  = 0;
    gRect.LowerRight = 1;
    ::GradientFill(pDC->m_hDC, vert, 2,&gRect,1,GRADIENT_FILL_RECT_H);
}

void cmDrawUtils::DrawAppleGradient(	CDC* pDC, 
									const CRect& rc, 
									COLORREF topColor, 
									COLORREF middleColor1, 
									COLORREF middleColor2, 
									COLORREF bottomColor)
{
	CRect tmpRect;

	tmpRect = rc;
	tmpRect.bottom = tmpRect.top + tmpRect.Height() / 2;
	DrawVerticalGradient(pDC, tmpRect, topColor, middleColor1, 1);
	
	tmpRect = rc;
	tmpRect.top += tmpRect.Height() / 2;
	DrawVerticalGradient(pDC, tmpRect, middleColor2, bottomColor, 1);
}

void cmDrawUtils::DrawFilledRect(CDC* pDC, const CRect& rc, COLORREF bkColor)
{
	CBrush b(bkColor);

	pDC->FillRect(rc, &b);
}

CRect cmDrawUtils::DrawBitmapTransparent(CDC* pDC, UINT bmpResourceId, const CRect& rc, COLORREF transparentColor, UINT flags)
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


CRect cmDrawUtils::DrawBitmapTransparent(CDC* pDC, UINT bmpResourceId, const CPoint& topLeft, COLORREF transparentColor, UINT flags)
{
	CRect rc;

	rc.left = topLeft.x;
	rc.top = topLeft.y;
	rc.right = topLeft.x;
	rc.bottom = topLeft.y;

	return DrawBitmapTransparent(pDC, bmpResourceId, rc, transparentColor, flags);
}

CRect cmDrawUtils::DrawBitmap(CDC* pDC, UINT bmpResourceId, const CRect& rc, UINT flags)
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


CRect cmDrawUtils::DrawBitmap(CDC* pDC, UINT bmpResourceId, const CPoint& topLeft, UINT flags)
{
	CRect rc;

	rc.left = topLeft.x;
	rc.top = topLeft.y;
	rc.right = topLeft.x;
	rc.bottom = topLeft.y;

	return DrawBitmap(pDC, bmpResourceId, rc, flags);
}



void cmDrawUtils::DrawString(	CDC* pDC, 
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

void cmDrawUtils::DrawString(CDC* pDC, const cmString& str, CRect& rc, UINT format, COLORREF textColor, CFont* font)
{
	CFont* oldFont;

	oldFont = pDC->SelectObject(font);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(textColor);
	pDC->DrawText(str, rc, format);

	pDC->SelectObject(oldFont);
}

void cmDrawUtils::DrawString(CDC* pDC, const cmString& str, CRect& rc, UINT format, COLORREF textColor, const cmFontInfo& fontInfo)
{
	DrawString(pDC, str, rc, format, fontInfo.GetFontName(), fontInfo.GetFontSize(), textColor, fontInfo.GetFontBold());
}

CSize cmDrawUtils::GetBitmapSize(UINT bmpResourceId)
{
	CVMBitmap bmp;
	CSize size(0,0);
	BOOL ret;

	ret = bmp.LoadBitmap(bmpResourceId);
	if (ret)
		size = CSize(bmp.GetWidth(), bmp.GetHeight());
	
	return size;
}

void cmDrawUtils::DrawIcon(CDC* pDC, UINT icoResourceId, const CPoint& topLeft, int size)
{
	if (icoResourceId == 0)
		return;

	HICON icon;

	//icon = (HICON)::LoadImage(::GetModuleHandle(NULL), MAKEINTRESOURCE(icoResourceId), IMAGE_ICON, size, size, 0);
	//icon = AfxGetApp()->LoadIcon(icoResourceId);
	//icon = (HICON)::LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(icoResourceId), IMAGE_ICON, size, size, 0);
	icon = (HICON)::LoadImage(AfxFindResourceHandle(MAKEINTRESOURCE(icoResourceId),	RT_GROUP_ICON), MAKEINTRESOURCE(icoResourceId), IMAGE_ICON, size, size, 0);	
	
	::DrawIconEx(pDC->m_hDC, topLeft.x, topLeft.y, icon, size, size, 0, NULL, DI_NORMAL);
	
	::DestroyIcon(icon);

}

void cmDrawUtils::DrawRect(CDC* pDC, const CRect& rc, COLORREF borderColor)
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

COLORREF cmDrawUtils::MakeColorDarker(COLORREF color, BYTE offset)
{
	return MakeColor(color, -offset, -offset, -offset);
}

COLORREF cmDrawUtils::MakeColorBrighter(COLORREF color, BYTE offset)
{
	return MakeColor(color, offset, offset, offset);
}

COLORREF cmDrawUtils::MakeColor2(COLORREF baseColor, short maxColorOffset, short midColorOffset, short minColorOffset)
{
	int maxCol;
	int midCol;
	int minCol;
	COLORREF ret = 0;

	multimap<int, int>::iterator it;

	colors_.clear();

	// sort color components (and keep their offset in COLORREF)
	//
	colors_.insert(make_pair<int, int>(GetRValue(baseColor), 0));
	colors_.insert(make_pair<int, int>(GetGValue(baseColor), 8));
	colors_.insert(make_pair<int, int>(GetBValue(baseColor), 16));

	it = colors_.begin();

	minCol = max(min(it->first + minColorOffset, 0xFF), 0);
	ret |= minCol << it->second;
	it++;

	midCol = max(min(it->first + midColorOffset, 0xFF), 0);
	ret |= midCol << it->second;
	it++;

	maxCol = max(min(it->first + maxColorOffset, 0xFF), 0);
	ret |= maxCol << it->second;

	return ret;
}

COLORREF cmDrawUtils::MakeColor(COLORREF baseColor, short offsetR, short offsetG, short offsetB)
{
	int r;
	int g;
	int b;

	r = GetRValue(baseColor);
	g = GetGValue(baseColor);
	b = GetBValue(baseColor);

	r = min(0xff, r + offsetR);
	g = min(0xff, g + offsetG);
	b = min(0xff, b + offsetB);

	r = max(0, r);
	g = max(0, g);
	b = max(0, b);

	return RGB(r, g, b);
}

void cmDrawUtils::DrawLine(CDC* pDC, const CPoint& pt1, const CPoint& pt2, COLORREF color)
{
	CPen pen(PS_SOLID, 1, color);
	CPen* oldPen;

	oldPen = pDC->SelectObject(&pen);
	
	pDC->MoveTo(pt1);
	pDC->LineTo(pt2);
		
	pDC->SelectObject(oldPen);

	pen.DeleteObject();
}