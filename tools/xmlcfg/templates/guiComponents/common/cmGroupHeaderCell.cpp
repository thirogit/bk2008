// cmGroupHeaderCell.cpp: implementation of the cmGroupHeaderCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmGroupHeaderCell.h"
#include "cmCellBaseRenderer.h"
#include "cmCellBkRenderer.h"
#include "cmCellFrRenderer.h"
#include "cmCellDescriptor.h"
#include "cmCellBkGradient.h"
#include "cmRenderer_CaptionCell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmGroupHeaderCell::cmGroupHeaderCell():
	cmBaseCell(false)
{
	Initialize(true);
}

cmGroupHeaderCell::cmGroupHeaderCell(bool createStockRenderers):
	cmBaseCell(false)
{
	Initialize(createStockRenderers);
}

cmGroupHeaderCell::~cmGroupHeaderCell()
{

}

void cmGroupHeaderCell::Initialize(bool createStockRenderers)
{
	if (createStockRenderers) {
		// create "normal" rendering set
		//
		{
			cmRenderer_CaptionCell* rend = new cmRenderer_CaptionCell;

			rend->SetBkColor(::GetSysColor(COLOR_3DFACE));
			rend->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
			rend->GetFontInfo().SetFontSize(12);
			stockRenderers_[enCmCellStatus_Normal] = rend;
		}

		// create "selected" rendering set
		//
		{
			cmRenderer_CaptionCell* rend = new cmRenderer_CaptionCell;
		
			rend->SetBkColor(::GetSysColor(COLOR_3DFACE));
			rend->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			rend->GetFontInfo().SetFontSize(12);
			rend->GetFontInfo().SetFontBold(true);
			stockRenderers_[enCmCellStatus_Selected] = rend;
		}

		// create "normal + mouseover" rendering set
		//
		{
			cmRenderer_CaptionCell* rend = new cmRenderer_CaptionCell;
	
			rend->SetBkColor(::GetSysColor(COLOR_3DFACE));
			rend->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			rend->GetFontInfo().SetFontSize(12);
			stockRenderers_[enCmCellStatus_NormalMouseOver] = rend;
		}

		// create "selected + mouseover" rendering set
		//
		{
			cmRenderer_CaptionCell* rend = new cmRenderer_CaptionCell;

			rend->SetBkColor(::GetSysColor(COLOR_3DFACE));
			rend->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			rend->GetFontInfo().SetFontSize(12);
			rend->GetFontInfo().SetFontBold(true);
			stockRenderers_[enCmCellStatus_SelectedMouseOver] = rend;
		}
	}
}
