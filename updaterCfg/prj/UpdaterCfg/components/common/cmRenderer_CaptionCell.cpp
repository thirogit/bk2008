// cmRenderer_CaptionCell.cpp
//


#include "stdafx.h"
#include "cmRenderer_CaptionCell.h"
#include "cmBaseCell.h"
#include "cmDrawUtils.h"

cmRenderer_CaptionCell::cmRenderer_CaptionCell():
	iconId_(0),
	leftMargin_(0)
{}

cmRenderer_CaptionCell::cmRenderer_CaptionCell(cmRenderer_CaptionCell& rend)
{
	Copy(rend);
}

cmRenderer_CaptionCell::~cmRenderer_CaptionCell()
{}

void cmRenderer_CaptionCell::Draw(CDC* pDC, cmBaseCell* cell)
{
	DrawBackground(pDC, cell);
	DrawForeground(pDC, cell);
}

void cmRenderer_CaptionCell::DrawBackground(CDC* pDC, cmBaseCell* cell)
{
	pDC->FillSolidRect(cell->GetDrawArea(), bkColor_);

	if (iconId_) {
		CPoint topLeft;

		topLeft = cell->GetDrawArea().TopLeft();
		topLeft.Offset(1, 1);
		cmDrawUtils::DrawIcon(pDC, iconId_, topLeft, 16);
	}
}

void cmRenderer_CaptionCell::DrawForeground(CDC* pDC, cmBaseCell* cell)
{
	if (cell->GetText().IsEmpty())
		return;

	CRect txtRect;
	CFont* font;

	txtRect = cell->GetDrawArea();
	
	txtRect.DeflateRect(1 + leftMargin_, 1, 1, 1);
    font = fontInfo_.CreateFont();
	cmDrawUtils::DrawString(pDC, cell->GetText(), txtRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS, txtColor_, font);
	fontInfo_.ReleaseFont();
}

void cmRenderer_CaptionCell::SetBkColor(COLORREF color)
{
	bkColor_ = color;
}

COLORREF cmRenderer_CaptionCell::GetBkColor() const
{
	return bkColor_;
}

void cmRenderer_CaptionCell::SetTextColor(COLORREF color)
{
	txtColor_ = color;
}

COLORREF cmRenderer_CaptionCell::GetTextColor() const
{
	return txtColor_;
}

void cmRenderer_CaptionCell::SetIconId(UINT icon)
{
	iconId_ = icon;

	leftMargin_ = icon ? 18 : 0;
}

UINT cmRenderer_CaptionCell::GetIconId() const
{
	return iconId_;
}

cmCellBaseRenderer& cmRenderer_CaptionCell::operator =(const cmRenderer_CaptionCell& rend)
{
	return Copy(rend);
}

cmCellBaseRenderer& cmRenderer_CaptionCell::Copy(const cmRenderer_CaptionCell& rend)
{
	cmCellBaseRenderer::Copy(rend);
		
	fontInfo_ = rend.fontInfo_;
	iconId_ = rend.iconId_;
	leftMargin_ = rend.leftMargin_; 

	bkColor_ = rend.bkColor_;
	txtColor_ = rend.txtColor_;

	return *this;
}

void cmRenderer_CaptionCell::SetLeftMargin(int margin)
{
	leftMargin_ = margin;
}

int cmRenderer_CaptionCell::GetLeftMargin() const
{
	return leftMargin_;
}
