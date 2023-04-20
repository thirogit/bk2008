// AsOptionGroup.cpp: implementation of the cmOptionGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "cmOptionGroup.h"
#include "ICmOptionGroupListener.h"

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
#define EXPAND_BUTTON_WIDTH 8
#define EXPAND_BUTTON_HEIGHT 8

cmOptionGroup::cmOptionGroup():
	titleRowHeight_(21),
	propertyRowsHeight_(21),
	verticalStripWidth_(20),
	borderColor_(::GetSysColor(COLOR_3DSHADOW)),
	titleRowBkColor_(::GetSysColor(COLOR_3DFACE)),
	propertyRowsBkColor_(::GetSysColor(COLOR_WINDOW)),
	verticalStripBkColor_(::GetSysColor(COLOR_3DFACE)),
	expanded_(true),
	listener_(NULL),
	parentWnd_(NULL),
	headerIsSelected_(false),
	sorted_(true)
{
	header_.SetBackColor(titleRowBkColor_);
}

cmOptionGroup::~cmOptionGroup()
{
	OptionRowMap::iterator it;
	
	for (it = rows_.begin(); it != rows_.end(); it++)
		delete it->second;
}

void cmOptionGroup::Draw(CDC* pDC)
{
	//TRACE1("DrawGroup %s\n", caption_.c_str());
	OptionRowMap::iterator it;
	CRect tmp;

	header_.Draw(pDC);
	if (expanded_) {
		for (it = rows_.begin(); it != rows_.end(); it++)
			it->second->Draw(pDC);
		}
	
	CPen pen(PS_SOLID, 1, borderColor_);
	CPen* oldPen;

	oldPen = pDC->SelectObject(&pen);

	// Paint vertical strip background
	//
	CBrush b(verticalStripBkColor_);
	
	tmp = drawArea_;
	tmp.right = verticalStripWidth_;
	pDC->FillRect(tmp, &b);
	
	// draw vertical strip border
	//
	pDC->MoveTo(verticalStripWidth_, drawArea_.top);
	pDC->LineTo(verticalStripWidth_, drawArea_.bottom);
	
	if (expanded_) {
		// draw rows borders
		//
		for (it = rows_.begin(); it != rows_.end(); it++) {
			
			tmp = it->second->GetDrawArea();

			pDC->MoveTo(tmp.left, tmp.top - BORDER);
			pDC->LineTo(tmp.right, tmp.top - BORDER);
		}
	}

	// draw expand button
	//
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

void cmOptionGroup::SetCaption(const cmString& caption)
{
	caption_ = caption;

	header_.SetText(caption);	
}

cmString cmOptionGroup::GetCaption() const
{
	return caption_;
}

void cmOptionGroup::SetDescription(const cmString& description)
{
	description_ = description;
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
	row->SetFontInfo(fontInfo_);
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

	// set header draw area
	//
	CRect headerDrawArea;

	headerDrawArea.left = verticalStripWidth_ + BORDER;
	headerDrawArea.right = drawArea.right;
	headerDrawArea.top = drawArea.top;
	headerDrawArea.bottom = drawArea.top + titleRowHeight_ + BORDER;

	header_.SetDrawArea(headerDrawArea);

	if (expanded_) {
		OptionRowMap::iterator it;
		CRect tmp;

		tmp.left = verticalStripWidth_ + BORDER;
		tmp.right = drawArea_.right;
		tmp.top = headerDrawArea.bottom + BORDER;
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

void cmOptionGroup::SetTitleRowBkColor(COLORREF bkColor)
{
	titleRowBkColor_ = bkColor;
}

COLORREF cmOptionGroup::GetTitleRowBkColor() const
{
	return titleRowBkColor_;
}

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

cmOptionGroup::enCmOptionGroupHitTest cmOptionGroup::HitTest(const CPoint& pt)
{
	if (expandRect_.PtInRect(pt))
		return enCmOptionGroupHitTest_Expand;
	
	if (header_.HitTest(pt))
		return enCmOptionGroupHitTest_Header;

	if (drawArea_.PtInRect(pt) && (pt.x > verticalSplitterPos_ - cmOptionRow::SplitterArea_ && pt.x < verticalSplitterPos_ + cmOptionRow::SplitterArea_))
		return enCmOptionGroupHitTest_Splitter;

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
}

bool cmOptionGroup::ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	enCmOptionGroupHitTest hitTest;
	bool processed;

	processed = false;

	switch (msg) {
		case WM_LBUTTONUP:
			hitTest = HitTest(pt);
			switch (hitTest) {
				case enCmOptionGroupHitTest_Expand:
					OnExpandButtonPressed(false);
					needRedraw = true;
					break;

				case enCmOptionGroupHitTest_Header:
					SelectHeader(true, false);
					needRedraw = true;
					break;

				case enCmOptionGroupHitTest_Rows:
					SelectHeader(false, false);
					needRedraw = true;
					break;

				default:
					ClearSelection(false);
			}
			break;

		case WM_LBUTTONDBLCLK:
			hitTest = HitTest(pt);
			switch (hitTest) {
				case enCmOptionGroupHitTest_Expand:
					OnExpandButtonPressed(false);
					needRedraw = true;
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
		processed = row->ProcessMessage(msg, lParam, wParam, pt, parentRect, needRedraw);
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

void cmOptionGroup::SetFontInfo(const cmString& fontName, int fontSize, bool fontBold)
{
	cmFontInfo tmp;

	tmp.SetFontName(fontName);
	tmp.SetFontSize(fontSize);
	tmp.SetFontBold(fontBold);

	SetFontInfo(tmp);
}

void cmOptionGroup::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;

	// update font on all rows
	//
	OptionRowMap::iterator it;
	cmOptionRow* row;
	
	for (it = rows_.begin(); it != rows_.end(); it++) {
		row = it->second;
		row->SetFontInfo(fontInfo);
	}

	header_.SetFontInfo(fontInfo);
}

void cmOptionGroup::SetHeaderFontInfo(const cmFontInfo& fontInfo)
{
	header_.SetFontInfo(fontInfo);
}

void cmOptionGroup::SetHeaderFontInfo(const cmString& fontName, int fontSize, bool fontBold)
{
	cmFontInfo tmp;

	tmp.SetFontName(fontName);
	tmp.SetFontSize(fontSize);
	tmp.SetFontBold(fontBold);

	SetHeaderFontInfo(tmp);
}

cmBaseCell* cmOptionGroup::MoveSelection(	bool moveUp, 
											bool keepTopSelection, 
											bool keepBottomSelection, 
											bool& selectionLost,
											bool redraw)
{
	int rowIndex;
	OptionRowMap::iterator it;
	cmOptionRow* row;
	cmBaseCell* selCell = NULL;
	
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

	if (GetSelectedRow() && listener_)
		listener_->OnSelectionInfoChanged(this, redraw);

	return selCell;
}

void cmOptionGroup::SelectHeader(bool select, bool redraw)
{
	if (headerIsSelected_ == select)
		return;

	headerIsSelected_ = select;

	if (headerIsSelected_) {
		header_.SetBackColor(::GetSysColor(COLOR_HIGHLIGHT));
		header_.SetForeColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		if (listener_)
			listener_->OnSelectionInfoChanged(this);
	} else {
		header_.SetBackColor(titleRowBkColor_);
		header_.SetForeColor(::GetSysColor(COLOR_WINDOWTEXT));
	}

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

void cmOptionGroup::GetSelectionInfo(cmString& caption, 
									 cmString& description,
									 cmString& tooltipText,
									 UINT& tooltipIcon)
{
	if (headerIsSelected_) {
		caption = caption_;
		description = description_;
		tooltipText = _T("");
		tooltipIcon = 0;
	} else {
		cmOptionRow* row;

		row = GetSelectedRow();
		if (row) {
			caption = row->GetCaption();
			description = row->GetDescription();
			tooltipText = row->GetTooltipText();
			tooltipIcon = row->GetTooltipIcon();
		} else {
			caption = _T("");
			description = _T("");
			tooltipText = _T("");
			tooltipIcon = 0;
		}
	}
}

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
	if (listener_)
		listener_->OnSelectionInfoChanged(this, redraw);
}

void cmOptionGroup::SetSorted(bool sorted)
{
	sorted_ = sorted;
}

bool cmOptionGroup::GetSorted() const
{
	return sorted_;
}