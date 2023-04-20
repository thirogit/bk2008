// cmGroupHeaderCell.cpp: implementation of the cmGroupHeaderCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmHeaderCell.h"
#include "cmCellBaseRenderer.h"
#include "cmRenderer_CaptionCell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmHeaderCell::cmHeaderCell():
	cmBaseCell(false)
{
	Initialize(true);
}

cmHeaderCell::cmHeaderCell(bool createStockRenderers):
	cmBaseCell(false)
{
	Initialize(createStockRenderers);
}

cmHeaderCell::~cmHeaderCell()
{

}

void cmHeaderCell::Initialize(bool createStockRenderers)
{
	if (createStockRenderers) {
		cmRenderer_CaptionCell* rend;
		
		// create "normal" rendering set
		//
		rend = new cmRenderer_CaptionCell;

		rend->SetBkColor(RGB(0xF1, 0xF3, 0xF8));//::GetSysColor(COLOR_WINDOW));
		rend->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		rend->GetFontInfo().SetFontSize(12);
		stockRenderers_[enCmCellStatus_Normal] = rend;

		// create "selected" rendering set
		//
		rend = new cmRenderer_CaptionCell;

		rend->SetBkColor(RGB(0xC4, 0xC4, 0xC4));
		rend->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		rend->GetFontInfo().SetFontSize(12);
		rend->GetFontInfo().SetFontBold(true);
		stockRenderers_[enCmCellStatus_Selected] = rend;

		// create "normal + mouseover" rendering set
		//
		rend = new cmRenderer_CaptionCell;

		rend->SetBkColor(RGB(0xF1, 0xF3, 0xF8));
		rend->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		rend->GetFontInfo().SetFontSize(12);
		stockRenderers_[enCmCellStatus_NormalMouseOver] = rend;

		// create "selected + mouseover" rendering set
		//
		rend = new cmRenderer_CaptionCell;

		rend->SetBkColor(RGB(0xC4, 0xC4, 0xC4));
		rend->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		rend->GetFontInfo().SetFontSize(12);
		rend->GetFontInfo().SetFontBold(true);
		stockRenderers_[enCmCellStatus_SelectedMouseOver] = rend;
	}
}