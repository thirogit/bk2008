// cmBaseCell.cpp: implementation of the cmBaseCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmBaseCell.h"
#include "DrawUtils.h"

/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmBaseCell::cmBaseCell():
	backColor_(::GetSysColor(COLOR_WINDOW)),
	foreColor_(::GetSysColor(COLOR_BTNTEXT)),
	parentWnd_(NULL),
	drawArea_(0,0,0,0),
	hasFocus_(false),
	isSelected_(false),
	visible_(true),
	cellListener_(NULL),
	leftMargin_(10),
	topMargin_(2),
	rightMargin_(5),
	bottomMargin_(2),
	iconId_(0)
{
}

cmBaseCell::~cmBaseCell()
{

}

void cmBaseCell::SetDrawArea(const CRect& drawArea)
{
	drawArea_ = drawArea;
}

CRect cmBaseCell::GetDrawArea() const
{
	return drawArea_;
}

CSize cmBaseCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmBaseCell::Draw(CDC* pDC)
{
	if (!visible_)
		return;

	CBrush b(backColor_);
	CRect tmp;

	pDC->FillRect(drawArea_, &b);
	
	if (iconId_)
		DrawUtils::DrawIcon(pDC, iconId_, CPoint(drawArea_.left + 1, drawArea_.top + 3), 16);

	DrawText(pDC);	
}

void cmBaseCell::DrawText(CDC* pDC)
{
	if (text_.GetLength()) {
		CRect txtRect;
		CFont* font;

		txtRect = drawArea_;
		//txtRect.DeflateRect(9, 0, 5, 2);
		//txtRect.DeflateRect(10, 2, 5, 2);
		txtRect.DeflateRect(leftMargin_, topMargin_, rightMargin_, bottomMargin_);
        font = fontInfo_.CreateFont();
		DrawUtils::DrawString(pDC, text_, txtRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS, foreColor_, font);
		fontInfo_.ReleaseFont();
	}
}

bool cmBaseCell::HitTest(const CPoint& pt)
{
	return drawArea_.PtInRect(pt) == TRUE;
}


void cmBaseCell::SetVisible(bool visible)
{
	visible_ = visible;
}

bool cmBaseCell::IsVisible() const
{
	return visible_;
}

void cmBaseCell::SetText(const cmString& text, bool redraw)
{
	text_ = text;

	if(redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

cmString cmBaseCell::GetText()
{
	return text_;	
}

void cmBaseCell::SetParentWindow(CWnd* parentWnd)
{
	parentWnd_ = parentWnd;
}

bool cmBaseCell::ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;
	
	processed = false;
	switch (msg) {
		case WM_LBUTTONDOWN:
		{
			BOOL ret;
			bool changed = false;

			ret = drawArea_.PtInRect(pt);
			if (ret) {
				if (!IsSelected()) {
					SetSelection(false);
					needRedraw = true;
				} else if (!HasFocus()) {
					SetFocus(false);
					needRedraw = true;
				}
			} else {
				if (IsSelected()) {
					KillSelection(false);
					if (!IsHidden())
						needRedraw = true;
				} else if (HasFocus()) {
					KillFocus(false);
					if (!IsHidden())
						needRedraw = true;
				}
			}
			
			
			break;
		}
	}

	return processed;
}

void cmBaseCell::SetFontInfo(const cmString& fontName, int fontSize, bool fontBold)
{
	cmFontInfo tmp;

	tmp.SetFontName(fontName);
	tmp.SetFontSize(fontSize);
	tmp.SetFontBold(fontBold);

	SetFontInfo(tmp);
}

void cmBaseCell::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;	
}

void cmBaseCell::KillFocus(bool storeData, bool redraw)
{
	hasFocus_ = false;
	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

void cmBaseCell::SetFocus(bool redraw)
{
	hasFocus_ = true;

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

bool cmBaseCell::HasFocus()
{
	return hasFocus_;
}

void cmBaseCell::SetBackColor(COLORREF backColor)
{
	backColor_ = backColor;
}

COLORREF cmBaseCell::GetBackColor() const
{
	return backColor_;
}

void cmBaseCell::SetForeColor(COLORREF foreColor)
{
	foreColor_ = foreColor;
}

COLORREF cmBaseCell::GetForeColor() const
{
	return foreColor_;
}

void cmBaseCell::SetSelection(bool redraw)
{
	isSelected_ = true;

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

void cmBaseCell::KillSelection(bool redraw)
{
	KillFocus(true, redraw);
	isSelected_ = false;

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

bool cmBaseCell::IsSelected()
{
	return isSelected_;
}

bool cmBaseCell::IsHidden()
{
	CRect parentRect;
	bool isHidden;

	if (parentWnd_ == NULL)
		return false;

	parentWnd_->GetClientRect(parentRect);
	
	// check if this cell is visible
	//
	isHidden = (drawArea_.bottom < parentRect.top || drawArea_.top > parentRect.bottom);

	return isHidden;
}

void cmBaseCell::SetCellListener(ICmCellListener* cellListener)
{
	cellListener_ = cellListener;
}

void cmBaseCell::SetName(const cmString& name)
{
	name_ = name;
}

cmString cmBaseCell::GetName() const
{
	return name_;
}

void cmBaseCell::SetLeftMargin(int l)
{
	leftMargin_ = l;
}

void cmBaseCell::SetTopMargin(int t)
{
	topMargin_ = t;
}

void cmBaseCell::SetRightMargin(int r)
{
	rightMargin_ = r;
}

void cmBaseCell::SetBottomMargin(int b)
{
	bottomMargin_ = b;
}

void cmBaseCell::SetIcon(UINT iconId)
{
	iconId_ = iconId;
}