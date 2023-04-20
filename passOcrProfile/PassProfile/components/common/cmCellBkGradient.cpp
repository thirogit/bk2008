// cmCellBkGradient.cpp: implementation of the cmCellBkGradient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmCellBkGradient.h"
#include "cmDrawUtils.h"
#include "cmBaseCell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmCellBkGradient::cmCellBkGradient():
	gradientTopColor_(RGB(0x03, 0x38, 0x94)),
	gradientBottomColor_(RGB(0x57, 0x86, 0xD4)),
	gradientPercentage_(1.0F)
{

}

cmCellBkGradient::cmCellBkGradient(const cmCellBkGradient& rend)
{
	Copy(rend);
}

cmCellBkGradient::~cmCellBkGradient()
{

}

void cmCellBkGradient::SetGradientTopColor(COLORREF color)
{
	gradientTopColor_ = color;
}

COLORREF cmCellBkGradient::GetGradientTopColor() const
{
	return gradientTopColor_;
}

void cmCellBkGradient::SetGradientBottomColor(COLORREF color)
{
	gradientBottomColor_ = color;
}

COLORREF cmCellBkGradient::GetGradientBottomColor() const
{
	return gradientBottomColor_;
}

void cmCellBkGradient::SetGradientPercentage(int perc)
{
	gradientPercentage_ = (float)perc / 100;
}

int cmCellBkGradient::GetGradientPercentage() const
{
	return (int)(gradientPercentage_ * 100);
}

void cmCellBkGradient::Draw(CDC* pDC, cmBaseCell* cell)
{
	cmDrawUtils::DrawVerticalGradient(pDC, cell->GetDrawArea(), gradientTopColor_, gradientBottomColor_, gradientPercentage_);
}

cmCellBaseRenderer& cmCellBkGradient::operator=(const cmCellBaseRenderer& rend)
{
	return Copy(rend);
}

cmCellBaseRenderer& cmCellBkGradient::Copy(const cmCellBaseRenderer& rend)
{
	cmCellBaseRenderer::Copy(rend);

	/*gradientTopColor_ = rend.gradientTopColor_;
	gradientBottomColor_ = rend.gradientBottomColor_;
	gradientPercentage_ = rend.gradientPercentage_;
*/
	return *this;
}