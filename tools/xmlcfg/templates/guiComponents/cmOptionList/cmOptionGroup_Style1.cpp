// cmOptionGroup_Style1.cpp
//

#include "stdafx.h"
#include "cmOptionGroup_Style1.h"
#include "cmCellEnum.h"
#include "cmRenderer_GlassCell.h"
#include "cmRenderer_GradientCell.h"
#include "cmCellDefs.h"
#include "cmDrawUtils.h"

#define COLOR_BOTTOM					RGB(0x29, 0x29, 0x29)
#define COLOR_HEADER_TEXT				RGB(0xFF, 0xFF, 0xFF)
#define COLOR_CELL_NORMAL				RGB(0xCD, 0xDC, 0xF4)
#define COLOR_CELL_SEL_GRADIENT_BOTTOM  RGB(0xFF, 0x9B, 0x3B)
#define COLOR_CELL_SEL_GRADIENT_TOP		RGB(0xFF, 0xC9, 0x95)
#define BORDER 1

cmOptionGroup_Style1::cmOptionGroup_Style1(): 
	headerBkColor_(COLOR_BOTTOM),
	headerTextColor_(COLOR_HEADER_TEXT),
	headerTextShadow_(true)
{
	InitializeHeaderLookAndFeel();
	
	titleRowHeight_ = 18;
}

cmOptionGroup_Style1::~cmOptionGroup_Style1()
{
	
}

void cmOptionGroup_Style1::Draw(CDC* pDC, const CPoint& mousePos)
{
	//cmOptionGroup::Draw(pDC, mousePos);

	header_.Draw(pDC, mousePos);
	DrawExpandButton(pDC);

	DrawRows(pDC, mousePos);
	DrawGrid(pDC);
}

void cmOptionGroup_Style1::UpdateHeaderDrawArea(const CRect& drawArea)
{
	// set header draw area
	//
	CRect headerDrawArea;

	headerDrawArea.left = drawArea.left;
	headerDrawArea.right = drawArea.right;
	headerDrawArea.top = drawArea.top;
	headerDrawArea.bottom = drawArea.top + titleRowHeight_ + BORDER;

	header_.SetDrawArea(headerDrawArea);

}

void cmOptionGroup_Style1::DrawExpandButton(CDC* pDC)
{
	// draw white background
	//
	pDC->FillSolidRect(expandRect_, RGB(255,255,255));
	
	// draw button
	//
	cmOptionGroup::DrawExpandButton(pDC);
}

void cmOptionGroup_Style1::DrawGrid(CDC* pDC)
{
	CPen pen(PS_SOLID, 1, borderColor_);
	CPen* oldPen;
	CRect tmp;

	oldPen = pDC->SelectObject(&pen);

	// draw vertical strip border
	//
	pDC->MoveTo(verticalStripWidth_, header_.GetDrawArea().bottom + BORDER);
	pDC->LineTo(verticalStripWidth_, drawArea_.bottom);
	
	if (expanded_) {
		// draw rows borders
		//
		for (OptionRowMap::iterator it = rows_.begin(); it != rows_.end(); it++) {
			
			tmp = it->second->GetDrawArea();

			tmp.InflateRect(1, 1, 0, 0);
				
			cmDrawUtils::DrawRect(pDC, tmp, borderColor_);
		}
	}

	pDC->SelectObject(oldPen);
}

void cmOptionGroup_Style1::InitializeHeaderLookAndFeel()
{
	header_.SetRenderingMode(enCmRenderingMode_Custom);

	// setup gradient background for "Normal" and "Normal + MouseOver" state header
	//
	cmRenderer_GlassCell* rendNormal;

	rendNormal = rendFactory_.GetGlassRenderer();
	InitGlassRenderer(rendNormal);
	rendNormal->GetFontInfo().SetFontBold(false);
	
	header_.SetRenderer(enCmCellStatus_Normal, rendNormal);
	header_.SetRenderer(enCmCellStatus_NormalMouseOver, rendFactory_.GetGlassRenderer(*rendNormal));
	
	// setup gradient background for "Selected" and "Selected + MouseOver" state header
	//
	cmRenderer_GlassCell* rendSelected = rendFactory_.GetGlassRenderer();
	InitGlassRenderer(rendSelected);
	rendSelected->GetFontInfo().SetFontBold(true);
	
	header_.SetRenderer(enCmCellStatus_Selected, rendSelected);
	header_.SetRenderer(enCmCellStatus_SelectedMouseOver, rendFactory_.GetGlassRenderer(*rendSelected));
}

