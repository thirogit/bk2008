// AsOptionList.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "MemDC.h"
#include "ICmOptionListListener.h"
#include "cmOptionList.h"
#include "cmOptionGroup.h"


/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

#define BORDER 1
#define LINE_HEIGHT 20

#if !defined(IDC_HAND)
	#define IDC_HAND MAKEINTRESOURCE(32649)
#endif

/////////////////////////////////////////////////////////////////////////////
// cmOptionList

cmOptionList::cmOptionList():
	borderColor_(::GetSysColor(COLOR_3DSHADOW)),
	bkColor_(::GetSysColor(COLOR_WINDOW)),
	verticalSplitBkColor_(::GetSysColor(COLOR_BTNFACE)),
	verticalStripWidth_(20),
	verticalSplitterPos_(200),
	offset_(0),
	listener_(NULL),
	sorted_(true)
{
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    //HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, _T("cmOptionList"), &wndcls)))
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
        wndcls.lpszClassName    = _T("cmOptionList");

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return;
        }
    }

}

cmOptionList::~cmOptionList()
{
	OptionGroupMap::iterator it;

	for (it = groups_.begin(); it != groups_.end(); it++)
		delete it->second;
}

BEGIN_MESSAGE_MAP(cmOptionList, CWnd)
	//{{AFX_MSG_MAP(cmOptionList)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_SIZING()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cmOptionList message handlers

void cmOptionList::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd())
		UpdateWindowSize();
	
}

BOOL cmOptionList::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void cmOptionList::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(&dc);

	DrawBackground(memDC);	
	DrawGroups(memDC);
	DrawBorder(memDC);

	/*cmBaseCell* focusedCell;

	focusedCell = GetFocusedCell();
	if (focusedCell)
		focusedCell->Draw(memDC);*/
}

void cmOptionList::DrawBackground(CDC* pDC)
{
	CRect rc;
	CBrush b1(bkColor_);
	CBrush b2(verticalSplitBkColor_);
	
	// Fill background
	//
	GetClientRect(rc);
	rc.DeflateRect(verticalStripWidth_, 0, 0, 0);
	pDC->FillRect(rc, &b1);

	//GetClientRect(rc);
	//rc.right = rc.left + verticalStripWidth_;
	//pDC->FillRect(rc, &b2);
}

void cmOptionList::DrawBorder(CDC* pDC)
{
	// draw outer rect
	//
	CPen pen(PS_SOLID, 1, borderColor_);
	CPen* oldPen;
	OptionGroupMap::iterator it;
	CRect rc;
	CRect drawArea;

	GetClientRect(rc);
	oldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(rc.TopLeft());
	pDC->LineTo(rc.right - 1, rc.top);
	pDC->LineTo(rc.right - 1, rc.bottom - 1);
	pDC->LineTo(rc.left, rc.bottom - 1);
	pDC->LineTo(rc.TopLeft());

	for (it = groups_.begin(); it != groups_.end(); it++) {
		drawArea = it->second->GetDrawArea();
		pDC->MoveTo(0, drawArea.top - 1);
		pDC->LineTo(drawArea.right + 1,  drawArea.top - 1);
	}

	// draw last bottom line
	//
	pDC->MoveTo(0, drawArea.bottom);
	pDC->LineTo(drawArea.right + 1,  drawArea.bottom);

	pDC->SelectObject(oldPen);
}

void cmOptionList::UpdateWindowSize()
{
	long totalHeight;
	
	totalHeight = UpdateDrawArea();
		
	// update scrollbar
	// 
	SCROLLINFO si;
	int nHeight;
	long lHeight = 0;
	CRect rc;
		
	// Get client rectangle
	GetClientRect(rc);
	nHeight = rc.Height() + 1;
	lHeight = totalHeight;
	
	// Setup scroll info
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = (int) lHeight;
	si.nPage = nHeight;

	// Set scroll info
	if ((int)si.nPage > si.nMax)
	{
		offset_ = 0;
	}
	SetScrollInfo(SB_VERT, &si, TRUE);

	//Invalidate();
	// -- Redraw
	Invalidate();

	// -- Update window
	UpdateWindow();
}

