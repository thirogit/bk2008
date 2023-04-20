// AsOptionGroup.cpp: implementation of the cmOptionGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "cmOptionGroup.h"
#include "cmDrawUtils.h"
#include "cmCellFactory.h"
#include "cmBaseCellInfo.h"
#include "ICmOptionGroupListener.h"
#include "cmDrawUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define BORDER 1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define EXPAND_BUTTON_WIDTH 8
#define EXPAND_BUTTON_HEIGHT 8

cmOptionGroup::cmOptionGroup():
	titleRowHeight_(18),
	propertyRowsHeight_(18),
	verticalStripWidth_(20),
	borderColor_(::GetSysColor(COLOR_3DSHADOW)),
	//titleRowBkColor_(::GetSysColor(COLOR_3DFACE)),
	propertyRowsBkColor_(::GetSysColor(COLOR_WINDOW)),
	verticalStripBkColor_(::GetSysColor(COLOR_3DFACE)),
	cellBkColor_(::GetSysColor(COLOR_WINDOW)),
	expanded_(true),
	listener_(NULL),
	parentWnd_(NULL),
	headerIsSelected_(false),
	sorted_(true)
{
	header_.SetInfo(cmCellFactory::CreateCellInfo());	
}

cmOptionGroup::~cmOptionGroup()
{
	OptionRowMap::iterator it;
	
	for (it = rows_.begin(); it != rows_.end(); it++)
		delete it->second;
}

void cmOptionGroup::Draw(CDC* pDC, const CPoint& mousePos)
{
	header_.Draw(pDC, mousePos);
	DrawRows(pDC, mousePos);
	DrawGrid(pDC);
	DrawExpandButton(pDC);
}

void cmOptionGroup::SetCaption(const cmString& caption)
{
	caption_ = caption;

	header_.SetText(caption);	
	header_.GetInfo()->SetTitle(caption);

}

cmString cmOptionGroup::GetCaption() const
{
	return caption_;
}

void cmOptionGroup::SetDescription(const cmString& description)
{
	description_ = description;
	header_.GetInfo()->SetDescription(description);
}

cmString cmOptionGroup::GetDescription() const
{
	return description_;
}

OptionRowMap* cmOptionGroup::GetRows()
{
	return &rows_;
}

cmOptionRow* cmOptionGroup::AddRow(const cmString& name, const cmString& caption, enCmCellType valueCellType)
{
	cmOptionRow* row;
	cmString sortKey;

	assert(GetRow(name) == NULL);

	row = new cmOptionRow;

	row->SetParentWindow(parentWnd_);
	row->SetValueCellType(valueCellType);
	row->SetCaption(caption);
	row->RegisterListener(this);
	

	if (!sorted_) {
		cmString buf;

		buf.Format(_T("row_index_%08d"), rows_.size());

		sortKey = buf;
	} else {
		sortKey = name;
	}

	rows_[sortKey] = row;

	if (listener_)
		listener_->OnGroupSizeChanged();

	return row;
}

cmOptionRow* cmOptionGroup::GetRow(const cmString& name)
{
	OptionRowMap::iterator it;
	cmOptionRow* row = NULL;

	it = rows_.find(name);
	if (it != rows_.end())
		row = it->second;

	return row;
}

void cmOptionGroup::SetDrawArea(const CRect& drawArea)
{
	drawArea_ = drawArea;

	drawArea_.bottom = drawArea_.top + titleRowHeight_ + BORDER; // title row height
	if (expanded_)
		drawArea_.bottom += rows_.size() * (propertyRowsHeight_ +  BORDER);

	UpdateHeaderDrawArea(drawArea);
	if (expanded_) {
		OptionRowMap::iterator it;
		CRect tmp;

		tmp.left = verticalStripWidth_ + BORDER;
		tmp.right = drawArea_.right;
		tmp.top = header_.GetDrawArea().bottom + 2 * BORDER;
		tmp.bottom = tmp.top + propertyRowsHeight_;
		for (it = rows_.begin(); it != rows_.end(); it++) {
			
			it->second->SetDrawArea(tmp);

			tmp.OffsetRect(0, propertyRowsHeight_ + BORDER);
		}
	}

	expandRect_.left = drawArea.left + (verticalStripWidth_ - EXPAND_BUTTON_WIDTH) / 2;
	expandRect_.right = expandRect_.left + EXPAND_BUTTON_WIDTH;
	expandRect_.top = drawArea_.top + (header_.GetMinSize().cy - EXPAND_BUTTON_HEIGHT) / 2;
	expandRect_.bottom = expandRect_.top + EXPAND_BUTTON_HEIGHT;

}

CRect cmOptionGroup::GetDrawArea() const
{
	return drawArea_;
}

