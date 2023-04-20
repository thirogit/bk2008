
#include "stdafx.h"
#include "cmGlassButton.h"
#include "cmDrawUtils.h"
#include "MemDC.h"

cmGlassButton::cmGlassButton(): 
	baseColor_(RGB(0x7E, 0xA1, 0xC4)),
	captionColor_(0)
{
	renderer_.SetBorderEnable(false);
	renderer_.GetFontInfo().SetFontName(_T("Tahoma"));
	renderer_.GetFontInfo().SetFontBold(false);
	renderer_.GetFontInfo().SetFontSize(-14);
	renderer_.SetTextFlags(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	renderer_.EnableTextShadow(false);
	renderer_.SetTextColor(captionColor_);
}

cmGlassButton::~cmGlassButton()
{
}


BEGIN_MESSAGE_MAP(cmGlassButton,CButton)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


void cmGlassButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!GetSafeHwnd())
		return;

	SetStatus(enButtonStatus_Down);
	//SetCapture();
	
	CButton::OnLButtonDown(nFlags, point);
}


void cmGlassButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!GetSafeHwnd())
		return;

	CRect rc;

	SetStatus(enButtonStatus_Normal);
	//ReleaseCapture();
	Invalidate();
	CButton::OnLButtonUp(nFlags, point);

	//GetClientRect(rc);
	
	//if (m_PrevStatus == DOWN && ButtonRect.PtInRect(point))
	//	GetParent()->SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),0),(LPARAM) m_hWnd);
}

void cmGlassButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (!GetSafeHwnd())
		return;

	GetParent()->SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),0),(LPARAM) m_hWnd);
}

void cmGlassButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!GetSafeHwnd())
		return;

	CRect rc;
	TRACKMOUSEEVENT tme;
	
	GetClientRect(rc);

	//sprintf(logs,"Button Rect(%d,%d,%d,%d), Mouse Point= (%d,%d)",ButtonRect.left,ButtonRect.right,
	//		ButtonRect.top, ButtonRect.bottom, point.x, point.y);


//	GetParent()->SetWindowText(logs);

	if (status_ == enButtonStatus_Down) 
	{
		if (rc.PtInRect(point))
		{
			//SetStatus(DOWN);
		}
		else
		{
			SetStatus(enButtonStatus_Hover);
			Invalidate(FALSE);
		}
	}
	else if (prevStatus_ == enButtonStatus_Down && status_ == enButtonStatus_Hover)
	{
		SetStatus(enButtonStatus_Down);
	}
	else
	{
		//if (IsInRect(ButtonRect,point))
		if (rc.PtInRect(point))
		{
			//GetParent()->SetWindowText("END HOVER");
			SetStatus(enButtonStatus_Hover);
			Invalidate(FALSE);
		}
		else
		{
			//GetParent()->SetWindowText("END NORMAL");
			SetStatus(enButtonStatus_Normal);
			//Invalidate();
		}
	}

	CButton::OnMouseMove(nFlags, point);


	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = m_hWnd;

	_TrackMouseEvent(&tme);
}

void cmGlassButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (!GetSafeHwnd())
		return;

	CMemDC pthisDC(CDC::FromHandle(lpDrawItemStruct->hDC));
	COLORREF c = baseColor_;
	CRect rc;
	cmString caption;

	GetWindowText(caption);
	GetClientRect(rc);
	cell_.SetDrawArea(rc);
	
	switch (status_)
	{
		case enButtonStatus_Normal:
			break;
		
		case enButtonStatus_Hover:
			c = cmDrawUtils::MakeColorBrighter(baseColor_, 20);
			break;

		case enButtonStatus_Down:
			c = cmDrawUtils::MakeColorDarker(baseColor_, 20);
			break;
	}

	cell_.SetText(caption, false);
	
	renderer_.SetBkColor(c);
	renderer_.Draw(pthisDC, &cell_);
}

LRESULT cmGlassButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (!GetSafeHwnd())
		return 0;

	SetStatus(enButtonStatus_Normal);
	RedrawWindow();
	
	return 0;
}


void cmGlassButton::SetStatus(enum cmGlassButton::enButtonStatus bs)
{
	// store status
	//
	prevStatus_ = status_;
	status_ = bs;

}

BOOL cmGlassButton::IsInRect(CRect &rect, CPoint &point)
{
	return (point.x >= 0 && point.x <= (rect.right - rect.left) && point.y >= 0 && point.y <= (rect.bottom - rect.top))? TRUE : FALSE;
}


BOOL cmGlassButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
}

BOOL cmGlassButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message )
{
	// update cursor
	//
	HCURSOR hCursor;
	
	switch (status_) {
	
		case enButtonStatus_Normal:
			hCursor = ::LoadCursor(NULL, IDC_ARROW);
			break;

		case enButtonStatus_Hover:
			hCursor = ::LoadCursor(NULL, IDC_ARROW);
			break;

		case enButtonStatus_Down:
			hCursor = ::LoadCursor(NULL, IDC_ARROW);
			break;
	
	}

	::SetCursor(hCursor);

	return TRUE;
}

void cmGlassButton::SetBaseColor(COLORREF color)
{
	baseColor_ = color;
}

void cmGlassButton::EnableCaptionShadow(bool e)
{
	captionShadowEnabled_ = e;
	renderer_.EnableTextShadow(e);

	RedrawWindow();
}

void cmGlassButton::SetCaptionColor(COLORREF color)
{
	captionColor_ = color;

	renderer_.SetTextColor(color);

	RedrawWindow();
}

cmFontInfo& cmGlassButton::GetFontInfo()
{
	return renderer_.GetFontInfo();
}

void cmGlassButton::EnableBorder(bool e)
{
	renderer_.SetBorderEnable(e);
}