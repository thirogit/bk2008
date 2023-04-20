// AsGradientCell.cpp
//
#include "stdafx.h"
#include "cmGradientCell.h"
#include "cmDrawUtils.h"

cmGradientCell::cmGradientCell():
	topColor_(RGB(0xFD, 0xFD, 0xFD)),
	midTopColor_(RGB(0xEF, 0xEF, 0xEF)),
	midBottomColor_(RGB(0xDF, 0xDF, 0xDF)),
	bottomColor_(RGB(0xFF, 0xFF, 0xFF))
{
	//fontInfo_.SetFontBold(true);
	//fontInfo_.SetFontSize(11);
}

cmGradientCell::~cmGradientCell()
{}

void cmGradientCell::Draw(CDC* pDC, const CPoint& mousePos)
{
	cmBaseCell::Draw(pDC, mousePos);
}

void cmGradientCell::SetTopColor(COLORREF topColor)
{
	topColor_ = topColor;
}

COLORREF cmGradientCell::GetTopColor() const
{
	return topColor_;
}

void cmGradientCell::SetMidTopColor(COLORREF midTopColor)
{
	midTopColor_ = midTopColor;
}

COLORREF cmGradientCell::GetMidTopColor() const
{
	return midTopColor_;
}

void cmGradientCell::SetMidBottomColor(COLORREF midBottomColor)
{
	midBottomColor_ = midBottomColor;
}

COLORREF cmGradientCell::GetMidBottomColor() const
{
	return midBottomColor_;
}

void cmGradientCell::SetBottomColor(COLORREF bottomColor)
{
	bottomColor_ = bottomColor;
}

COLORREF cmGradientCell::GetBottomColor() const
{
	return bottomColor_;
}