void cmOptionGroup::SetTitleRowHeight(int height)
{
	titleRowHeight_ = height;
}

int cmOptionGroup::GetTitleRowHeight() const
{
	return titleRowHeight_;
}

void cmOptionGroup::SetPropertyRowsHeight(int height)
{
	propertyRowsHeight_ = height;
}

int cmOptionGroup::GetPropertyRowsHeight() const
{
	return propertyRowsHeight_;
}

void cmOptionGroup::SetVerticalStripWidth(int width)
{
	verticalStripWidth_ = width;
}

int cmOptionGroup::GetVerticalStripWidth() const
{
	return verticalStripWidth_;
}

void cmOptionGroup::SetBorderColor(COLORREF borderColor)
{
	borderColor_ = borderColor;
}

COLORREF cmOptionGroup::GetBorderColor() const
{
	return borderColor_;
}
/*
void cmOptionGroup::SetTitleRowBkColor(COLORREF bkColor)
{
	titleRowBkColor_ = bkColor;
}

COLORREF cmOptionGroup::GetTitleRowBkColor() const
{
	return titleRowBkColor_;
}
*/
void cmOptionGroup::SetPropertyRowsBkColor(COLORREF bkColor)
{
	propertyRowsBkColor_ = bkColor;
}

COLORREF cmOptionGroup::GetPropertyRowsBkColor() const
{
	return propertyRowsBkColor_;
}

void cmOptionGroup::SetVerticalSplitterPos(int pos)
{
	verticalSplitterPos_ = pos;

	OptionRowMap::iterator it;
		
	for (it = rows_.begin(); it != rows_.end(); it++)
		it->second->SetVerticalSplitterPos(pos);
}

int cmOptionGroup::GetVerticalSplitterPos() const
{
	return verticalSplitterPos_;
}

cmOptionGroup::enCmOptionGroupHitTest cmOptionGroup::HitTest(const CPoint& pt, int& itemIndex)
{
	if (expandRect_.PtInRect(pt))
		return enCmOptionGroupHitTest_Expand;
	
	if (header_.HitTest(pt))
		return enCmOptionGroupHitTest_Header;

	if (drawArea_.PtInRect(pt) && (pt.x > verticalSplitterPos_ - cmOptionRow::SplitterArea_ && pt.x < verticalSplitterPos_ + cmOptionRow::SplitterArea_))
		return enCmOptionGroupHitTest_Splitter;
	
	itemIndex = 0;
	for (OptionRowMap::iterator it = rows_.begin(); it != rows_.end(); it++) {
		CRect tmp;
		
		tmp = it->second->GetDrawArea();

		if (pt.x >= tmp.left &&
			pt.x <= tmp.right &&
			pt.y >= tmp.top - BORDER &&
			pt.y <= tmp.top - 1)	
			return enCmOptionGroupHitTest_Grid;

		itemIndex++;
	}
	itemIndex = -1;

	if (drawArea_.PtInRect(pt))
		return enCmOptionGroupHitTest_Rows;

	return enCmOptionGroupHitTest_None;
}


void cmOptionGroup::RegisterListener(ICmOptionGroupListener* listener)
{
	listener_ = listener;
}

void cmOptionGroup::SetParentWindow(CWnd* parentWnd)
{
	parentWnd_ = parentWnd;
	header_.SetParentWindow(parentWnd);
}

bool cmOptionGroup::ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	enCmOptionGroupHitTest hitTest;
	bool processed;
	int itemIndex;

	processed = false;
	hitTest = HitTest(pt, itemIndex);

	switch (msg) {
		case WM_LBUTTONUP:
			if (hitTest == enCmOptionGroupHitTest_Expand) {
				OnExpandButtonPressed(false);
				needRedraw = true;
			} /*else {
				ClearSelection(false);
			}*/
			break;

		case WM_LBUTTONDOWN:
			switch (hitTest) {
				case enCmOptionGroupHitTest_Header:
					SelectHeader(true, false);
					needRedraw = true;
					break;

				case enCmOptionGroupHitTest_Rows:
					SelectHeader(false, false);
					needRedraw = true;
					break;

				case enCmOptionGroupHitTest_Expand:
					break;

				default:
					ClearSelection(false);
			}
			break;

		case WM_LBUTTONDBLCLK:
			
			switch (hitTest) {
				case enCmOptionGroupHitTest_Expand:
					//OnExpandButtonPressed(false);
					//needRedraw = true;
					break;

				case enCmOptionGroupHitTest_Header:
					OnExpandButtonPressed(false);
					needRedraw = true;
					break;
			}
			break;

		case WM_KEYDOWN:
			if (headerIsSelected_) {
				if ((expanded_ && wParam == VK_LEFT) || (!expanded_ && wParam == VK_RIGHT)) {
					OnExpandButtonPressed(false);
					needRedraw = true;
				}
					
			}
			break;


	}


	// send message to each row
	//
	OptionRowMap::iterator it;
	cmOptionRow* row;
	
	for (it = rows_.begin(); it != rows_.end(); it++) {
		row = it->second;
		processed |= row->ProcessMessage(msg, lParam, wParam, pt, parentRect, needRedraw);
	}
	
	if (msg == WM_LBUTTONDOWN && hitTest == enCmOptionGroupHitTest_Grid) {
		SelectRow(itemIndex, false);
		needRedraw = true;
	}

	return processed;
}

