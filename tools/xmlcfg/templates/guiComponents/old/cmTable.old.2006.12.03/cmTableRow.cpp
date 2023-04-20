// AsTableRow.cpp: implementation of the cmTableRow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawUtils.h"
#include "cmTableRow.h"
#include "cmBaseCell.h"
#include "cmGenericTextCell.h"
#include "cmCheckBoxCell.h"
#include "cmComboBoxCell.h"
#include "cmNumericTextCell.h"
#include "cmGradientCell.h"
#include "cmSmartNumericTextCell.h"

/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/

#define BORDER 1


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int cmTableRow::SplitterArea_ = 4;

cmTableRow::cmTableRow():
	numColumns_(0),
	parentWnd_(NULL),
	borderColor_(::GetSysColor(COLOR_3DSHADOW)),
	rowHeight_(21),
	fixedColumnsNumber_(0),
	firstNonFixedColIndex_(0)
{
	fontInfo_.SetFontBold(false);
	fontInfo_.SetFontName(_T("tahoma"));
	fontInfo_.SetFontSize(10);
}

cmTableRow::~cmTableRow()
{
	Clear();
}

void cmTableRow::SetNumColumns(unsigned int numColumns)
{
	numColumns_ = numColumns;
}

unsigned int cmTableRow::GetNumColumns() const
{
	return numColumns_;
}

void cmTableRow::Draw(CDC* pDC)
{
	DrawBackground(pDC);
	DrawCells(pDC);
	DrawBorders(pDC);
}

void cmTableRow::DrawBackground(CDC* pDC)
{
	/*CBrush b(RGB(255,100,100));
	
	pDC->FillRect(drawArea_, &b);*/
}

void cmTableRow::DrawBorders(CDC* pDC)
{
	CRect cellDrawArea;
	CPen pen(PS_SOLID, 1, borderColor_);
	CPen* oldPen;
	CRect rc;
	CRect tmp;
	int lastRight;
	unsigned int i;
	cmBaseCell* cell;
	//cmBaseCell* selectedCell = NULL;

	parentWnd_->GetClientRect(rc);
	oldPen = pDC->SelectObject(&pen);

	for (i = 0; i < fixedColumnsNumber_ && i < cells_.size(); i++) {
		cellDrawArea = cells_[i]->GetDrawArea();
		pDC->MoveTo(cellDrawArea.right, cellDrawArea.top);
		pDC->LineTo(cellDrawArea.right, cellDrawArea.bottom);
	}

	for (i = firstNonFixedColIndex_; i < cells_.size(); i++) {
		cell = cells_[i];
		cellDrawArea = cells_[i]->GetDrawArea();
		if (!rc.PtInRect(cellDrawArea.TopLeft()))
			break;

		pDC->MoveTo(cellDrawArea.right, cellDrawArea.top);
		pDC->LineTo(cellDrawArea.right, cellDrawArea.bottom);

		//if (cell->IsSelected())
		//	selectedCell = cell;
		lastRight = cellDrawArea.right;
	}

	pDC->SelectObject(oldPen);
}

void cmTableRow::DrawCells(CDC* pDC)
{
	unsigned int i;
	int lastRight;
	cmBaseCell* cell;
	CRect drawArea;
	CRect rc;

	parentWnd_->GetClientRect(rc);
	for (i = 0; i < fixedColumnsNumber_ && i < cells_.size(); i++) {
		cell = cells_[i];
		if (cell)
			cell->Draw(pDC);
	}
	lastRight = 0;
	for (i = firstNonFixedColIndex_; i < cells_.size(); i++) {
		cell = cells_[i];
		if (cell) {
			drawArea = cell->GetDrawArea();
			if (!rc.PtInRect(drawArea.TopLeft()) || drawArea.right < lastRight)
				break;
			//TRACE1("\tdrawing cell %d\n", i);
			cell->Draw(pDC);
		}
		lastRight = drawArea.right;
	}
}

