// cmCellChkBoxFrRenderer.cpp: implementation of the cmCellChkBoxFrRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmCellChkBoxFrRenderer.h"
#include "cmBaseCell.h"
#include "cmCheckBoxCell.h"
#include "cmCellDefs.h"
#include "cmDrawUtils.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmCellChkBoxFrRenderer::cmCellChkBoxFrRenderer()
{
	
}

cmCellChkBoxFrRenderer::~cmCellChkBoxFrRenderer()
{

}

void cmCellChkBoxFrRenderer::Draw(CDC* pDC, cmBaseCell* cell)
{
	CRect txtRect;
	CFont* font;
	cmCheckBoxCell* c;

	c = dynamic_cast<cmCheckBoxCell*>(cell);
	assert(c);
	if (c) {
		txtRect = c->GetDrawArea();
		txtRect.DeflateRect(c->GetIconRect().Width() + 2 * CHK_LEFT_BORDER, 0, CHK_LEFT_BORDER, 0);
	
		font = fontInfo_.CreateFont();
		if (c->GetChecked())
			cmDrawUtils::DrawString(pDC, c->GetCheckedText(), txtRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS, txtColor_, font); 
		else
			cmDrawUtils::DrawString(pDC, c->GetUncheckedText(), txtRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS, txtColor_, font); 

		fontInfo_.ReleaseFont();
	}
}