void cmOptionGroup::OnExpandButtonPressed(bool redraw)
{
	expanded_ = !expanded_;
	listener_->OnGroupSizeChanged();
	
	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);

	if (!expanded_) {
		ClearSelection(redraw);
		SelectHeader(true, redraw);
	}
}

cmBaseCell* cmOptionGroup::MoveSelection(	int shift, 
											bool keepTopSelection, 
											bool keepBottomSelection, 
											bool& selectionLost,
											bool redraw)
{
	int rowIndex;
	OptionRowMap::iterator it;
	cmOptionRow* row;
	cmBaseCell* selCell = NULL;
	bool moveUp;

	moveUp = (shift < 0);

	selectionLost = false;

	if (headerIsSelected_) {
		if (moveUp) {
			if (!keepTopSelection) {
				SelectHeader(false, redraw);

				selectionLost = true;
			}
		} else {
			// select first row (if exists)
			//
			if (rows_.size() && expanded_) {
				row = rows_.begin()->second;
				row->Select(true, redraw);
				SelectHeader(false, redraw);
				selCell = row->GetCaptionCell();
			} else if (!keepBottomSelection) {
				SelectHeader(false, redraw);
				selectionLost = true;
			}
		}
	} else {

		rowIndex = 0;
		for (it = rows_.begin(); it != rows_.end(); it++) {
			
			row = it->second;

			if (row->IsSelected()) {
				if (moveUp) {
					row->Select(false, redraw);
					if (rowIndex == 0) {
						SelectHeader(true, redraw);
						selCell = &header_;
					} else {
						it--;
						row = it->second;
						row->Select(true, redraw);
						selCell = row->GetCaptionCell();
					}
				} else {
					if (rowIndex == rows_.size() - 1) {
						if (!keepBottomSelection) {
							row->Select(false, redraw);	
							selectionLost = true;
						}
					} else {
						row->Select(false, redraw);
						it++;
						row = it->second;
						row->Select(true, redraw);
						selCell = row->GetCaptionCell();
					}
				}
			}
			rowIndex++;
		}
	}

//	if (GetSelectedRow() && listener_)
//		listener_->OnSelectionInfoChanged(this, redraw);

	return selCell;
}

void cmOptionGroup::SelectHeader(bool select, bool redraw)
{
	if (headerIsSelected_ == select)
		return;

	headerIsSelected_ = select;
	if (select)
		header_.SetSelection(redraw);
	else
		header_.KillSelection(redraw);

/*	
	if (headerIsSelected_) {
		if (listener_)
			listener_->OnSelectionInfoChanged(this);
	}
*/

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow();
}

void cmOptionGroup::SelectBottomRow(bool redraw)
{
	if (rows_.size() == 0 || !expanded_) {
		SelectHeader(true, redraw);
		return;
	}

	rows_.rbegin()->second->Select(true, redraw);
}

void cmOptionGroup::ClearSelection(bool redraw)
{
	SelectHeader(false, redraw);

	OptionRowMap::iterator it;
	cmOptionRow* row;
	
	for (it = rows_.begin(); it != rows_.end(); it++) {
		row = it->second;
		row->Select(false, redraw);
	}
}

cmBaseCell* cmOptionGroup::GetHeaderCell()
{
	return &header_;
}

cmBaseCell* cmOptionGroup::GetFocusedCell()
{
	cmBaseCell* focusedCell = NULL;
	OptionRowMap::iterator it;
	cmOptionRow* row;
	
	for (it = rows_.begin(); it != rows_.end(); it++) {
		row = it->second;
		if (row->HasFocus()) {
			focusedCell = row->GetValueCell();
			break;
		}
	}

	return focusedCell;
}

bool cmOptionGroup::IsSelected()
{
	cmOptionRow* selectedRow;

	selectedRow = GetSelectedRow();

	return headerIsSelected_ || selectedRow;
}

