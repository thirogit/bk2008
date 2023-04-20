// cmRenderer_CaptionCell.cpp
//
#include "stdafx.h"
#include "cmRenderer_GradientCell.h"
#include "cmBaseCell.h"
#include "cmDrawUtils.h"

cmRenderer_GradientCell::cmRenderer_GradientCell()
{}

cmRenderer_GradientCell::cmRenderer_GradientCell(const cmRenderer_GradientCell& rend)
{
	Copy(rend);
}

cmRenderer_GradientCell::~cmRenderer_GradientCell()
{}

void cmRenderer_GradientCell::SetGradientTopColor(COLORREF color)
{
	gradientTopColor_ = color;
}

COLORREF cmRenderer_GradientCell::GetGradientTopColor() const
{
	return gradientTopColor_;
}

void cmRenderer_GradientCell::SetGradientBottomColor(COLORREF color)
{
	gradientBottomColor_ = color;
}

COLORREF cmRenderer_GradientCell::GetGradientBottomColor() const
{
	return gradientBottomColor_;
}
	
void cmRenderer_GradientCell::operator= (const cmRenderer_GradientCell& rend)
{
	Copy(rend);
}

void cmRenderer_GradientCell::Copy(const cmRenderer_GradientCell& rend)
{
	cmRenderer_CaptionCell::Copy(rend);

	gradientTopColor_ = rend.gradientTopColor_;
	gradientBottomColor_ = rend.gradientBottomColor_;
}

void cmRenderer_GradientCell::DrawBackground(CDC* pDC, cmBaseCell* cell)
{
	cmDrawUtils::DrawVerticalGradient(pDC, cell->GetDrawArea(), gradientTopColor_, gradientBottomColor_, 1);

	if (iconId_) {
		CPoint topLeft;

		topLeft = cell->GetDrawArea().TopLeft();
		topLeft.Offset(1, 1);
		cmDrawUtils::DrawIcon(pDC, iconId_, topLeft, 16);
	}
}
	
