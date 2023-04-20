// cmCompositeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cmCompositeWnd.h"
#include "cmInfoArea.h"
#include "cmCellDefs.h"
#include "cmDrawUtils.h"
#include "memDC.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFO_BAR_HEIGHT 50
#define SPLITTER_HEIGHT	8

/////////////////////////////////////////////////////////////////////////////
// cmCompositeWnd

cmCompositeWnd::cmCompositeWnd():
	infoArea_(NULL),
	upperPaneWnd_(NULL),
	borderEnabled_(true),
	borderColor_(::GetSysColor(COLOR_3DSHADOW)),
	bkColor_(::GetSysColor(COLOR_3DFACE)),
	infoAreaEnabled_(false),
	splitterRect_(0, 0, 0, 0),
	splitterColor_(::GetSysColor(COLOR_3DFACE)),
	draggingSplitter_(false),
	splitterGradient_(false),
	lastRc_(0, 0, 0, 0)
{
}

cmCompositeWnd::~cmCompositeWnd()
{
	if (infoArea_)
		delete infoArea_;
}


BEGIN_MESSAGE_MAP(cmCompositeWnd, CWnd)
	//{{AFX_MSG_MAP(cmCompositeWnd)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_GETDLGCODE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_MESSAGE(CMC_SETFOCUS, OnCellSetFocus)
	ON_MESSAGE(CMC_LOSTFOCUS, OnCellLostFocus)
	ON_MESSAGE(CMC_SETSEL, OnCellSetSelection)
	ON_MESSAGE(CMC_LOSTSEL, OnCellLostSelection)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// cmCompositeWnd message handlers

BOOL cmCompositeWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int cmCompositeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->dwExStyle |= WS_EX_CONTROLPARENT;
	lpCreateStruct->style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN/* | WS_VSCROLL | WS_HSCROLL*/);

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create and initialize infoArea
	//
	infoArea_ = new cmInfoArea;
	infoArea_->Create(CRect(0, 0, 0, 0), this, 0);
	infoArea_->ModifyStyleEx(0, WS_EX_CONTROLPARENT | WS_EX_NOPARENTNOTIFY);
	
	// calculate initial splitterRect_
	//
	if (lpCreateStruct->cx == 0 || lpCreateStruct->cy == 0) {
		splitterRect_ = CRect(0, 0, 0, 0);
	} else {
		CRect rc;

		rc.left = lpCreateStruct->x;
		rc.right = lpCreateStruct->x + lpCreateStruct->cx;
		rc.top = lpCreateStruct->y;
		rc.bottom = lpCreateStruct->cy;

		splitterRect_ = GetDefaultSplitterRect(rc);
		/*
		splitterRect_.left = max(lpCreateStruct->x + borderEnabled_, 0);
		splitterRect_.right = max(lpCreateStruct->x + lpCreateStruct->cx - borderEnabled_, 0);
		splitterRect_.top = max(lpCreateStruct->y + lpCreateStruct->cy - 2 * borderEnabled_ - INFO_BAR_HEIGHT - SPLITTER_HEIGHT, 0);
		splitterRect_.bottom = max(splitterRect_.top  + SPLITTER_HEIGHT, 0);
		*/
	}

	return 0;
}

void cmCompositeWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
}

BOOL cmCompositeWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

UINT cmCompositeWnd::OnGetDlgCode() 
{
	return CWnd::OnGetDlgCode();
}

void cmCompositeWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (infoAreaEnabled_ && splitterRect_.PtInRect(point)) {
		draggingSplitter_ = true;

		SetCapture();

		lastDragPoint_ = point;
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void cmCompositeWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (draggingSplitter_) {
		draggingSplitter_ = false;
		ReleaseCapture();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void cmCompositeWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (draggingSplitter_) {
		int deltaY;
		CRect splitterRange;

		splitterRange = GetSplitterRange();

		if (point.y < splitterRange.top) {
			deltaY = splitterRange.top - lastDragPoint_.y;	
			lastDragPoint_.y = splitterRange.top;
		} else if (point.y > splitterRange.bottom) {
			deltaY = splitterRange.bottom - lastDragPoint_.y;
			lastDragPoint_.y = splitterRange.bottom;
		} else {
		deltaY = point.y - lastDragPoint_.y;
		lastDragPoint_ = point;
		}
				
		splitterRect_.OffsetRect(0, deltaY);

		UpdateWindowSize();
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void cmCompositeWnd::OnPaint() 
{
	CPaintDC dc(this); 
	CMemDC memDC(&dc);

	DrawBackground(memDC);
	DrawBorders(memDC);
	DrawSplitter(memDC);
}

void cmCompositeWnd::DrawBackground(CDC* pDC)
{
	CRect rc;
	
	GetClientRect(rc);
	
	pDC->FillSolidRect(rc, borderColor_);	
	//pDC->FillSolidRect(rc, RGB(0,255,0));	
}

void cmCompositeWnd::DrawBorders(CDC* pDC)
{

}

void cmCompositeWnd::DrawSplitter(CDC* pDC)
{
	if (!infoAreaEnabled_)
		return;

	
	

	// draw background
	//
	if (splitterGradient_) {
		COLORREF topColor;

		topColor = cmDrawUtils::MakeColorBrighter(splitterColor_, 120);
		cmDrawUtils::DrawVerticalGradient(pDC, splitterRect_, topColor, splitterColor_, 1);

		// draw grip
		//
		int left = splitterRect_.Width() / 2 - 17;
		
		for (int i = 0; i < 9; i++) {
			pDC->SetPixel(left, splitterRect_.top + 2, RGB(0x28, 0x32, 0x47));	
			pDC->SetPixel(left + 1, splitterRect_.top + 2, RGB(0x28, 0x32, 0x47));	
			
			pDC->SetPixel(left, splitterRect_.top + 3, RGB(0x28, 0x32, 0x47));	
			pDC->SetPixel(left + 1, splitterRect_.top + 3, RGB(0x61, 0x74, 0x98));	
			pDC->SetPixel(left + 2, splitterRect_.top + 3, RGB(0xf9, 0xf9, 0xfb));

			pDC->SetPixel(left + 1, splitterRect_.top + 4, RGB(0xf9, 0xf9, 0xfb));
			pDC->SetPixel(left + 2, splitterRect_.top + 4, RGB(0xf9, 0xf9, 0xfb));

			left += 5;
		}

	} else {
		CPen p(PS_SOLID, 1, borderColor_);
		CPen* oldPen;

		pDC->FillSolidRect(splitterRect_, splitterColor_);

		oldPen = pDC->SelectObject(&p);
		
		pDC->MoveTo(splitterRect_.left, splitterRect_.top + 1);
		pDC->LineTo(splitterRect_.right, splitterRect_.top + 1);

		pDC->MoveTo(splitterRect_.left, splitterRect_.bottom - 1);
		pDC->LineTo(splitterRect_.right, splitterRect_.bottom - 1);
		
		pDC->SelectObject(oldPen);

		p.DeleteObject();
	}

}

void cmCompositeWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	UpdateWindowSize();
}

cmInfoArea* cmCompositeWnd::GetInfoArea()
{
	return infoArea_;
}

void cmCompositeWnd::UpdateWindowSize()
{
	CRect rc;
	CRect upperPaneRect;
	int borderWidth;

	borderWidth = borderEnabled_ ? 1 : 0;

	GetClientRect(rc);

	rc.DeflateRect(borderWidth, borderWidth, borderWidth, borderWidth);
	
	upperPaneRect = rc;
	
	if (infoAreaEnabled_) {

		if (lastRc_.IsRectNull())
			lastRc_ = rc;
		
		
		int deltaY;
		CRect splitterRange;

		splitterRange = GetSplitterRange();

		if (splitterRange.Height() < 0) {
			// hide infoArea because there is no enough space
			//
			infoArea_->ShowWindow(SW_HIDE);
			
			splitterRect_.top = splitterRange.top;
			splitterRect_.bottom = splitterRect_.top +  SPLITTER_HEIGHT;
		} else {
			deltaY = rc.bottom - lastRc_.bottom;

			if (splitterRect_.top + deltaY < splitterRange.top) {
				splitterRect_.top = splitterRange.top;
				splitterRect_.bottom = splitterRect_.top +  SPLITTER_HEIGHT;
			} else {
				splitterRect_.OffsetRect(0,  deltaY);
			}
			
			infoArea_->SetWindowPos(&CWnd::wndTop, rc.left, splitterRect_.bottom, rc.Width(), rc.bottom - splitterRect_.bottom, SWP_SHOWWINDOW);

			upperPaneRect.bottom = splitterRect_.top;
		}

		splitterRect_.left = rc.left;
		splitterRect_.right = rc.right;
		
		lastRc_ = rc;		

	} else {
		
		infoArea_->ShowWindow(SW_HIDE);
	}
	
	upperPaneWnd_->SetWindowPos(&CWnd::wndTop, 
								upperPaneRect.left, 
								upperPaneRect.top, 
								upperPaneRect.Width(), 
								upperPaneRect.Height(), 
								SWP_SHOWWINDOW);

	RedrawWindow();
}


LRESULT cmCompositeWnd::OnCellSetFocus(WPARAM wParam, LPARAM lParam)
{
//	if (infoArea_)
//		infoArea_->SetCellInfo((cmBaseCellInfo*)lParam);

	return 1L;
}

LRESULT cmCompositeWnd::OnCellLostFocus(WPARAM wParam, LPARAM lParam)
{
	if (infoArea_)
		infoArea_->SetCellInfo((cmBaseCellInfo*)lParam);

	return 1L;
}

LRESULT cmCompositeWnd::OnCellSetSelection(WPARAM wParam, LPARAM lParam)
{
	static int i = 0;

	TRACE(_T("[%03d] OnCellSel\n"), i++);
	if (infoArea_)
		infoArea_->SetCellInfo((cmBaseCellInfo*)lParam);

	return 1L;
}

LRESULT cmCompositeWnd::OnCellLostSelection(WPARAM wParam, LPARAM lParam)
{

	return 1L;
}

void cmCompositeWnd::SetBorderColor(COLORREF color, bool redraw)
{
	borderColor_ = color;

	if (redraw)
		RedrawWindow();
}

COLORREF cmCompositeWnd::GetBorderColor() const
{
	return borderColor_;
}

void cmCompositeWnd::EnableBorder(bool enable, bool redraw)
{
	borderEnabled_ = enable;

	UpdateWindowSize();

	if (redraw)
		RedrawWindow();
}

void cmCompositeWnd::ShowInfoArea(bool enable, bool redraw)
{
	if (enable == infoAreaEnabled_)
		return;

	if (enable)
		UpdateSplitterRect();

	infoAreaEnabled_ = enable;

	UpdateWindowSize();

	if (redraw)
		RedrawWindow();
}

void cmCompositeWnd::SetSplitterColor(COLORREF color, bool enableGradient, bool redraw)
{
	splitterColor_ = color;
	splitterGradient_ = enableGradient;

	if (redraw)
		RedrawWindow();
}

COLORREF cmCompositeWnd::GetSplitterColor() const
{
	return splitterColor_;
}

BOOL cmCompositeWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT) {
		CPoint pt;
		
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		if (splitterRect_.PtInRect(pt) && infoAreaEnabled_) {
			::SetCursor(AfxGetApp()->LoadCursor(IDC_SPLITTER));
			return TRUE;
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

CRect cmCompositeWnd::GetDefaultSplitterRect(const CRect& rc)
{
	CRect ret;

	ret.left = rc.left + borderEnabled_;
	ret.right = rc.right - borderEnabled_;
	ret.top = (double)rc.Height() * .7;
	ret.bottom = ret.top + SPLITTER_HEIGHT;

	return ret;
}


CRect cmCompositeWnd::GetSplitterRange()
{
	CRect rc;

	GetClientRect(rc);

	rc.DeflateRect(0, 35, 0, 35);

	return rc;
}

void cmCompositeWnd::UpdateSplitterRect()
{
	if (splitterRect_.IsRectNull()) {
		// set default position (30% of the whole height)
		//
		CRect rc;

		GetClientRect(rc);

		splitterRect_ = GetDefaultSplitterRect(rc);
	}

	CRect splitterRange;

	splitterRange = GetSplitterRange();

	if (splitterRect_.top < splitterRange.top) {
		splitterRect_.top = splitterRange.top;
		splitterRect_.bottom = splitterRect_.top + SPLITTER_HEIGHT;
	} else if (splitterRect_.bottom > splitterRange.bottom) {
		splitterRect_.bottom = splitterRange.bottom;
		splitterRect_.top = splitterRect_.bottom - SPLITTER_HEIGHT;
	}
}