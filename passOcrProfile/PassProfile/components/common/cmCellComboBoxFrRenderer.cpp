// cmCellComboBoxFrRenderer.cpp: implementation of the cmCellComboBoxFrRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmCellComboBoxFrRenderer.h"
#include "cmBaseCell.h"
#include "cmDrawUtils.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmCellComboBoxFrRenderer::cmCellComboBoxFrRenderer()
{

}

cmCellComboBoxFrRenderer::~cmCellComboBoxFrRenderer()
{

}

void cmCellComboBoxFrRenderer::Draw(CDC* pDC, cmBaseCell* cell)
{
	CSize bmpButtonSize;
	CRect buttonRect(0,0,0,0);
	CRect clientRect;

	clientRect = cell->GetDrawArea();
	if (cell->IsMouseOver() || cell->IsSelected()) {
		// draw button
		//
		bmpButtonSize = cmDrawUtils::GetBitmapSize(IDB_COMBO_BOX_NORMAL);

		buttonRect = clientRect;
		buttonRect.left = buttonRect.right - bmpButtonSize.cx - 2;
		buttonRect.top = cell->GetDrawArea().top + (buttonRect.Height() - bmpButtonSize.cy) / 2;

		cmDrawUtils::DrawBitmap(pDC, IDB_COMBO_BOX_NORMAL, buttonRect.TopLeft(), cmDrawUtils::enBmp_AlignLeft);

		clientRect.DeflateRect(4, 0, buttonRect.Width() + 2, 0);
	}

	if (!cell->HasFocus()) {
		
		CRect txtRect;
		CFont* font;

		txtRect = cell->GetDrawArea();
		
		//txtRect.DeflateRect(10, 2, 5, 2);
		txtRect.DeflateRect(1, 1, buttonRect.Width() + 1, 1);
		font = fontInfo_.CreateFont();
		cmDrawUtils::DrawString(pDC, cell->GetText(), txtRect, DT_LEFT /*| DT_VCENTER | DT_SINGLELINE*/ | DT_END_ELLIPSIS, txtColor_, font);
		fontInfo_.ReleaseFont();
	}
}