// cmTableRow_Style1.cpp: implementation of the cmTableRow_Style1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmTableRow_Style1.h"
#include "cmCellFactory.h"
#include "cmBaseCell.h"
#include "cmRenderer_CaptionCell.h"
#include "cmRenderer_GradientCell.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COLOR_BOTTOM						RGB(0x29, 0x29, 0x29)

#define COLOR_HEADER_TEXT					RGB(0xFF, 0xFF, 0xFF)
#define COLOR_HEADER_NORMAL					RGB(0x63, 0x8a, 0xc5)
#define COLOR_HEADER_SEL_GRADIENT_BOTTOM	RGB(0x77, 0x9e, 0xd9)
#define COLOR_HEADER_SEL_GRADIENT_TOP		RGB(0x92, 0xbc, 0xfa)

#define COLOR_CELL_NORMAL					RGB(0xCD, 0xDC, 0xF4)
#define COLOR_CELL_SEL_GRADIENT_BOTTOM		RGB(0xFF, 0x9B, 0x3B)
#define COLOR_CELL_SEL_GRADIENT_TOP			RGB(0xFF, 0xC9, 0x95)
#define BORDER 1

#define DEFAULT_COL_WIDTH 80

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmTableRow_Style1::cmTableRow_Style1()
{

}

cmTableRow_Style1::~cmTableRow_Style1()
{

}

void cmTableRow_Style1::SetNumColumns(unsigned int numColumns)
{
	cmBaseCell* newCell = NULL;

	Clear();

	numColumns_ = numColumns;

	for (unsigned int i = 0; i < numColumns; i++) {
		newCell = cmCellFactory::CreateCell(enCmCellType_Caption);
		
		if (newCell) {
			newCell->SetParentWindow(parentWnd_);
			newCell->SetInfo(cmCellFactory::CreateCellInfo());
			InitializeCellLookAndFeel(newCell);			
			colWidthList_.push_back(DEFAULT_COL_WIDTH);
		}		
		cells_.push_back(newCell);
	}

	SetDrawArea(drawArea_, true);
}

void cmTableRow_Style1::InitializeCellLookAndFeel(cmBaseCell* cell)
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

void cmTableRow_Style1::InitializeHeaderLookAndFeel(cmBaseCell* cell)
{
	cell->SetRenderingMode(enCmRenderingMode_Custom);
	
	// initialize Normal and NormalMouseOver renderers
	//
	cmRenderer_CaptionCell* rend;

	rend = rendFactory_.GetCaptionRenderer();

	rend->SetBkColor(COLOR_HEADER_NORMAL);
	rend->SetTextColor(COLOR_HEADER_TEXT);
	rend->GetFontInfo().SetFontSize(12);

	
	cell->SetRenderer(enCmCellStatus_Normal, rend);
	cell->SetRenderer(enCmCellStatus_NormalMouseOver, rendFactory_.GetCaptionRenderer(*rend));

	// initialize Selected and SelectedMouseOver renderers
	//
	cmRenderer_GradientCell* gradRend;

	gradRend = rendFactory_.GetGradientRenderer();
	gradRend->SetGradientTopColor(COLOR_HEADER_SEL_GRADIENT_TOP);
	gradRend->SetGradientBottomColor(COLOR_HEADER_SEL_GRADIENT_BOTTOM);
	gradRend->SetTextColor(COLOR_HEADER_TEXT);
	gradRend->GetFontInfo().SetFontSize(12);
		
	cell->SetRenderer(enCmCellStatus_Selected, gradRend);
	cell->SetRenderer(enCmCellStatus_SelectedMouseOver, rendFactory_.GetGradientRenderer(*gradRend));
}

cmBaseCell* cmTableRow_Style1::SetCellType(unsigned int index, enCmCellType cellType)
{
	cmBaseCell* cell;

	cell = cmTableRow::SetCellType(index, cellType);
	if (cell) {
		if (cellType == enCmCellType_Header)
			InitializeHeaderLookAndFeel(cell);
		else
			InitializeCellLookAndFeel(cell);
	}

	/*
	assert(index < numColumns_);
	if (index >= numColumns_)
		return NULL;
	
	cmBaseCell* cell;
	CRect drawArea;

	cell = GetCell(index);
	if (cell->GetType() == cellType)
		return cell;

	drawArea = cell->GetDrawArea();

	// delete previous cell
	//
	delete cell;

	// create a new cell
	//
	cell = cmCellFactory::CreateCell(cellType);

	if (cell) {
		cell->SetParentWindow(parentWnd_);
		//cell->SetFontInfo(fontInfo_);
		if (cellType == enCmCellType_Header)
			InitializeHeaderLookAndFeel(cell);
		else
			InitializeCellLookAndFeel(cell);
		cell->SetDrawArea(drawArea);
	}
	
	cells_[index] = cell;
	*/

	return cell;
}

void cmTableRow_Style1::SetFixedColumnsNumber(unsigned int number)
{
	
}