// AsTable.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "MemDC.h"
#include "cmBaseCell.h"
#include "cmBaseCellInfo.h"
#include "cmCellDefs.h"
#include "cmDrawUtils.h"
#include "cmTable.h"
#include "cmTableRow.h"
#include "cmTableRow_Style1.h"

#define SELECTION_COLOR	RGB(80, 80, 80)

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

#define BORDER 1
/////////////////////////////////////////////////////////////////////////////
// cmTable

cmTable::cmTable():
	bkColor_(::GetSysColor(COLOR_BTNFACE)),
	gridColor_(::GetSysColor(COLOR_3DSHADOW)),
	defaultRowHeight_(22),
	horizontalOffset_(0),
	fixedColumnsNumber_(0),
	fixedRowsNumber_(0),
	firstNonFixedRowIndex_(0),
	firstNonFixedColumnIndex_(0),
	rowsNumber_(0),
	columnsNumber_(0),
	vScrollMinPos_(0),
	vScrollMaxPos_(0),
	vScrollPageSize_(0),
	hScrollMinPos_(0),
	hScrollMaxPos_(0),
	hScrollPageSize_(0),
	selSplitterIndex_(-1),
	splitterRowIndex_(-1),
	draggingSplitter_(false),
	selectedRowIndex_(-1),
	selectedCellIndex_(-1),
	rowSelectionMode_(cmHeaderSelectionMode_None),
	columnSelectionMode_(cmHeaderSelectionMode_None),
	selectedRowsCount_(0),
	selectedColsCount_(0),
	borderEnabled_(true)
{
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    //HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, _T("cmTable"), &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = _T("cmTable");

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return;
        }
    }
}

cmTable::~cmTable()
{
	Clear();
}

BEGIN_MESSAGE_MAP(cmTable, CWnd)
	//{{AFX_MSG_MAP(cmTable)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_GETDLGCODE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETFOCUS()
	ON_MESSAGE(CMC_SETFOCUS, OnCellSetFocus)
	ON_MESSAGE(CMC_LOSTFOCUS, OnCellLostFocus)
	ON_MESSAGE(CMC_SETSEL, OnCellSetSelection)
	ON_MESSAGE(CMC_LOSTSEL, OnCellLostSelection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// cmTable message handlers

BOOL cmTable::Create(const CRect& rc, CWnd* parentWnd, DWORD style, UINT nID)
{
	BOOL ret;

	style |= WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	ret = CWnd::Create(_T("cmTable"), _T("CodeMachines.com Table"), style, rc, parentWnd, nID, NULL);

	if (ret)
		ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	return ret;
}


BOOL cmTable::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void cmTable::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	UpdateWindowSize();

}

void cmTable::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(&dc);
	CPoint mousePos;

	GetCursorPos(&mousePos);
	ScreenToClient(&mousePos);

	DrawBackground(&memDC);
	DrawRows(&memDC, mousePos);
	DrawGrid(&memDC);
	DrawBorder(&memDC);
	DrawSelectedCell(&memDC);
}

void cmTable::DrawBackground(CDC* pDC)
{
	CRect rc;

	GetClientRect(rc);
	pDC->FillSolidRect(rc, bkColor_);
}

void cmTable::DrawBorder(CDC* pDC)
{
	CRect rc;

	GetClientRect(rc);

	// draw external border
	//
	if (borderEnabled_)
		cmDrawUtils::DrawRect(pDC, rc, gridColor_);
}

void cmTable::DrawGrid(CDC* pDC)
{
	// main border
	//
	CRect rc;
	CRect tmp;
	unsigned int rowIndex;
	unsigned int colIndex;
	CPen pen(PS_SOLID, 1, gridColor_);
	CPen* oldPen;

	GetClientRect(rc);	

	if (borderEnabled_)
	rc.DeflateRect(0, 0, 1, 1);

	if (rows_.size() == 0)
		return;

	// draw gridlines
	//
	oldPen = pDC->SelectObject(&pen);
	
	// draw vertical lines
	//
	cmTableRow* row;
	CRect drawArea;
	int x;
	int top;
	int bottom;
	int left;
	int right;
		
	row = rows_[0];
	top = rc.top;
	bottom = rows_.size() * (row->GetRowHeight() + BORDER);
	for (colIndex = 0; colIndex < row->GetNumColumns(); colIndex++) {
		drawArea = row->GetCell(colIndex)->GetDrawArea();
		x = drawArea.right;
		if (x <= rc.right) {
			pDC->MoveTo(x, top);
			pDC->LineTo(x, bottom);
		}
	}

	// draw horizontal lines
	//
	
	left = rc.left;
	for (rowIndex = 0; rowIndex < rows_.size(); rowIndex++) {
		row = rows_[rowIndex];
		drawArea = row->GetCell(0)->GetDrawArea();
		bottom = drawArea.bottom;
		right = row->GetCell(row->GetNumColumns() - 1)->GetDrawArea().right;
		pDC->MoveTo(left, bottom);
		pDC->LineTo(right, bottom);
	}

	pDC->SelectObject(oldPen);

	pen.DeleteObject();
}

