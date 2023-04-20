// cmCellBkRenderer.cpp: implementation of the cmCellBkRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmCellBkRenderer.h"
#include "cmBaseCell.h"
#include "cmDrawUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


cmCellBkRenderer::cmCellBkRenderer():
	bkColor_(RGB(255, 255, 255))
{

}

cmCellBkRenderer::~cmCellBkRenderer()
{

}

void cmCellBkRenderer::Draw(CDC* pDC, cmBaseCell* cell)
{
	pDC->FillSolidRect(cell->GetDrawArea(), bkColor_);
}

void cmCellBkRenderer::SetBkColor(COLORREF color)
{
	bkColor_ = color;
}

COLORREF cmCellBkRenderer::GetBkColor() const
{
	return bkColor_;
}

cmCellBaseRenderer& cmCellBkRenderer::operator=(const cmCellBaseRenderer& rend)
{
	return Copy(rend);
}

cmCellBaseRenderer& cmCellBkRenderer::Copy(const cmCellBaseRenderer& rend)
{
	cmCellBaseRenderer::Copy(rend);

	//bkColor_ = rend.bkColor_;

	return *this;
}
