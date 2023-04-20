// cmCellChkBoxBkRenderer.cpp: implementation of the cmCellChkBoxBkRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmCellChkBoxBkRenderer.h"
#include "cmCheckBoxCell.h"
#include "cmDrawUtils.h"
#include "cmCellDefs.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmCellChkBoxBkRenderer::cmCellChkBoxBkRenderer():
	uncheckedIconId_(0),
	checkedIconId_(0)
{

}

cmCellChkBoxBkRenderer::~cmCellChkBoxBkRenderer()
{

}

void cmCellChkBoxBkRenderer::Draw(CDC* pDC, cmBaseCell* cell)
{
	cmCheckBoxCell* c;
	
	c = dynamic_cast<cmCheckBoxCell*>(cell);
	assert(c);
	if (c)
		cmDrawUtils::DrawIcon(pDC, c->GetChecked() ? checkedIconId_: uncheckedIconId_, c->GetIconRect().TopLeft(), CHK_ICON_SIZE);
}

void cmCellChkBoxBkRenderer::SetIcons(UINT uncheckedIconId, UINT checkedIconId)
{
	uncheckedIconId_ = uncheckedIconId;
	checkedIconId_ = checkedIconId;
}