void cmTable::DrawRows(CDC* pDC, const CPoint& mousePos)
{
	unsigned int i;
	CRect rc;
	cmTableRow* row;
	int lastBottom;
	CRect drawArea;
	
	GetClientRect(rc);

	for (i = 0; i < fixedRowsNumber_ && i < rows_.size(); i++) {
		row = rows_[i];
		if (row)
			row->Draw(pDC, mousePos);
	}
	lastBottom = 0;
	for (i = firstNonFixedRowIndex_; i < rows_.size(); i++) {
		row = rows_[i];
		if (row) {
			drawArea = row->GetDrawArea();
			if (!rc.PtInRect(drawArea.TopLeft()) || drawArea.top < lastBottom)
				break;
			//TRACE1("drawing row %s\n", row->GetCell(0)->GetText().c_str());
			row->Draw(pDC, mousePos);
		}
		lastBottom = drawArea.bottom + BORDER;
	}
}

void cmTable::UpdateWindowSize(bool redraw)
{
	UpdateDrawArea();
		
	UpdateFirstNonFixedRowIndex();
	UpdateFirstNonFixedColumnIndex();

	// update vertical scrollbar
	// 
	SCROLLINFO si;
	
	vScrollMinPos_ = 0;
	vScrollMaxPos_ = rows_.size() -  fixedRowsNumber_ - 1;
	vScrollPageSize_ = GetNonFixedRowsPerPage();

	// Setup scroll info
	//
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = vScrollMinPos_;
	si.nMax = vScrollMaxPos_;
	si.nPage = vScrollPageSize_;

	// Set scroll info
	if ((int)si.nPage > si.nMax)
		firstNonFixedRowIndex_ = fixedRowsNumber_;

	SetScrollInfo(SB_VERT, &si, redraw);
	
	// update horizontal scrollbar
	//
	hScrollMinPos_ = 0;
	hScrollMaxPos_ = columnsNumber_ - fixedColumnsNumber_ - 1;
	hScrollPageSize_ = GetNonFixedColumnsPerPage();

	// Setup scroll info
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = hScrollMinPos_;
	si.nMax = hScrollMaxPos_;
	si.nPage = hScrollPageSize_;

	// Set scroll info
	//
	if ((int)si.nPage > si.nMax)
		firstNonFixedColumnIndex_ = fixedColumnsNumber_;
	
	SetScrollInfo(SB_HORZ, &si, TRUE);
	
	if (redraw) {
		RedrawWindow();
		// -- Redraw
		//Invalidate();

		// -- Update window
		//UpdateWindow();
	}
}

void cmTable::Clear()
{
	for (unsigned int i = 0; i < rows_.size(); i++)
		delete rows_[i];

	rows_.clear();
}

cmTableRow* cmTable::AppendRow(unsigned int numColumns, enTableRowType type, bool redraw)
{
	return InsertRow(rows_.size(), numColumns, type, redraw);
}

cmTableRow* cmTable::InsertRow(unsigned int index, unsigned int numColumns, enTableRowType type, bool redraw)
{
	cmTableRow* newRow;

	switch (type) {
		case enTableRowType_Classic:
	newRow = new cmTableRow;
			break;

		case enTableRowType_Style1:
			newRow = new cmTableRow_Style1;
			break;
	}
	
	newRow->SetParentWnd(this);
	newRow->SetNumColumns(numColumns);
	newRow->SetColumnWidthList(colWidth_);
	newRow->SetParentWnd(this);
	newRow->SetGridColor(gridColor_);
	newRow->SetColumnsInfo(colNames_, colDescriptions_);
	newRow->SetFixedColumnsNumber(fixedColumnsNumber_);
	if (index < 0)
		index = 0;
	else if (index >= rows_.size())
		index = rows_.size();

	rows_.insert(rows_.begin() + index, newRow);

	if (!redraw)
		SetRedraw(FALSE);

	UpdateWindowSize(redraw);

	if (!redraw)
		SetRedraw();

	return newRow;
}

void cmTable::DeleteRow(unsigned int index, bool redraw)
{	
	if(index < 0 || index >= rows_.size())
		return;

	delete rows_[index];

	rows_.erase(rows_.begin() + index);

	if (redraw) {
		UpdateDrawArea();
		RedrawWindow();
	}
}

cmTableRow* cmTable::GetRow(unsigned int index)
{
	if(index < 0 || index >= rows_.size())
		return NULL;

	return rows_[index];
}

cmBaseCell* cmTable::GetCell(unsigned int row, unsigned int col)
{
	cmTableRow* r;

	r = GetRow(row);
	if (r)
		return r->GetCell(col);

	return NULL;
}

unsigned int cmTable::GetRowCount() const
{
	return rows_.size();
}

void cmTable::DeleteAll(bool redraw)
{
	Clear();

	if (redraw)
		RedrawWindow();
}

void cmTable::UpdateDrawArea()
{
	CRect rc;
	CRect drawArea;
	CRect tmp;
	cmTableRow* row;
	int totalHeight;
	unsigned int i;
	int rowHeight;

	GetClientRect(rc);

	if (rc.IsRectEmpty())
		return;

	//rc.InflateRect(horizontalOffset_, verticalOffset_, 0, 0);
	if (borderEnabled_)
	rc.DeflateRect(BORDER, BORDER, BORDER, BORDER);	// remove borders 

	drawArea = rc;
	
	totalHeight = 0;
	i = 0;

	// calculate draw areas for fixed rows
	//
	for (i = 0; (i < fixedRowsNumber_) && (i < rows_.size()); i++) {
		row = rows_[i];	
		drawArea.bottom = drawArea.top + row->GetRowHeight();
		row->SetDrawArea(drawArea, draggingSplitter_);

		totalHeight += drawArea.Height() + BORDER;
		if (totalHeight > rc.Height())
			break;
		
		tmp = rc;
		tmp.top = drawArea.bottom + BORDER;
		drawArea = tmp;
	}
	
	// calculate draw areas for non fixed rows
	//
	//TRACE1("------------------------------------\n", i - 1);
	for (; i < firstNonFixedRowIndex_ && i < rows_.size(); i++)
		rows_[i]->SetDrawArea(CRect(0,0,0,0), false);

	i = firstNonFixedRowIndex_;
	while (/*(totalHeight < rc.Height()) &&*/ (i < rows_.size())) {
		row = rows_[i];
		
		//TRACE1("updating row %s\n", row->GetCell(0)->GetText().c_str());

		rowHeight = row->GetRowHeight();
		drawArea.bottom = drawArea.top + rowHeight;
		row->SetDrawArea(drawArea, draggingSplitter_);
		row->SetVisible(true);
		//drawArea = row->GetDrawArea();

		totalHeight += rowHeight + BORDER;
				
		drawArea.OffsetRect(0, rowHeight + BORDER);
		
		i++;
	}

	//for (; i < rows_.size(); i++)
	//	rows_[i]->SetDrawArea(CRect(0,0,0,0), false);
}

