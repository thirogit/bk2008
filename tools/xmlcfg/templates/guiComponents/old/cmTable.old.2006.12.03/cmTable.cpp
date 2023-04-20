// AsTable.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "MemDC.h"
#include "DrawUtils.h"
#include "cmTable.h"
#include "cmTableRow.h"
#include "cmBaseCell.h"
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
	borderColor_(::GetSysColor(COLOR_3DSHADOW)),
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
	draggingSplitter_(false)
{
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    //HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, _T("AsTable"), &wndcls)))
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
        wndcls.lpszClassName    = _T("AsTable");

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// cmTable message handlers

BOOL cmTable::Create(const CRect& rc, CWnd* parentWnd, DWORD style, UINT nID)
{
	BOOL ret;

	style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | /*WS_VSCROLL | WS_HSCROLL |*/ WS_CHILD | WS_VISIBLE;
	
	ret = CWnd::Create(_T("AsTable"), _T("Algostar Table"), style, rc, parentWnd, nID, NULL);


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
	
	DrawBackground(&memDC);
	DrawBorders(&memDC);
	DrawRows(&memDC);
	DrawSelectedCell(&memDC);
}

void cmTable::DrawBackground(CDC* pDC)
{
	CBrush b(bkColor_);
	CRect rc;

	GetClientRect(rc);
	pDC->FillRect(rc, &b);
}

void cmTable::DrawBorders(CDC* pDC)
{
	// main border
	//
	CRect rc;
	CRect tmp;
	GetClientRect(rc);	
	unsigned int i;

	rc.DeflateRect(0, 0, 1, 1);
	DrawUtils::DrawRect(pDC, rc, borderColor_);

	for (i = 0; i < fixedRowsNumber_; i++) {
		tmp = rows_[i]->GetDrawArea();
		tmp.InflateRect(1, 1, 0, 0);
		
		DrawUtils::DrawRect(pDC, tmp, borderColor_);
	}

	for (i = firstNonFixedRowIndex_; i < rows_.size(); i++) {
		tmp = rows_[i]->GetDrawArea();
		tmp.InflateRect(1, 1, 0, 0);
		if (!rc.PtInRect(tmp.TopLeft()))
			break;
		DrawUtils::DrawRect(pDC, tmp, borderColor_);
	}
}

void cmTable::DrawRows(CDC* pDC)
{
	unsigned int i;
	CRect rc;
	cmTableRow* row;
	int lastBottom;
	CRect drawArea;
	GetClientRect(rc);

	for (i = 0; i < fixedRowsNumber_; i++) {
		row = rows_[i];
		if (row)
			row->Draw(pDC);
	}
	lastBottom = 0;
	for (i = firstNonFixedRowIndex_; i < rows_.size(); i++) {
		row = rows_[i];
		if (row) {
			drawArea = row->GetDrawArea();
			if (!rc.PtInRect(drawArea.TopLeft()) || drawArea.top < lastBottom)
				break;
			//TRACE1("drawing row %s\n", row->GetCell(0)->GetText().c_str());
			row->Draw(pDC);
		}
		lastBottom = drawArea.bottom + BORDER;
	}
}

BOOL cmTable::PreTranslateMessage(MSG* pMsg) 
{
	CPoint pt;
	CRect rc;
	bool ret;
	bool needRedraw = false;
	bool processed = false;

	pt = pMsg->pt;
	ScreenToClient(&pt);
	GetClientRect(rc);

	// first of all check if we are dragging the splitter
	//
	switch (pMsg->message) {
		case WM_LBUTTONDOWN:
			selSplitterIndex_ = HitSplitter(pt);
			if (selSplitterIndex_ != -1) {
				draggingSplitter_ = true;
				return TRUE;
			}
			break;

		case WM_MOUSEMOVE:
			if (draggingSplitter_ && (pMsg->wParam & MK_LBUTTON)) {
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
				TRACE3("splitterIndex = %d, width=%d, x=%d\n", selSplitterIndex_, cellWidth, point.x);
				
				RedrawWindow();
			
				return TRUE;
			}
			
		case WM_LBUTTONUP:
			if (draggingSplitter_) {
				draggingSplitter_ = false;
				selSplitterIndex_ = -1;
				splitterRowIndex_ = - 1;
				return TRUE;
			}
			break;
	}
	
	
	
	cmTableRow* row;

	for (unsigned int i = 0; i < rows_.size(); i++) {
		row = rows_[i];
		processed |= row->ProcessMessage(pMsg->message, pMsg->lParam, pMsg->wParam, pt, rc, needRedraw);
	}

	if(processed)
		return TRUE;

	if (pMsg->message == WM_KEYDOWN && !processed) { 
		switch (pMsg->wParam) {
			case VK_ESCAPE:
				KillCellFocus(false);
				RedrawWindow();
				return TRUE;

			case VK_RETURN:
				KillCellFocus(true);
				MoveSelection(0, 1, true);
				return TRUE;

			case VK_UP:
				MoveSelection(0, -1, true);
				return TRUE;
				break;

			case VK_DOWN:
				MoveSelection(0, 1, true);
				return TRUE;
				break;

			case VK_LEFT:
				ret = MoveSelection(-1, 0, true);
				if (ret)
					return TRUE;
				
				needRedraw = true;
				break;

			case VK_RIGHT:
				ret = MoveSelection(1, 0, true);
				if (ret)
					return TRUE;
				
				needRedraw = true;
				break;

			case VK_F2:
				SetCellFocus(false);
				needRedraw = true;
				break;

		}
	}

	if (needRedraw)
		RedrawWindow();

	return CWnd::PreTranslateMessage(pMsg);
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

	SetScrollInfo(SB_VERT, &si, TRUE);
	
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

cmTableRow* cmTable::AppendRow(unsigned int numColumns, bool redraw)
{
	return InsertRow(rows_.size(), numColumns, redraw);
}

cmTableRow* cmTable::InsertRow(unsigned int index, unsigned int numColumns, bool redraw)
{
	cmTableRow* newRow;

	newRow = new cmTableRow;
	newRow->SetNumColumns(numColumns);
	newRow->SetColumnWidthList(colWidth_);
	newRow->SetParentWnd(this);
	newRow->SetFixedColumnsNumber(fixedColumnsNumber_);
	if (index < 0)
		index = 0;
	else if (index >= rows_.size())
		index = rows_.size();

	rows_.insert(rows_.begin() + index, newRow);

	UpdateWindowSize(redraw);

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

unsigned int cmTable::GetRowCount() const
{
	return rows_.size();
}

void cmTable::DeleteAll()
{
	Clear();

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

	//rc.InflateRect(horizontalOffset_, verticalOffset_, 0, 0);
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
	while ((totalHeight < rc.Height()) && (i < rows_.size())) {
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

	for (; i < rows_.size(); i++)
		rows_[i]->SetDrawArea(CRect(0,0,0,0), false);
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

void cmTable::UpdateFirstNonFixedRowIndex()
{
	CRect rc;
	CRect fixedRowsArea;
	int nonFixedAreaHeight;
	int tmpHeight;
	unsigned int i;
	int old;
	
	old = firstNonFixedRowIndex_;
	GetClientRect(rc);
	rc.DeflateRect(0, BORDER, 0, BORDER);
	fixedRowsArea = GetFixedRowsArea();
	
	nonFixedAreaHeight = rc.Height() - fixedRowsArea.Height();
	
	tmpHeight = 0;
	for (i = firstNonFixedRowIndex_; i < rows_.size(); i++) {
		tmpHeight += rows_[i]->GetDrawArea().Height();
		tmpHeight += BORDER;
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


int cmTable::HitSplitter(const CPoint& point) 
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
	bool itemFound;
	unsigned int selRowIndex;
	unsigned int selCellIndex;
	unsigned int newRowIndex;
	unsigned int newCellIndex;

	itemFound = GetSelectedItem(&selRow, &selCell, selRowIndex, selCellIndex);

	if (!itemFound)
		return false;

	newRowIndex = selRowIndex + verticalShift;
	newCellIndex = selCellIndex + horizontalShift;

	if (newRowIndex < fixedRowsNumber_ || newRowIndex >= rows_.size())
		return false;

	if (newCellIndex < fixedColumnsNumber_ || newCellIndex >= columnsNumber_)
		return false;

	if (selCell->HasFocus() && horizontalShift != 0)
		return false;

	// kill selection on active cell
	//
	selCell->KillSelection(false);
	
	// get new cell and setfocus
	//
	selRow = rows_[newRowIndex];
	selCell = selRow->GetCell(newCellIndex);
	selCell->SetSelection(false);

	EnsureVisible(newRowIndex, newCellIndex, false);
	
	if (redraw)
		RedrawWindow();

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

bool cmTable::GetSelectedItem(cmTableRow** row, cmBaseCell** cell, unsigned int& rowIndex, unsigned int& cellIndex)
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

void cmTable::SetCellFocus(bool redraw)
{
	cmTableRow* selRow;
	cmBaseCell* selCell;
	unsigned int rowIndex;
	unsigned int cellIndex;
	bool itemFound;

	itemFound = GetSelectedItem(&selRow, &selCell, rowIndex, cellIndex);
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
		
		//static int a = 0;
		//TRACE1("%04d ********************** selezione\n",a++);
	} //else {
		//static int a = 0;
		//TRACE1("%04d nessuna selezione\n",a++);
	//}
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