// cmCellFrRenderer.cpp: implementation of the cmCellFrRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmCellFrRenderer.h"
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

cmCellFrRenderer::cmCellFrRenderer():
	txtColor_(RGB(0, 0, 0))
{
	fontInfo_.SetFontName(_T("tahoma"));
	fontInfo_.SetFontBold(false);
	fontInfo_.SetFontSize(12);
}

cmCellFrRenderer::~cmCellFrRenderer()
{

}

void cmCellFrRenderer::Draw(CDC* pDC, cmBaseCell* cell)
{
	if (cell->GetText().IsEmpty())
		return;

	CRect txtRect;
	CFont* font;

	txtRect = cell->GetDrawArea();
	//txtRect.DeflateRect(10, 2, 5, 2);
	txtRect.DeflateRect(1, 1, 1, 1);
    font = fontInfo_.CreateFont();
	cmDrawUtils::DrawString(pDC, cell->GetText(), txtRect, DT_LEFT /*| DT_VCENTER | DT_SINGLELINE*/ | DT_END_ELLIPSIS, txtColor_, font);
	fontInfo_.ReleaseFont();
}

void cmCellFrRenderer::SetTextColor(COLORREF color)
{
	txtColor_ = color;
}

COLORREF cmCellFrRenderer::GetTextColor() const
{
	return txtColor_;
}

cmFontInfo& cmCellFrRenderer::GetFontInfo()
{
	return fontInfo_;
}