void cmOptionGroup_Style1::InitializeCellLookAndFeel(cmBaseCell* cell)
{
	cell->SetRenderingMode(enCmRenderingMode_Custom);
	
	// initialize Normal and NormalMouseOver renderers
	//
	cmRenderer_CaptionCell* rend;

	rend = rendFactory_.GetCaptionRenderer();

	rend->SetBkColor(COLOR_CELL_NORMAL);
	rend->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	rend->GetFontInfo().SetFontSize(12);

	
	cell->SetRenderer(enCmCellStatus_Normal, rend);
	cell->SetRenderer(enCmCellStatus_NormalMouseOver, rendFactory_.GetCaptionRenderer(*rend));

	// initialize Selected and SelectedMouseOver renderers
	//
	cmRenderer_GradientCell* gradRend;

	gradRend = rendFactory_.GetGradientRenderer();
	gradRend->SetGradientTopColor(COLOR_CELL_SEL_GRADIENT_TOP);
	gradRend->SetGradientBottomColor(COLOR_CELL_SEL_GRADIENT_BOTTOM);
	gradRend->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	gradRend->GetFontInfo().SetFontSize(12);
		
	cell->SetRenderer(enCmCellStatus_Selected, gradRend);
	cell->SetRenderer(enCmCellStatus_SelectedMouseOver, rendFactory_.GetGradientRenderer(*gradRend));
	
	
	
}

cmOptionRow* cmOptionGroup_Style1::AddRow(const cmString& name, const cmString& caption, enCmCellType valueCellType)
{
	cmOptionRow* row;

	row = cmOptionGroup::AddRow(name, caption, valueCellType);
	if (row) {
		row->SetSplitterColor(borderColor_);
		InitializeCellLookAndFeel(row->GetCaptionCell());
		InitializeCellLookAndFeel(row->GetValueCell());
	}

	return row;
}

void cmOptionGroup_Style1::InitGlassRenderer(cmRenderer_GlassCell* rend)
{
	rend->SetBkColor(headerBkColor_);
	rend->GetFontInfo().SetFontName(_T("Tahoma"));
	rend->GetFontInfo().SetFontSize(-12);
	rend->SetTextColor(headerTextColor_);
	rend->EnableTextShadow(headerTextShadow_);
	rend->SetLeftMargin(verticalStripWidth_);
}

void cmOptionGroup_Style1::SetHeaderBkColor(COLORREF color, bool redraw)
{
	cmRenderer_GlassCell* rend;

	headerBkColor_ = color;

	for (int i = 0; i < 4; i++) {
		rend = dynamic_cast<cmRenderer_GlassCell*>(header_.GetRenderer((enCmCellStatus)i));

		rend->SetBkColor(color);
	}

	if (redraw)
		parentWnd_->RedrawWindow();
}

COLORREF cmOptionGroup_Style1::GetHeaderBkColor() const
{
	return headerBkColor_;
}

void cmOptionGroup_Style1::SetHeaderTextColor(COLORREF color, bool redraw)
{
	cmRenderer_GlassCell* rend;

	headerTextColor_ = color;

	for (int i = 0; i < 4; i++) {
		rend = dynamic_cast<cmRenderer_GlassCell*>(header_.GetRenderer((enCmCellStatus)i));

		rend->SetTextColor(color);
	}

	if (redraw)
		parentWnd_->RedrawWindow();
}

COLORREF cmOptionGroup_Style1::GetHeaderTextColor() const
{
	return headerTextColor_;
}

void cmOptionGroup_Style1::SetHeaderShadow(bool shadowEnabled, bool redraw)
{
	cmRenderer_GlassCell* rend;

	headerTextShadow_ = shadowEnabled;

	for (int i = 0; i < 4; i++) {
		rend = dynamic_cast<cmRenderer_GlassCell*>(header_.GetRenderer((enCmCellStatus)i));

		rend->EnableTextShadow(shadowEnabled);
	}

	if (redraw)
		parentWnd_->RedrawWindow();

}

bool cmOptionGroup_Style1::GetHeaderShadow() const
{
	return headerTextShadow_;
}