void cmTable::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{	
	// Declare variables
	SCROLLINFO si;
	CRect rcClient;
	int newIndex;

	// Setup vertical scrollbar information
	//
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE;

	// Get scrollbar information
	//
	//GetScrollInfo(SB_VERT, &si);

	newIndex = firstNonFixedRowIndex_;
	// Switch scrollbar code
	//
	switch (nSBCode)
	{
		case SB_LINEDOWN:
			firstNonFixedRowIndex_++;
			break;

		case SB_LINEUP:
			firstNonFixedRowIndex_--;
			break;

		case SB_PAGEDOWN:
			firstNonFixedRowIndex_ += vScrollPageSize_;
			break;

		case SB_PAGEUP:
			firstNonFixedRowIndex_ -= vScrollPageSize_;
			break;

		case SB_THUMBTRACK:
			firstNonFixedRowIndex_ = nPos + fixedRowsNumber_;
			break;

		default:
			return;
	}

	// Calculate
	firstNonFixedRowIndex_ = min(vScrollMaxPos_ - vScrollPageSize_ + fixedRowsNumber_ + 1, firstNonFixedRowIndex_);
	firstNonFixedRowIndex_ = max(firstNonFixedRowIndex_, vScrollMinPos_ + fixedRowsNumber_);

	// Set scrollbar info
	si.fMask = SIF_POS;
	si.nPos = firstNonFixedRowIndex_ - fixedRowsNumber_;
	SetScrollInfo(SB_VERT, &si, TRUE);

	UpdateDrawArea();
	
	RedrawWindow();

}

void cmTable::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Declare variables
	SCROLLINFO si;
	CRect rcClient;
	unsigned int newIndex;
	cmTableRow* row;

	// Setup vertical scrollbar information
	//
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE;

	// Get scrollbar information
	//
	row = rows_[0];

	newIndex = row->GetFirstNonFixedColIndex();
	// Switch scrollbar code
	//
	switch (nSBCode)
	{
		case SB_LINEDOWN:
			newIndex++;
			break;

		case SB_LINEUP:
			newIndex--;
			break;

		case SB_PAGEDOWN:
			newIndex += hScrollPageSize_;
			break;

		case SB_PAGEUP:
			newIndex -= hScrollPageSize_;
			break;

		case SB_THUMBTRACK:
			newIndex = nPos + fixedColumnsNumber_;
			break;

		default:
			return;
	}

	// Calculate
	newIndex = min(hScrollMaxPos_ - hScrollPageSize_ + fixedColumnsNumber_ + 1, newIndex);
	newIndex = max(newIndex, vScrollMinPos_ + fixedColumnsNumber_);
	
	for(unsigned int i = 0; i < rows_.size(); i++)
		rows_[i]->SetFirstNonFixedColIndex(newIndex);

	// Set scrollbar info
	si.fMask = SIF_POS;
	si.nPos = newIndex - fixedColumnsNumber_;
	SetScrollInfo(SB_HORZ, &si, TRUE);

	UpdateDrawArea();
	
	RedrawWindow();
}

void cmTable::SetFixedRowsNumber(unsigned int number)
{
	fixedRowsNumber_ = number;
	firstNonFixedRowIndex_ = fixedRowsNumber_;
}

unsigned int cmTable::GetFixedRowsNumber() const
{
	return fixedRowsNumber_;
}

void cmTable::SetFixedColumnsNumber(unsigned int number)
{
	fixedColumnsNumber_ = number;

	for (unsigned int i = 0; i < rows_.size(); i++)
		rows_[i]->SetFixedColumnsNumber(number);
}

unsigned int cmTable::GetFixedColumnsNumber() const
{
	return fixedColumnsNumber_;
}

unsigned int cmTable::GetNonFixedRowsPerPage()
{
	//return 10;

	CRect rc;
	unsigned int i;
	int fixedAreaHeight = 0;
	int variableAreaHeight;
	unsigned int rowsPerPage;
	int tmpHeight;
	int rowHeight;
	cmTableRow* row;

	GetClientRect(rc);
	rc.DeflateRect(BORDER, BORDER, BORDER, BORDER);
	for (i = 0; (i < fixedRowsNumber_) && (i < rows_.size()); i++) {
		row = rows_[i];
		rowHeight = row->GetDrawArea().Height();
		fixedAreaHeight += rowHeight;
		fixedAreaHeight += BORDER;
	}

	variableAreaHeight = rc.Height() - fixedAreaHeight;

	i = firstNonFixedRowIndex_;
	rowsPerPage = 0;
	tmpHeight = 0;
	for (i = firstNonFixedRowIndex_; i < rows_.size(); i++) {
		row = rows_[i];
		rowHeight = row->GetDrawArea().Height();
		if (rowHeight == 0 || tmpHeight + rowHeight > variableAreaHeight)
			break;

		tmpHeight += rowHeight;
		tmpHeight += BORDER;
		rowsPerPage++;
	}
	
	//TRACE1("rows per page = %d\n", rowsPerPage);

	return rowsPerPage;
}