BOOL cmOptionList::Create(const CRect& rc, CWnd* parentWnd, DWORD style, UINT nID)
{
	BOOL ret;

	style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL | WS_CHILD | WS_VISIBLE;
	
	ret = CWnd::Create(_T("cmOptionList"), _T("CodeMachines Option List"), style, rc, parentWnd, nID, NULL);

	if (ret)
		ModifyStyleEx(0, WS_EX_CONTROLPARENT/* | WS_EX_NOPARENTNOTIFY*/);

	return ret;
}

void cmOptionList::SetBorderColor(COLORREF borderColor, bool redraw)
{
	borderColor_ = borderColor;

	OptionGroupMap::iterator it;

	for (it = groups_.begin(); it != groups_.end(); it++)
		it->second->SetBorderColor(borderColor);

	if (redraw)
		RedrawWindow();
}

COLORREF cmOptionList::GetBorderColor() const
{
	return borderColor_;
}

void cmOptionList::SetBkColor(COLORREF bkColor, bool redraw)
{
	bkColor_ = bkColor;
	if (redraw)
		RedrawWindow();
}

COLORREF cmOptionList::GetBkColor() const
{
	return bkColor_;
}


cmOptionGroup* cmOptionList::AddGroup(const cmString& name)
{
	assert(GetGroup(name) == NULL);

	cmOptionGroup* group;
	cmString sortKey;

	group = new cmOptionGroup;
	group->SetVerticalSplitterPos(verticalSplitterPos_);
	group->RegisterListener(this);
	group->SetParentWindow(this);
	group->SetFontInfo(fontInfo_);
	group->SetCaption(name);
	
	if (!sorted_)
		sortKey.Format(_T("row_index_%08d"), groups_.size());
	else
		sortKey = name;
	
	groups_[sortKey] = group;

	UpdateWindowSize();

	return group;
}

cmOptionGroup* cmOptionList::GetGroup(const cmString& name)
{
	OptionGroupMap::iterator it;
	cmOptionGroup* group = NULL;

	it = groups_.find(name);
	if (it != groups_.end())
		group = it->second;

	return group;
}

void cmOptionList::DrawGroups(CDC* pDC)
{
	OptionGroupMap::iterator it;

	for (it = groups_.begin(); it != groups_.end(); it++)
		it->second->Draw(pDC);

}

int cmOptionList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	

	UpdateWindowSize();
	
	return 0;
}

void cmOptionList::SetVerticalStripWidth(int width)
{
	verticalStripWidth_ = width;

	OptionGroupMap::iterator it;

	for (it = groups_.begin(); it != groups_.end(); it++)
		it->second->SetVerticalStripWidth(width);
}

int cmOptionList::GetVerticalStripWidth() const
{
	return verticalStripWidth_;
}

void cmOptionList::SetVerticalSplitterPos(int pos)
{
	verticalSplitterPos_ = pos;

	OptionGroupMap::iterator it;

	for (it = groups_.begin(); it != groups_.end(); it++)
		it->second->SetVerticalSplitterPos(pos);
}

int cmOptionList::GetVerticalSplitterPos() const
{
	return verticalSplitterPos_;
}