void cmTableRow::SetDrawArea(const CRect& drawArea, bool forceRecalc)
{
	if (drawArea_ == drawArea && !forceRecalc)
		return;
	
	drawArea_ = drawArea;
	if (drawArea_.Height() != rowHeight_ && drawArea_.Height() != 0)
		drawArea_.bottom = drawArea_.top + rowHeight_;
	
	
	int totalWidth = 0;
	int cellWidth;
	unsigned int i;
	CRect tmp;
	CRect rc;
	CRect cellDrawArea;

	parentWnd_->GetClientRect(rc);

	tmp = drawArea_;
	cellWidth = drawArea_.Width();

	// set draw area on fixed columns
	//
	for (i = 0; i < fixedColumnsNumber_ && i < cells_.size(); i++) {
		tmp.right = tmp.left + colWidthList_[i];
		cells_[i]->SetDrawArea(tmp);
			
		if (!rc.PtInRect(tmp.TopLeft()))
			break;
		
		tmp.left = tmp.right + BORDER;

		totalWidth += colWidthList_[i];
		totalWidth += BORDER;	
	}

	for (i = fixedColumnsNumber_; i < firstNonFixedColIndex_ && i < cells_.size(); i++)
		cells_[i]->SetDrawArea(CRect(0,0,0,0));

	for (i = firstNonFixedColIndex_; i < numColumns_ && i < cells_.size(); i++) {
		tmp.right = tmp.left + colWidthList_[i];
		cells_[i]->SetDrawArea(tmp);
			
		if (!rc.PtInRect(tmp.TopLeft()))
			break;
		
		tmp.left = tmp.right + BORDER;

		totalWidth += colWidthList_[i];
		totalWidth += BORDER;
		
		if (!cells_[i]->IsVisible())
			cells_[i]->SetVisible(true);
	}

	drawArea_.right = drawArea_.left + totalWidth - 1;
}

CRect cmTableRow::GetDrawArea() const
{
	return drawArea_;
}

cmBaseCell* cmTableRow::CreateCell(unsigned int index, enCmTableCellType cellType)
{
	if (cells_.size() >= numColumns_)
		return NULL;
	
	cmBaseCell* newCell = NULL;

	switch (cellType) {
		case enCmTableCellType_Caption:
			newCell = new cmBaseCell;
			break;

		case enCmTableCellType_TextBox:
			newCell = new cmGenericTextCell;
			break;

		case enCmTableCellType_NumTextBox:
			newCell = new cmNumericTextCell;
			break;

		case enCmTableCellType_CheckBox:
			newCell = new cmCheckBoxCell;
			break;

		case enCmTableCellType_ComboBox:
			newCell = new cmComboBoxCell;
			break;

		case enCmTableCellType_Gradient:
			newCell = new cmGradientCell;
			break;

		case enCmTableCellType_SmartNumTextBox:
			newCell = new cmSmartNumericTextCell;
			break;
	}

	if (newCell) {
		newCell->SetParentWindow(parentWnd_);
		newCell->SetFontInfo(fontInfo_);
		newCell->SetDrawArea(CRect(0, 0, colWidthList_[index], 0));
	}

	cells_.push_back(newCell);

	SetDrawArea(drawArea_, true);

	return newCell;
}

cmBaseCell* cmTableRow::GetCell(unsigned int index)
{
	if (index < 0 || index >= cells_.size())
		return NULL;

	return cells_[index];
}

cmBaseCell* cmTableRow::GetFocusedCell(unsigned int& cellIndex)
{
	cmBaseCell* cell = NULL;

	for (unsigned int i = fixedColumnsNumber_; i < cells_.size(); i++) {
		if (cells_[i]->HasFocus()) {
			cell = cells_[i];
			cellIndex = i;
			break;
		}
	}
	
	return cell;
}