unsigned int cmTable::GetNonFixedColumnsPerPage()
{
	if (rows_.size() == 0)
		return 0;

	CRect rc;
	unsigned int i;
	int fixedAreaWidth = 0;
	int variableAreaWidth;
	int columnsPerPage;
	int tmpWidth;
	int cellWidth;
	cmTableRow* row;
	cmBaseCell* cell;

	GetClientRect(rc);
	rc.DeflateRect(BORDER, BORDER, BORDER, BORDER);
	
	row = rows_[0];
	
	for (i = 0; (i < fixedColumnsNumber_) && (i < columnsNumber_); i++) {
		cell = row->GetCell(i);
		if (cell == NULL)
			break;

		cellWidth = cell->GetDrawArea().Width();
		fixedAreaWidth += cellWidth;
		fixedAreaWidth += BORDER;
	}

	variableAreaWidth = rc.Width() - fixedAreaWidth;

	
	columnsPerPage = 0;
	tmpWidth = 0;
	for (i = row->GetFirstNonFixedColIndex(); i < columnsNumber_; i++) {
		cell = row->GetCell(i);
		if (cell == NULL)
			break;

		cellWidth = cell->GetDrawArea().Width();
		if (cellWidth == 0 || tmpWidth + cellWidth > variableAreaWidth)
			break;

		tmpWidth += cellWidth;
		tmpWidth += BORDER;
		columnsPerPage++;
	}
	
	//TRACE1("columns per page = %d\n", columnsPerPage);

	return columnsPerPage;
}


void cmTable::SetColumnWidth(unsigned int index, int width)
{
	if (index < 0 || index >= columnsNumber_)
		return;

	colWidth_[index] = width;

	// update all rows
	//
	for (unsigned int i = 0; i < rows_.size(); i++)
		rows_[i]->SetColumnWidth(index, width);
}

int cmTable::GetColumnWidth(unsigned int index)
{
	if (index < 0 || index >= columnsNumber_)
		return -1;

	return colWidth_[index];
}

void cmTable::SetColumnsNumber(unsigned int number, int width)
{
	columnsNumber_ = number;

	colWidth_.clear();

	for (unsigned int i = 0; i < number; i++)
		colWidth_.push_back(width);
}

unsigned int cmTable::GetColumnsNumber() const
{
	return columnsNumber_;
}

// This method updates the index of the first non-fixed row
// Usually called when the table is resized
//
void cmTable::UpdateFirstNonFixedRowIndex()
{
	CRect rc;
	CRect fixedRowsArea;
	int nonFixedAreaHeight;
	int tmpHeight;
	unsigned int i;
	int old;
	
	if (rows_.size() == 0)
		return;

	old = firstNonFixedRowIndex_;
	GetClientRect(rc);
	rc.DeflateRect(0, BORDER, 0, BORDER);
	fixedRowsArea = GetFixedRowsArea();
	
	nonFixedAreaHeight = rc.Height() - fixedRowsArea.Height();
	
	// calculate the height of the "non-fixed-rows area"
	//
	tmpHeight = 0;
	for (i = firstNonFixedRowIndex_; i < rows_.size(); i++) {
		tmpHeight += rows_[i]->GetDrawArea().Height();
		tmpHeight += BORDER;
	
		if (tmpHeight >= nonFixedAreaHeight)
			return;
	}

	if (tmpHeight >= nonFixedAreaHeight)
		return;

	if (firstNonFixedRowIndex_ >= 1) {
		for (i = firstNonFixedRowIndex_ - 1; i >= fixedRowsNumber_; i--) {
			tmpHeight += rows_[i]->GetDrawArea().Height();
			tmpHeight += BORDER;
	
			if (tmpHeight >= nonFixedAreaHeight)
				break;
	
			firstNonFixedRowIndex_--;
		}
	}

	if (old != firstNonFixedRowIndex_)
		UpdateDrawArea();

	//TRACE1("new first row = %d\n", firstNonFixedRowIndex_);
}

void cmTable::UpdateFirstNonFixedColumnIndex()
{
	CRect rc;
	CRect fixedColsArea;
	int nonFixedAreaWidth;
	int tmpWidth;
	unsigned int i;
	cmTableRow* row;
	cmBaseCell* cell;
	int old;

	if (rows_.size() == NULL)
		return;

	old = firstNonFixedColumnIndex_;
	row = rows_[0];
	
	GetClientRect(rc);
	rc.DeflateRect(BORDER, BORDER, BORDER, BORDER);
	fixedColsArea = GetFixedColsArea();
	
	nonFixedAreaWidth = rc.Width() - fixedColsArea.Width();
	
	tmpWidth = 0;
	firstNonFixedColumnIndex_ = row->GetFirstNonFixedColIndex();
	for (i = firstNonFixedColumnIndex_; i < columnsNumber_; i++) {
		cell = row->GetCell(i);
		if (cell == NULL)
			break;

		tmpWidth += cell->GetDrawArea().Width();
		tmpWidth += BORDER;
	}

	if (tmpWidth >= nonFixedAreaWidth)
		return;

	firstNonFixedColumnIndex_ = row->GetFirstNonFixedColIndex();
	for (i = firstNonFixedColumnIndex_ - 1; i >= fixedColumnsNumber_; i--) {
		cell = row->GetCell(i);
		if (cell == NULL)
			break;

		tmpWidth += cell->GetDrawArea().Width();
		tmpWidth += BORDER;

		if (tmpWidth >= nonFixedAreaWidth)
			break;

		firstNonFixedColumnIndex_--;
	}

	//TRACE1("new first column = %d\n", firstNonFixedColumnIndex_);
	if (firstNonFixedColumnIndex_ != old) {
		UpdateDrawArea();
		for (i = 0; i < rows_.size(); i++)
			rows_[i]->SetFirstNonFixedColIndex(firstNonFixedColumnIndex_);
	}
}