BOOL cmOptionList::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	OptionGroupMap::iterator it;
	cmOptionGroup::enCmOptionGroupHitTest hitTest;
	CPoint pt;
	
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	for (it = groups_.begin(); it != groups_.end(); it++) {
		hitTest = it->second->HitTest(pt);

		if (hitTest == cmOptionGroup::enCmOptionGroupHitTest_Expand) {
			::SetCursor(::LoadCursor(NULL, IDC_HAND));
			
			return TRUE;
		}

		if (hitTest == cmOptionGroup::enCmOptionGroupHitTest_Splitter)  {
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			
			return TRUE;
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void cmOptionList::OnSizing(UINT fwSide, LPRECT pRect) 
{
	UpdateWindowSize();
	
	CWnd::OnSizing(fwSide, pRect);
}

void cmOptionList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Declare variables
	SCROLLINFO si;
	CRect rcClient;
	long lHeight, lNY;

	// Setup scrollbar information
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE;

	// Get client rectangle
	GetClientRect(rcClient);
	
	// Set focus
	//SetFocus();

	// Get height
	lHeight = rcClient.Height() + 1;

	// Get scrollbar information
	GetScrollInfo(SB_VERT, &si);

	// Get origin
	lNY = offset_;

	// Switch scrollbar code
	switch (nSBCode)
	{
		case SB_LINEDOWN:
			lNY += LINE_HEIGHT;
			break;

		case SB_LINEUP:
			lNY -= LINE_HEIGHT;
			break;

		case SB_PAGEDOWN:
			lNY += lHeight;
			break;

		case SB_PAGEUP:
			lNY -= lHeight;
			break;

		case SB_THUMBTRACK:
			lNY = nPos;
			break;
	}

	// Calculate
	lNY = __min(__max(lNY, si.nMin), si.nMax - lHeight);

	// Set origin
	offset_ = lNY;

	// Set scrollbar info
	si.fMask = SIF_POS;
	si.nPos = lNY;
	SetScrollInfo(SB_VERT, &si, TRUE);

	UpdateDrawArea();

	// Force to redraw
	Invalidate();

	// Update window
	UpdateWindow();



	
	// OLD **** CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void cmOptionList::OnGroupSizeChanged(bool redraw)
{
	UpdateWindowSize();
}

CRect cmOptionList::GetGlobalDrawArea()
{
	OptionGroupMap::iterator it;
	cmOptionGroup* group;
	CRect globalDrawArea;
	CRect drawArea;
	CRect rc;
	
	GetClientRect(rc);
	globalDrawArea.left = rc.left;
	globalDrawArea.right = rc.right;
	globalDrawArea.top = 0;
	globalDrawArea.bottom = 0;
	for (it = groups_.begin(); it != groups_.end(); it++) {
		group = it->second;

		drawArea = group->GetDrawArea();
		globalDrawArea.top = min(globalDrawArea.top, drawArea.top);
		globalDrawArea.bottom = max(globalDrawArea.bottom, drawArea.bottom);
	}

	return globalDrawArea;
}

long cmOptionList::UpdateDrawArea()
{
	CRect rc;
	CRect drawArea;
	CRect tmp;
	OptionGroupMap::iterator it;
	cmOptionGroup* group;
	long totalHeight;

	GetClientRect(rc);

	rc.InflateRect(0, offset_, 0, 0);
	rc.DeflateRect(BORDER, BORDER, BORDER, BORDER);	// remove borders 
	drawArea = rc;
	
	totalHeight = 0;
	for (it = groups_.begin(); it != groups_.end(); it++) {
		// Note: each group limits drawArea to its min size
		//
		group = it->second;		
		group->SetDrawArea(drawArea);
		group->SetVerticalSplitterPos(verticalSplitterPos_);

		drawArea = group->GetDrawArea();

		totalHeight += drawArea.Height() + BORDER;
		tmp = rc;
		tmp.top = drawArea.bottom + BORDER;

		drawArea = tmp;
	}

	return totalHeight;
}
BOOL cmOptionList::PreTranslateMessage(MSG* pMsg) 
{
	//bool processed;
	cmOptionGroup* group;
	OptionGroupMap::iterator it;
	CPoint pt;
	CRect rc;
	bool needRedraw = false;
	
	pt = pMsg->pt;
	ScreenToClient(&pt);
	GetClientRect(rc);

	// first of all check if we are dragging the splitter
	//
	/*switch (pMsg->message) {
		case WM_LBUTTONDOWN:
			if (HitSplitter(pt)) {
				draggingSplitter_ = true;
				return TRUE;
			}
			break;

		case WM_MOUSEMOVE:
			if (draggingSplitter_ && (pMsg->wParam & MK_LBUTTON)) {
				CRect rc;
				int minPos;
				int maxPos;
				CPoint point(pt);
				GetClientRect(rc);
				
				minPos = (int)(verticalStripWidth_ + (rc.Width() - verticalStripWidth_) * .1);
				maxPos = (int)(verticalStripWidth_ + (rc.Width() - verticalStripWidth_) * .9);
				point.x = min(point.x, maxPos);
				point.x = max(point.x, minPos);

				verticalSplitterPos_ = point.x;
				UpdateDrawArea();
				RedrawWindow();
			
				return TRUE;
			}
			
		case WM_LBUTTONUP:
			if (draggingSplitter_) {
				draggingSplitter_ = false;
				return TRUE;
			}
			break;



	}*/


	/***
	for (it = groups_.begin(); it != groups_.end(); it++) {
		group = it->second;
		processed = group->ProcessMessage(pMsg->message, pMsg->lParam, pMsg->wParam, pt, rc, needRedraw);

		if (processed)
			return TRUE;
	}
	***/

	switch (pMsg->message) {
		case WM_KEYDOWN:
			if (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN) {
				bool moveUp;
				bool keepTopSelection;
				bool keepBottomSelection;
				bool selectionLost;
				int groupIndex;
				cmBaseCell* selCell;

				moveUp = pMsg->wParam == VK_UP;
				groupIndex = 0;
				for (it = groups_.begin(); it != groups_.end(); it++) {
					keepTopSelection = (groupIndex == 0);
					keepBottomSelection = (groupIndex == groups_.size() - 1);
					group = it->second;
					selCell = group->MoveSelection(moveUp, keepTopSelection, keepBottomSelection, selectionLost, false);
					if (selectionLost) {
						if (moveUp) {
							it--;
							group = it->second;
							group->SelectBottomRow(false);
							break;
						} else {
							it++;
							group = it->second;
							group->SelectHeader(true, false);
							selCell = group->GetHeaderCell();
						}
					}
					if (selCell)
						EnsureCellVisible(selCell);
						
					groupIndex++;
				}

				
				RedrawWindow();
				return TRUE;
			}
	}

	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
		return TRUE;

	if (needRedraw)
		RedrawWindow();

	return CWnd::PreTranslateMessage(pMsg);
}

bool cmOptionList::HitSplitter(const CPoint& point) 
{
	OptionGroupMap::iterator it;
	cmOptionGroup::enCmOptionGroupHitTest hitTest;
	
	for (it = groups_.begin(); it != groups_.end(); it++) {
		hitTest = it->second->HitTest(point);

		if (hitTest == cmOptionGroup::enCmOptionGroupHitTest_Splitter)  {
			return true;
		}
	}

	return false;
}


BOOL cmOptionList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
		
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void cmOptionList::SetFontInfo(const cmString& fontName, int fontSize, bool fontBold)
{
	cmFontInfo tmp;

	tmp.SetFontName(fontName);
	tmp.SetFontSize(fontSize);
	tmp.SetFontBold(fontBold);

	SetFontInfo(tmp);
}

void cmOptionList::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;

	OptionGroupMap::iterator it;
	
	for (it = groups_.begin(); it != groups_.end(); it++)
		it->second->SetFontInfo(fontInfo);
}

void cmOptionList::EnsureCellVisible(cmBaseCell* cell)
{
	CRect rc;
	CRect drawArea;
	CRect globalDrawArea;

	GetClientRect(rc);
	
	drawArea = cell->GetDrawArea();
	globalDrawArea = GetGlobalDrawArea();

	if (rc.top > drawArea.top) {
		// selCell is over rect rc (visible area)
		//
		offset_ = drawArea.top - globalDrawArea.top;
		if (offset_ < 0)
			offset_ = 0;
		UpdateDrawArea();
		SetScrollPos(SB_VERT, offset_);
	} else if (rc.bottom < drawArea.bottom) {
		offset_ = drawArea.bottom - rc.Height() -globalDrawArea.top;
		UpdateDrawArea();
		SetScrollPos(SB_VERT, offset_);
	}
}

cmBaseCell* cmOptionList::GetFocusedCell()
{
	OptionGroupMap::iterator it;
	cmBaseCell* focusedCell = NULL;
	cmOptionGroup* group;

	for (it = groups_.begin(); it != groups_.end(); it++) {
		group = it->second;
		focusedCell = group->GetFocusedCell();
		if (focusedCell != NULL)
			break;
	}

	return focusedCell;
}

void cmOptionList::GetSelectedItemInfo(cmString& caption, 
									   cmString& description,
									   cmString& tooltipText,
									   UINT& tooltipIcon)
{
	OptionGroupMap::iterator it;
	cmOptionGroup* group;

	caption = _T("");
	description = _T("");
	tooltipText = _T("");
	tooltipIcon = 0;

	for (it = groups_.begin(); it != groups_.end(); it++) {
		group = it->second;

		if (group->IsSelected()) {
			group->GetSelectionInfo(caption, description, tooltipText, tooltipIcon);
			break;
		}
	}
}

void cmOptionList::OnSelectionInfoChanged(cmOptionGroup* selectedGroup, bool redraw)
{
	cmString caption;
	cmString description;
	cmString tooltipText;
	UINT tooltipIcon;

	selectedGroup->GetSelectionInfo(caption, description, tooltipText, tooltipIcon);
	if (listener_)
		listener_->OnInfoUpdate(caption, description, tooltipText, tooltipIcon);
}

void cmOptionList::RegisterListener(ICmOptionListListener* listener)
{	
	listener_ = listener;
}


HBRUSH cmOptionList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	//if (nCtlColor == CTLCOLOR_LISTBOX)
	//	MessageBox("XXX");	
	
	return hbr;
}

