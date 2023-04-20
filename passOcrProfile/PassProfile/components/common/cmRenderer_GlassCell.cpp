// cmRenderer_GlassCell.cpp
//

#include "stdafx.h"
#include "cmRenderer_GlassCell.h"	
#include "cmBaseCell.h"
#include "cmDrawUtils.h"

cmRenderer_GlassCell::cmRenderer_GlassCell(): 
	textFlags_(DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS), 
	textShadowEnabled_(true),
	borderEnabled_(true)
{}

cmRenderer_GlassCell::cmRenderer_GlassCell(const cmRenderer_GlassCell& rend)
{
	Copy(rend);
}

cmRenderer_GlassCell::~cmRenderer_GlassCell()
{

}

void cmRenderer_GlassCell::SetBorderEnable(bool enable)
{
	borderEnabled_ = enable;
}

bool cmRenderer_GlassCell::GetBorderEnable() const
{
	return borderEnabled_;
}

void cmRenderer_GlassCell::operator= (const cmRenderer_GlassCell& rend)
{
	Copy(rend);
}

void cmRenderer_GlassCell::Copy(const cmRenderer_GlassCell& rend)
{
	cmRenderer_CaptionCell::Copy(rend);	
}

/*
	sample :
	base color = #0d5876 [RGB(13, 88, 118)]

	top strip 	-------	0 - 50%
		B = 192 (= 118 +  74) -> 157 (= 118 + 39)
		G = 165 (=  88 +  79) -> 131 (=  88 + 43)
		R = 130 (=  13 + 117) -> 48	 (=  13 + 35)
		
	middle strip	------- 51% - 70%
		B = 118				  -> 134 (= 118 + 16)
		G = 88				  -> 106 (=  88 + 18)
		R = 13				  -> 40	 (=  13 + 27)

	bottom strip	------- 70% - 100%
		B = 134 (= 118 + 16)  -> 171 (= 118 + 53)
		G = 106 (=  88 + 18)  -> 155 (=	 88 + 67)
		R = 40	(=  13 + 27)  -> 81	 (=	 13 + 67)
*/

void cmRenderer_GlassCell::DrawBackground(CDC* pDC, cmBaseCell* cell)
{
	CRect rc;
	CRect topRect;
	CRect middleRect;
	CRect bottomRect;

	COLORREF topColor;
	COLORREF bottomColor;

	rc = cell->GetDrawArea();
	topRect = rc;
	topRect.bottom = rc.top + rc.Height() * 0.5F;
	
	middleRect = rc;
	middleRect.top = topRect.bottom;
	middleRect.bottom = rc.top + rc.Height() * 0.75F;

	bottomRect = rc;
	bottomRect.top = middleRect.bottom;
	bottomRect.bottom = rc.bottom;

	// draw top rect
	//
	topColor = cmDrawUtils::MakeColor2(bkColor_, 74, 79, 117);
	bottomColor = cmDrawUtils::MakeColor2(bkColor_, 39, 43, 35);
	cmDrawUtils::DrawVerticalGradient(pDC, topRect, topColor, bottomColor, 1);

	// draw mid rect
	//
	topColor = bkColor_;
	bottomColor = cmDrawUtils::MakeColor2(bkColor_, 16, 18, 27); 
	cmDrawUtils::DrawVerticalGradient(pDC, middleRect, topColor, bottomColor, 1);

	// draw bottom rect
	//
	topColor = cmDrawUtils::MakeColor2(bkColor_, 16, 18,27);
	bottomColor = cmDrawUtils::MakeColor2(bkColor_, 53, 67, 67); 
	cmDrawUtils::DrawVerticalGradient(pDC, bottomRect, topColor, bottomColor, 1);

	if (borderEnabled_) {
		// draw bright border rect
		//
		cmDrawUtils::DrawRect(pDC, rc, cmDrawUtils::MakeColorBrighter(bkColor_, 0x5C));
		
		// draw bottom border line
		//
		cmDrawUtils::DrawLine(pDC, CPoint(bottomRect.left, bottomRect.bottom), bottomRect.BottomRight(), cmDrawUtils::MakeColorDarker(bkColor_, 0xE));
	}
}

void cmRenderer_GlassCell::DrawForeground(CDC* pDC, cmBaseCell* cell)
{
	if (cell->GetText().IsEmpty())
		return;

	CRect txtRect;
	CRect shadowRect;
	
	txtRect = cell->GetDrawArea();
	
	txtRect.DeflateRect(1 + leftMargin_, 1, 1, 1);
    
	if (textShadowEnabled_) {
		// draw shadow
		//
		shadowRect = txtRect;
		shadowRect.OffsetRect(1, 1);
		
		cmDrawUtils::DrawString(pDC, 
								cell->GetText(), 
								shadowRect, 
								textFlags_, 
								RGB(0, 0, 0),
								fontInfo_);
	}

	cmDrawUtils::DrawString(pDC, cell->GetText(), txtRect, textFlags_, txtColor_, fontInfo_);
}

void cmRenderer_GlassCell::SetTextFlags(DWORD flags)
{
	textFlags_ = flags;
}

DWORD cmRenderer_GlassCell::GetTextFlags() const
{
	return textFlags_;
}

void cmRenderer_GlassCell::EnableTextShadow(bool e)
{
	textShadowEnabled_ = e;
}