CRect cmTable::GetFixedRowsArea()
{
	CRect area;
	CRect rc;
	cmTableRow* row;
	int rowHeight;

	GetClientRect(rc);
	area = rc;
	area.bottom = area.top;
	
	for (unsigned int i = 0; (i < fixedRowsNumber_) && (i < rows_.size()); i++) {
		row = rows_[i];
		rowHeight = row->GetDrawArea().Height();
		area.bottom += rowHeight;
		area.bottom += BORDER;
	}

	return area;
}

CRect cmTable::GetFixedColsArea()
{
	CRect area;
	CRect rc;
	cmTableRow* row;
	cmBaseCell* cell;
	int colWidth;

	GetClientRect(rc);
	area = rc;
	area.right = area.left;
	
	if (rows_.size() > 0) {
		row = rows_[0];

		for (unsigned int i = 0; (i < fixedRowsNumber_) && (i < rows_.size()); i++) {
			cell = row->GetCell(i);
			if (cell) {
				colWidth = cell->GetDrawArea().Width();
				area.right += colWidth;
				area.right += BORDER;
			}
		}
	}

	//TRACE2("fixed cols area topleft(%d, %d)", area.top, area.left);
	//TRACE2(" bottomright(%d, %d)\n", area.bottom, area.right);
	return area;
}