void cmOptionList::SetSorted(bool sorted)
{
	sorted_ = sorted;
}

bool cmOptionList::GetSorted() const
{
	return sorted_;
}

cmOptionRow* cmOptionList::GetSelectedRow()
{
	OptionGroupMap::iterator it;
	cmOptionRow* ret = NULL;
	cmOptionGroup* g;

	for (it = groups_.begin(); it != groups_.end(); it++) {
		g = it->second;

		ret = g->GetSelectedRow();
		if (ret)
			break;
	}

	return ret;
}

void cmOptionList::OnMouseMove( UINT nFlags, CPoint pt)
{
	if (draggingSplitter_ && (nFlags & MK_LBUTTON)) {
		CRect rc;
		int minPos;
		int maxPos;
		CPoint point(pt);
		GetClientRect(rc);
		
		minPos = (int)(verticalStripWidth_ + (rc.Width() - verticalStripWidth_) * .1);
		maxPos = (int)(verticalStripWidth_ + (rc.Width() - verticalStripWidth_) * .9);
		point.x = min(point.x, maxPos);
		point.x = max(point.x, minPos);

		verticalSplitterPos_ = point.x;
		UpdateDrawArea();
		RedrawWindow();
	}

	bool needRedraw;
	CRect rc;

	GetClientRect(rc);
	ForwardMsgToGroups(WM_MOUSEMOVE, MAKELPARAM(pt.x, pt.y), nFlags, pt, rc, needRedraw);
	if (needRedraw)
		RedrawWindow();
}