/*
void cmOptionGroup::GetSelectionInfo(cmString& caption, 
									 cmString& description,
									 cmString& tooltipText,
									 UINT& tooltipIconId)
{
	if (headerIsSelected_) {
		caption = caption_;
		description = description_;
		tooltipText = _T("");
		tooltipIconId = 0;
	} else {
		cmOptionRow* row;

		row = GetSelectedRow();
		if (row) {
			caption = row->GetCaption();
			description = row->GetDescription();
			tooltipText = row->GetTooltipText();
			tooltipIconId = row->GetTooltipIcon();
		} else {
			caption.Empty();
			description.Empty();
			tooltipText.Empty();
			tooltipIconId = 0;
		}
	}
}
*/

cmOptionRow* cmOptionGroup::GetSelectedRow()
{
	cmOptionRow* row;
	OptionRowMap::iterator it;

	for (it = rows_.begin(); it != rows_.end(); it++) {
		row = it->second;

		if (row->IsSelected())
			return row;
	}

	return NULL;
}

void cmOptionGroup::OnRowSelection(cmOptionRow* selectedRow, bool redraw)
{
//	if (listener_)
//		listener_->OnSelectionInfoChanged(this, redraw);
}

void cmOptionGroup::SetSorted(bool sorted)
{
	sorted_ = sorted;
}

bool cmOptionGroup::GetSorted() const
{
	return sorted_;
}

void cmOptionGroup::UpdateHeaderDrawArea(const CRect& drawArea)
{
	// set header draw area
	//
	CRect headerDrawArea;

	headerDrawArea.left = verticalStripWidth_;
	headerDrawArea.right = drawArea.right;
	headerDrawArea.top = drawArea.top;
	headerDrawArea.bottom = drawArea.top + titleRowHeight_ + BORDER;

	header_.SetDrawArea(headerDrawArea);
}

void cmOptionGroup::DrawExpandButton(CDC* pDC)
{
	CRect tmp;
	CPen pen(PS_SOLID, 1, borderColor_);
	CPen* oldPen;

	oldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(expandRect_.TopLeft());
	pDC->LineTo(expandRect_.right, expandRect_.top);
	pDC->LineTo(expandRect_.right, expandRect_.bottom);
	pDC->LineTo(expandRect_.left, expandRect_.bottom);
	pDC->LineTo(expandRect_.left, expandRect_.top);

	tmp = expandRect_;
		
	pDC->SelectObject(oldPen);
	if (expanded_) {
		// draw "minus"
		//
		pDC->MoveTo(tmp.left + 2, tmp.CenterPoint().y);
		pDC->LineTo(tmp.right - 1, tmp.CenterPoint().y);
	} else {
		// draw "plus"
		//
		pDC->MoveTo(tmp.left + 2, tmp.CenterPoint().y);
		pDC->LineTo(tmp.right - 1, tmp.CenterPoint().y);

		pDC->MoveTo(tmp.CenterPoint().x, tmp.top + 2);
		pDC->LineTo(tmp.CenterPoint().x, tmp.bottom - 1);
	}
}

void cmOptionGroup::DrawRows(CDC* pDC, const CPoint& mousePos)
{
	if (!expanded_)
		return;

	for (OptionRowMap::iterator it = rows_.begin(); it != rows_.end(); it++)
		it->second->Draw(pDC, mousePos);	
}

void cmOptionGroup::DrawGrid(CDC* pDC)
{
	CPen pen(PS_SOLID, 1, borderColor_);
	CPen* oldPen;
	CRect tmp;

	oldPen = pDC->SelectObject(&pen);

	// paint vertical strip background
	//
	tmp = drawArea_;
	tmp.right = verticalStripWidth_;
	tmp.bottom++;
	pDC->FillSolidRect(tmp, verticalStripBkColor_);
	
	// draw vertical strip border
	//
	pDC->MoveTo(verticalStripWidth_, drawArea_.top);
	pDC->LineTo(verticalStripWidth_, drawArea_.bottom);
	
	if (expanded_) {
		// draw rows borders
		//
		for (OptionRowMap::iterator it = rows_.begin(); it != rows_.end(); it++) {
			
			tmp = it->second->GetDrawArea();

			tmp.InflateRect(1, 1, 0, 0);

			//cmDrawUtils::DrawRect(pDC, tmp, borderColor_);
			pDC->MoveTo(tmp.left, tmp.bottom-1);
			pDC->LineTo(tmp.TopLeft());
			pDC->LineTo(tmp.right, tmp.top);
		}
	}

	// draw bottom border
	//
	pDC->MoveTo(drawArea_.left, tmp.bottom-1);
	pDC->LineTo(drawArea_.right, tmp.bottom-1);

	pDC->SelectObject(oldPen);
}

void cmOptionGroup::SelectRow(int index, bool redraw)
{
	int i = 0;

	for (OptionRowMap::iterator it = rows_.begin(); it != rows_.end(); it++) {
		if (i == index)
			it->second->Select(true, redraw);

		i++;
	}
}