BOOL cmTable::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	cmTableRow::enCmTableRowHitTest hitTest;
	CPoint pt;
	unsigned int index;

	GetCursorPos(&pt);
	ScreenToClient(&pt);

	for (unsigned int i = 0; i < rows_.size(); i++) {
		hitTest = rows_[i]->HitTest(pt, index);

		if (hitTest == cmTableRow::enCmTableRowHitTest_Splitter) {
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			return TRUE;
		}
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


unsigned int cmTable::HitSplitter(const CPoint& point) 
{
	cmTableRow::enCmTableRowHitTest hitTest;
	unsigned int splitterIndex;

	for (unsigned int i = 0; i < rows_.size(); i++) {
		hitTest = rows_[i]->HitTest(point, splitterIndex);

		splitterRowIndex_ = i;
		if (hitTest == cmTableRow::enCmTableRowHitTest_Splitter)
			return splitterIndex;
		
	}

	splitterRowIndex_ = -1;

	return -1;
}

BOOL cmTable::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (zDelta > 0) 
		OnVScroll(SB_LINEUP, 0, NULL);
	else if (zDelta < 0)
		OnVScroll(SB_LINEDOWN, 0, NULL);  

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

bool cmTable::MoveSelection(int horizontalShift, int verticalShift, bool redraw)
{
	cmTableRow* selRow;		
	cmBaseCell* selCell;
//	bool itemFound;
//	unsigned int selRowIndex;
//	unsigned int selCellIndex;
	int newRowIndex;
	int newCellIndex;

	//itemFound = GetSelectedItem(&selRow, &selCell, selRowIndex, selCellIndex);

	//if (!itemFound)
	//	return false;

	selCell = GetCell(selectedRowIndex_, selectedCellIndex_);

	newRowIndex = max((int)(selectedRowIndex_ + verticalShift), 0);
	newCellIndex = max((int)(selectedCellIndex_ + horizontalShift), 0);

	if (newRowIndex < fixedRowsNumber_ || newRowIndex >= rows_.size())
		return false;

	if (newCellIndex < fixedColumnsNumber_ && (rowSelectionMode_ != cmHeaderSelectionMode_Single) )
		return false;

	if (selCell->HasFocus() && horizontalShift != 0)
		return false;

	// kill selection on active cell
	//
	selCell->KillSelection(false);
	
	// get new cell and setfocus
	//
	selRow = rows_[newRowIndex];
	if ((selectedCellIndex_ < fixedColumnsNumber_) && 
		(rowSelectionMode_ == cmHeaderSelectionMode_Single)) {
		// select entire row
		//
		selRow->SetSelection(true, false);
	} else {
		// select single cell
		//
	selCell = selRow->GetCell(newCellIndex);
		assert(selCell);
		if (selCell == NULL)
			return false;

	selCell->SetSelection(false);
	}

	EnsureVisible(newRowIndex, newCellIndex, false);
	
	selectedRowIndex_ = newRowIndex;
	selectedCellIndex_ = newCellIndex;

	selectedRowsCount_ = 1;
	selectedColsCount_ = 1;

	if (redraw) {
		SetRedraw();
		RedrawWindow();
	}

	return true;
}

bool cmTable::GetFocusedItem(cmTableRow** row, cmBaseCell** cell, unsigned int& rowIndex, unsigned int& cellIndex)
{
	*row = NULL;
	*cell = NULL;

	for (unsigned int i = fixedRowsNumber_; i < rows_.size(); i++) {
		*cell = rows_[i]->GetFocusedCell(cellIndex);
		if (*cell) {
			*row = rows_[i];
			rowIndex = i;
			return true;
		}
	}

	return false;
}

bool cmTable::GetSelectedCell(cmTableRow** row, cmBaseCell** cell, unsigned int& rowIndex, unsigned int& cellIndex)
{
	*row = NULL;
	*cell = NULL;

	for (unsigned int i = fixedRowsNumber_; i < rows_.size(); i++) {
		*cell = rows_[i]->GetSelectedCell(cellIndex);
		if (*cell) {
			*row = rows_[i];
			rowIndex = i;
			return true;
		}
	}

	return false;
}

bool cmTable::GetSelectedItem(cmTableRow** row, cmBaseCell** cell, unsigned int& rowIndex, unsigned int& cellIndex)
{
	*row = NULL;
	*cell = NULL;

	for (unsigned int i = 0; i < rows_.size(); i++) {
		*cell = rows_[i]->GetSelectedItem(cellIndex);
		if (*cell) {
			*row = rows_[i];
			rowIndex = i;
			return true;
		}
	}

	return false;
}

void cmTable::SetCellFocus(bool redraw)
{
	cmTableRow* selRow;
	cmBaseCell* selCell;
	bool itemFound;

	itemFound = GetSelectedCell(&selRow, &selCell, selectedRowIndex_, selectedCellIndex_);
	if (itemFound)
		selCell->SetFocus(redraw);
}

void cmTable::KillCellFocus(bool storeData)
{
	cmTableRow* selRow;
	cmBaseCell* selCell;
	unsigned int rowIndex;
	unsigned int cellIndex;
	bool itemFound;

	itemFound = GetFocusedItem(&selRow, &selCell, rowIndex, cellIndex);
	if (itemFound)
		selCell->KillFocus(storeData);
}

void cmTable::EnsureVisible(unsigned int rowIndex, unsigned int cellIndex, bool redraw)
{
	firstNonFixedColumnIndex_ = rows_[0]->GetFirstNonFixedColIndex();

	SetRedraw(FALSE);
	if (rowIndex < firstNonFixedRowIndex_) {
		OnVScroll(SB_LINEUP, 0, NULL);
		return;
	}

	if (rowIndex > firstNonFixedRowIndex_ + GetNonFixedRowsPerPage()) {
		OnVScroll(SB_LINEDOWN, 0, NULL);
		return;
	}

	if (cellIndex < firstNonFixedColumnIndex_) {
		OnHScroll(SB_LINEUP, 0, NULL);
		return;
	}

	if (cellIndex > firstNonFixedColumnIndex_ + GetNonFixedColumnsPerPage() - 1) {
		OnHScroll(SB_LINEDOWN, 0, NULL);
		return;
	}

	if (redraw) {
		SetRedraw();
		RedrawWindow();
	}
}

void cmTable::DeleteNonFixedRows()
{
	if (rows_.size() > fixedRowsNumber_) {
		for (unsigned int i = fixedRowsNumber_; i < rows_.size(); i++)
			delete rows_[i];

		rows_.erase(rows_.begin() + fixedRowsNumber_, rows_.end());	
	}
}

void cmTable::DrawSelectedCell(CDC* pDC)
{
	return;

	cmBaseCell* selectedCell;
	unsigned int row;
	unsigned int col;
	CRect tmp;

	selectedCell = GetSelectedCell(row, col);
	if (selectedCell) {
		CPen p(PS_SOLID, 3, SELECTION_COLOR);
		CPen* oldPen;

		tmp = selectedCell->GetDrawArea();
		
		oldPen = pDC->SelectObject(&p);
		
		pDC->MoveTo(tmp.left - 1, tmp.top - 1);
		pDC->LineTo(tmp.right, tmp.top - 1);
		pDC->LineTo(tmp.right, tmp.bottom);
		pDC->LineTo(tmp.left - 1, tmp.bottom);
		pDC->LineTo(tmp.left - 1, tmp.top - 1);

		pDC->SelectObject(oldPen);
		
		
	} 
}

cmBaseCell* cmTable::GetSelectedCell(unsigned int& rowIndex, unsigned int& colIndex)
{
	cmBaseCell* ret = NULL;
	cmTableRow* row;
	
	for (unsigned int i = firstNonFixedRowIndex_; i < rows_.size(); i++) {
		row = rows_[i];
		if (row) {
			ret = row->GetSelectedCell(colIndex);
			if (ret) {
				rowIndex = i;
				return ret;
			}
			
		}
	}

	rowIndex = -1;
	colIndex = -1;

	return NULL;
}

cmBaseCell* cmTable::GetSelectedHeader(unsigned int& rowIndex, unsigned int& colIndex)
{
	cmBaseCell* ret = NULL;
	cmTableRow* row;
	unsigned int i;

	// search in fixed rows
	//
	for (i = 0; i < firstNonFixedRowIndex_; i++) {
		row = rows_[i];
		if (row) {
			ret = row->GetSelectedCell(colIndex);
			if (ret) {
				rowIndex = i;
				return ret;
			}
		}
	}

	// search in fixed columns
	//
	for (i = firstNonFixedRowIndex_; i < rows_.size(); i++) {
		row = rows_[i];
		for (unsigned int j = 0; j < fixedColumnsNumber_; j++) {
			ret = row->GetCell(j);
			if (ret && ret->IsSelected()) {
				rowIndex = i;
				colIndex = j;
				return ret;
			}
		}
	} 

	rowIndex = -1;
	colIndex = -1;

	return NULL;
}

void cmTable::OnMouseMove( UINT nFlags, CPoint pt)
{
	if (draggingSplitter_ && (nFlags & MK_LBUTTON)) {
		
		CRect rc;
		int minPos;
		//int maxPos;
		CPoint point(pt);
		GetClientRect(rc);
		cmBaseCell* cell;
		int cellWidth;
		
		cell = rows_[splitterRowIndex_]->GetCell(selSplitterIndex_);
		cellWidth = cell->GetDrawArea().Width();
		if (selSplitterIndex_ > 0)
			minPos = cell->GetDrawArea().left + 5;
		else
			minPos = 5;
		
		point.x = max(point.x, minPos);

		cellWidth = point.x - cell->GetDrawArea().left;
		SetColumnWidth(selSplitterIndex_, cellWidth);
		UpdateDrawArea();
		
		RedrawWindow();
	}
}

void cmTable::OnLButtonDown( UINT nFlags, CPoint pt)
{
	bool needRedraw = false;

	selSplitterIndex_ = HitSplitter(pt);
	if (selSplitterIndex_ != -1) {
		draggingSplitter_ = true;
	} else {
		
		cmTableRow* row;
		cmTableRow* selRow;
		cmBaseCell* selCell;
		CRect rc;
		vector<unsigned int> selColListBefore;
		vector<unsigned int> selRowListBefore;
		//unsigned int selRowIndex;
		//unsigned int selColIndex;
		bool ret;

		if ((nFlags & MK_CONTROL)) {
			
			// store selected columns
			//
			if (columnSelectionMode_ == cmHeaderSelectionMode_Multiple)
				selColListBefore = GetSelColList();
			
			// store selected rows
			//
			if (rowSelectionMode_ == cmHeaderSelectionMode_Multiple)
				selRowListBefore = GetSelRowList();
		}

		GetClientRect(rc);
		for (unsigned int i = 0; i < rows_.size(); i++) {
			row = rows_[i];
			row->ProcessMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(pt.x, pt.y), pt, rc, needRedraw);
		}

		// check row or column header selection
		//
		ret = GetSelectedItem(&selRow, &selCell, selectedRowIndex_, selectedCellIndex_); //selRowIndex, selColIndex);
		if (!ret)
			return;

		if ((selectedRowIndex_ < fixedRowsNumber_) && 
			(columnSelectionMode_ != cmHeaderSelectionMode_None)) {
			
			// there is a selection on a fixed row : this means we have to select a column
			//
			int i;
			for (i = 0; i < selColListBefore.size(); i++)
				SelectColumn(selColListBefore[i], false);

			SelectColumn(selectedCellIndex_);
			selectedColsCount_ = i + 1;
			needRedraw = true;

		} else if ((selectedCellIndex_ < fixedColumnsNumber_) && 
				   (rowSelectionMode_ != cmHeaderSelectionMode_None)) {
			
			// there is a selection on a fixed row : this means we have to select a column
			//
			int i;
			
			for (i = 0; i < selRowListBefore.size(); i++)
				SelectRow(selRowListBefore[i], false);

			SelectRow(selectedRowIndex_);
			selectedRowsCount_ = i + 1;
			needRedraw = true;

		}
	}

	if (needRedraw)
		RedrawWindow();
}