cmBaseCell* cmTableRow::GetSelectedCell(unsigned int& cellIndex)
{
	cmBaseCell* cell = NULL;

	for (unsigned int i = fixedColumnsNumber_; i < cells_.size(); i++) {
		if (cells_[i]->IsSelected()) {
			cell = cells_[i];
			cellIndex = i;
			break;
		}
	}
	
	return cell;
}

void cmTableRow::Clear()
{
	for (unsigned int i = 0; i < cells_.size(); i++)
		delete cells_[i];

	cells_.clear();
}

void cmTableRow::SetRowHeight(int height)
{
	rowHeight_ = height;
}

int cmTableRow::GetRowHeight() const
{
	return rowHeight_;
}

void cmTableRow::SetColumnWidthList(const vector<int>& widthList)
{
	colWidthList_.clear();

	for (unsigned int i = 0; i < widthList.size(); i++)
		colWidthList_.push_back(widthList[i]);
}

void cmTableRow::SetColumnWidth(unsigned int index, int width)
{
	if (index < 0 || index >= colWidthList_.size())
		return;

	colWidthList_[index] = width;
}

void cmTableRow::SetParentWnd(CWnd* parentWnd)
{
	parentWnd_ = parentWnd;
}

void cmTableRow::SetFixedColumnsNumber(unsigned int number)
{
	fixedColumnsNumber_ = number;
	firstNonFixedColIndex_ = fixedColumnsNumber_;
}

unsigned int cmTableRow::GetFixedColumnsNumber() const
{
	return fixedColumnsNumber_;
}

void cmTableRow::SetFirstNonFixedColIndex(unsigned int index)
{
	firstNonFixedColIndex_ = index;
}

unsigned int cmTableRow::GetFirstNonFixedColIndex() const
{
	return firstNonFixedColIndex_;
}

cmTableRow::enCmTableRowHitTest cmTableRow::HitTest(const CPoint& pt, unsigned int& index)
{
	int splitterPos = BORDER;
	cmBaseCell* cell;
	unsigned int i;

	if (!drawArea_.PtInRect(pt))
		return enCmTableRowHitTest_None;

	for(i = 0; i < fixedColumnsNumber_ && i < colWidthList_.size(); i++) {
		
		index = i;
		splitterPos += colWidthList_[i];

		if (pt.x >= splitterPos - SplitterArea_ && pt.x <= splitterPos + SplitterArea_)
			return enCmTableRowHitTest_Splitter;

		cell = cells_[i];
		if (cell->HitTest(pt) && i < fixedColumnsNumber_)
			return enCmTableRowHitTest_FixedColumn;

		if (cell->HitTest(pt))
			return enCmTableRowHitTest_NonFixedColumn;

		splitterPos += BORDER;
	}

	for(i = firstNonFixedColIndex_; i < colWidthList_.size(); i++) {
		
		index = i;
		splitterPos += colWidthList_[i];

		if (pt.x >= splitterPos - SplitterArea_ && pt.x <= splitterPos + SplitterArea_)
			return enCmTableRowHitTest_Splitter;

		cell = cells_[i];
		if (cell->HitTest(pt) && i < fixedColumnsNumber_)
			return enCmTableRowHitTest_FixedColumn;

		if (cell->HitTest(pt))
			return enCmTableRowHitTest_NonFixedColumn;

		splitterPos += BORDER;
	}

	return enCmTableRowHitTest_None;
}

bool cmTableRow::ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;
	//enOptionRowHitTest hitTest;
	cmBaseCell* cell;
	
	for (unsigned int i = 0; i < cells_.size(); i++) {
		cell = cells_[i];
		processed = cell->ProcessMessage(msg, lParam, wParam, pt, parentRect, needRedraw);
	}

	return processed;
}

void cmTableRow::SetVisible(bool visible)
{
	visible_ = visible;
}

bool cmTableRow::IsVisible() const
{
	return visible_;
}