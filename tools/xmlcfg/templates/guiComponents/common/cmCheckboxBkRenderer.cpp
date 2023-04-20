// cmCheckboxBkRenderer.cpp: implementation of the cmCheckboxBkRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmCheckBoxBkRenderer.h"
#include "cmDrawUtils.h"
#include "cmCellDefs.h"
#include "cmCheckBoxCell.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmCheckBoxBkRenderer::cmCheckBoxBkRenderer()
{

}

cmCheckBoxBkRenderer::~cmCheckBoxBkRenderer()
{

}

void cmCheckBoxBkRenderer::Draw(CDC* pDC, cmBaseCell* cell)
{
	cmCheckBoxCell* c;
	
	c = dynamic_cast<cmCheckBoxCell*>(cell);
	assert(c);
	if (c)
		cmDrawUtils::DrawIcon(pDC, c->GetChecked() ? checkedIconId_: uncheckedIconId_, c->GetIconRect().TopLeft(), CHK_ICON_SIZE);
}

void cmCheckBoxBkRenderer::SetIcons(UINT uncheckedIconId, UINT checkedIconId)
{
	uncheckedIconId_ = uncheckedIconId;
	checkedIconId_ = checkedIconId_;
}