void cmTable::OnLButtonUp( UINT nFlags, CPoint pt )
{
	cmTableRow* row;
	CRect rc;
	bool needRedraw = false;

	GetClientRect(rc);
	for (unsigned int i = 0; i < rows_.size(); i++) {
		row = rows_[i];
		row->ProcessMessage(WM_LBUTTONUP, nFlags, MAKELPARAM(pt.x, pt.y), pt, rc, needRedraw);
	}

	if (draggingSplitter_)
		draggingSplitter_ = false;

	if (needRedraw)
		RedrawWindow();
}

void cmTable::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	bool needRedraw = false;
	bool ret = false;
	bool processed = false;
	cmTableRow* row;
	CRect rc;
	CPoint pt(0,0);

	if (selectedRowsCount_ || selectedColsCount_) {
		if (nChar == VK_CONTROL)
			return;

		ClearSelection();
	}

	GetClientRect(rc);
	for (unsigned int i = 0; i < rows_.size() && !processed; i++) {
		row = rows_[i];
		processed |= row->ProcessMessage(WM_KEYDOWN, nChar, nFlags, pt, rc, needRedraw);
	}

	if (!processed) {
		
		switch (nChar) {
			case VK_ESCAPE:
				KillCellFocus(false);
				needRedraw = true;
				break;

			case VK_RETURN:
				MoveSelection(0, 1, true);
				needRedraw = true;
				break;

			case VK_UP:
				MoveSelection(0, -1, true);
				needRedraw = true;
				break;

			case VK_DOWN:
				MoveSelection(0, 1, true);
				needRedraw = true;
				break;

			case VK_LEFT:
				ret = MoveSelection(-1, 0, true);
				needRedraw = true;
				break;

			case VK_RIGHT:
				ret = MoveSelection(1, 0, true);
				needRedraw = true;
				break;
		}
	}
	
	if (needRedraw)
		RedrawWindow();
}

void cmTable::SetGridColor(COLORREF color)
{
	gridColor_ = color;

	for (unsigned int i = 0; i < rows_.size(); i++)
		rows_[i]->SetGridColor(color);
}

COLORREF cmTable::GetGridColor() const
{
	return gridColor_;
}

void cmTable::SetBkColor(COLORREF color)
{
	bkColor_ = color;
}

COLORREF cmTable::GetBkColor() const
{
	return bkColor_;
}