void cmOptionList::OnLButtonDown( UINT nFlags, CPoint pt)
{
	if (HitSplitter(pt))
		draggingSplitter_ = true;

	bool needRedraw;
	CRect rc;

	GetClientRect(rc);
	ForwardMsgToGroups(WM_LBUTTONDOWN, MAKELPARAM(pt.x, pt.y), nFlags, pt, rc, needRedraw);
	if (needRedraw)
		RedrawWindow();
}
 
void cmOptionList::OnLButtonUp( UINT nFlags, CPoint pt )
{
	if (draggingSplitter_)
		draggingSplitter_ = false;

	bool needRedraw;
	CRect rc;

	GetClientRect(rc);
	ForwardMsgToGroups(WM_LBUTTONUP, MAKELPARAM(pt.x, pt.y), nFlags, pt, rc, needRedraw);
	if (needRedraw)
		RedrawWindow();
}

void cmOptionList::ForwardMsgToGroups(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;
	cmOptionGroup* group;
	OptionGroupMap::iterator it;
	CRect rc;
	
	needRedraw = false;

	for (it = groups_.begin(); it != groups_.end(); it++) {
		group = it->second;
		processed = group->ProcessMessage(msg, lParam, wParam, pt, rc, needRedraw);

		if (processed)
			return;
	}
}