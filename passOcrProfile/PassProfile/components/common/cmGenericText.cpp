// cmGenericText.cpp : implementation file
//

#include "stdafx.h"
#include "cmGenericText.h"
#include "cmBaseCell.h"
#include "cmCellDefs.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cmGenericText

cmGenericText::cmGenericText():
	parentCell_(NULL),
	lastChar_(0),
	cursorLocked_(false)
{
}

cmGenericText::~cmGenericText()
{
}

BEGIN_MESSAGE_MAP(cmGenericText, CEdit)
	//{{AFX_MSG_MAP(cmGenericText)
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cmGenericText message handlers

UINT cmGenericText::OnGetDlgCode() 
{
	return DLGC_WANTARROWS | DLGC_WANTALLKEYS | DLGC_WANTCHARS;
}

void cmGenericText::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	bool processed = false;

	lastChar_ = nChar;

	switch (nChar) {
		case VK_ESCAPE:
			
			assert(parentCell_);
			if (parentCell_)
				parentCell_->KillFocus(false);
			processed = true;
			break;

		case VK_LEFT:
			if (cursorLocked_)
				break;

		case VK_RIGHT:
			if (cursorLocked_)
				break;

		case VK_RETURN:
		case VK_UP:
		case VK_DOWN:
			assert(parentCell_);
			if (parentCell_)
				parentCell_->KillFocus(true);
			processed = true;
			break;
	}

	if (!processed)
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void cmGenericText::SetParentCell(cmBaseCell* parent)
{
	parentCell_ = parent;
}

cmBaseCell* cmGenericText::GetParentCell() const
{
	return parentCell_;
}

UINT cmGenericText::GetLastChar() const
{
	return lastChar_;
}

int cmGenericText::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	cursorLocked_ = false;
	
	return 0;
}

void cmGenericText::LockCursor()
{
	cursorLocked_ = true;
}

void cmGenericText::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	
	//CEdit::OnLButtonDblClk(nFlags, point);
}

void cmGenericText::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	parentCell_->KillFocus(true, true);
}

void cmGenericText::OnLButtonDown(UINT nFlags, CPoint pt)
{
	/***
	CRect rc;

	GetClientRect(rc);

	if (!rc.PtInRect(pt)) {
		ClientToScreen(&pt);
		GetParent()->ScreenToClient(&pt);
		GetParent()->PostMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(pt.x, pt.y));
		parentCell_->KillFocus(true, true);
	} else {
		CEdit::OnLButtonDown(nFlags, pt);
	}
	***/

	CEdit::OnLButtonDown(nFlags, pt);
}