void cmTable::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	SetFocus();
	
}

int cmTable::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	unsigned int row;
	unsigned int col;
	cmBaseCell* cell;

	cell = GetSelectedCell(row, col);
	if (!cell || !cell->HasFocus())
		SetFocus();

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

UINT cmTable::OnGetDlgCode() 
{
	return DLGC_WANTARROWS|DLGC_WANTALLKEYS|DLGC_WANTCHARS;

	
	//return CWnd::OnGetDlgCode();
}

void cmTable::OnLButtonDblClk(UINT nFlags, CPoint pt) 
{
	/*** TODO : splitter autoresize

	selSplitterIndex_ = HitSplitter(pt);
	if (selSplitterIndex_ != -1) {
		draggingSplitter_ = true;
	}
	***/

	cmTableRow* row;
	CRect rc;
	bool needRedraw = false;

	GetClientRect(rc);
	for (unsigned int i = 0; i < rows_.size(); i++) {
		row = rows_[i];
		row->ProcessMessage(WM_LBUTTONDBLCLK, nFlags, MAKELPARAM(pt.x, pt.y), pt, rc, needRedraw);
	}

	if (needRedraw)
		RedrawWindow();	
}

void cmTable::SetColumnSelectionMode(cmHeaderSelectionMode mode)
{
	columnSelectionMode_ = mode;
}

cmHeaderSelectionMode cmTable::SetColumnSelectionMode() const
{
	return columnSelectionMode_;
}

void cmTable::SetRowSelectionMode(cmHeaderSelectionMode mode)
{	
	rowSelectionMode_ = mode;
}

cmHeaderSelectionMode cmTable::SetRowSelectionMode() const
{
	return rowSelectionMode_;
}

vector<unsigned int> cmTable::GetSelColList()
{
	cmTableRow* row;
	cmBaseCell* cell;
	vector<unsigned int> ret;

	if (fixedRowsNumber_ > 0) {
		row = GetRow(0);
		if (row) {
			for (unsigned int i = 0; i < row->GetNumColumns(); i++) {
				cell = row->GetCell(i);
				if (cell->IsSelected())
					ret.push_back(i);
			}
		}
	}

	return ret;
}

vector<unsigned int> cmTable::GetSelRowList()
{
	cmTableRow* row;
	cmBaseCell* cell;
	vector<unsigned int> ret;

	if (fixedColumnsNumber_ > 0) {
		for (unsigned int i = 0; i < GetRowCount(); i++) {
			row = GetRow(i);
			cell = row->GetCell(0);
			if (cell->IsSelected())
				ret.push_back(i);
		}
	}

	return ret;
}

void cmTable::SelectColumn(unsigned int colIndex, bool redraw)
{
	cmBaseCell* cell;
	
	for (unsigned int rowIndex = 0; rowIndex < rows_.size(); rowIndex++) {
		cell = GetCell(rowIndex, colIndex);	
		cell->SetSelection(false);
	}

	if (redraw)
		RedrawWindow();
}

void cmTable::SelectRow(unsigned int rowIndex, bool redraw)
{
	cmBaseCell* cell;
	
	for (unsigned int colIndex = 0; colIndex < columnsNumber_; colIndex++) {
		cell = GetCell(rowIndex, colIndex);	
		cell->SetSelection(false);
	}

	if (redraw)
		RedrawWindow();
}

void cmTable::ClearSelection(bool redraw)
{
	cmTableRow* row;

	for (int i = 0; i < rows_.size(); i++) {
		row = rows_[i];
	
		row->ClearSelection(redraw);
	}

	selectedRowsCount_ = 0;
	selectedColsCount_ = 0;

//	if (redraw)
		RedrawWindow();
}

void cmTable::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
}
	
void cmTable::EnableBorder(bool enable)
{
	borderEnabled_ = enable;
	
	UpdateDrawArea();
}

LRESULT cmTable::OnCellSetFocus(WPARAM wParam, LPARAM lParam)
{
	if (GetParent())
		GetParent()->SendMessage(CMC_SETFOCUS, wParam, lParam);

	return 1L;
}

LRESULT cmTable::OnCellLostFocus(WPARAM wParam, LPARAM lParam)
{
	if (GetParent())
		GetParent()->SendMessage(CMC_LOSTFOCUS, wParam, lParam);

	switch (wParam) {
		case VK_DOWN:
		case VK_RETURN:
			MoveSelection(0, 1, true);
			break;

		case VK_LEFT:
			MoveSelection(-1, 0, true);
			break;

		case VK_RIGHT:
			MoveSelection(1, 0, true);
			break;

		case VK_UP:
			MoveSelection(0, -1, true);
			break;
	}

	cmBaseCellInfo* info;
	cmCellContainer* container;

	info = (cmBaseCellInfo*)lParam;
	if (info) {
		container = info->GetCellContainer();
	
		if (container)
			container->UpdateCells();
	}

	return 1L;
}

LRESULT cmTable::OnCellSetSelection(WPARAM wParam, LPARAM lParam)
{
	if (GetParent())
		GetParent()->SendMessage(CMC_SETSEL, wParam, lParam);

	return 1L;
}

LRESULT cmTable::OnCellLostSelection(WPARAM wParam, LPARAM lParam)
{
	if (GetParent())
		GetParent()->SendMessage(CMC_LOSTSEL, wParam, lParam);

	return 1L;
}


void cmTable::SetColumnsInfo(const vector<cmString>& names, const vector<cmString>& descriptions)
{
	colNames_ = names;
	colDescriptions_ = descriptions;
}