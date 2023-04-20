// AsCheckBoxCell.cpp: implementation for the cmBaseCell class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "cmCheckBoxCell.h"
#include "DrawUtils.h"
#include "resource.h"
#include "ICmCellListener.h"

#define CHK_ICON_SIZE		16
#define CHK_LEFT_BORDER		6
#define CHK_TOP_BORDER		2

cmCheckBoxCell::cmCheckBoxCell():
	checked_(false),
	iconRect_(0,0,0,0),
	readOnly_(false)
{
}

cmCheckBoxCell::~cmCheckBoxCell()
{}

void cmCheckBoxCell::SetDrawArea(const CRect& drawArea)
{
	if (!visible_)
		return;

	CRect tmp;

	tmp = drawArea;
	
	if (tmp.Height() < 2 * CHK_TOP_BORDER + CHK_ICON_SIZE)
		tmp.bottom = tmp.top + 2 * CHK_TOP_BORDER + CHK_ICON_SIZE;

	if (tmp.Width() < 2 * CHK_LEFT_BORDER + CHK_ICON_SIZE)
		tmp.right = tmp.left + 2 * CHK_LEFT_BORDER + CHK_ICON_SIZE;

	iconRect_.left = tmp.left + CHK_LEFT_BORDER;
	iconRect_.top = tmp.top + (tmp.Height() - CHK_ICON_SIZE) / 2;
	iconRect_.right = iconRect_.left + CHK_ICON_SIZE;
	iconRect_.bottom = iconRect_.top + CHK_ICON_SIZE;
	
	cmBaseCell::SetDrawArea(tmp);
}

CSize cmCheckBoxCell::GetMinSize()
{
	CSize minSize;

	minSize.cx = 2 * CHK_LEFT_BORDER + CHK_ICON_SIZE;
	minSize.cy = 2 * CHK_TOP_BORDER + CHK_ICON_SIZE;

	return minSize;
}

void cmCheckBoxCell::Draw(CDC* pDC)
{
	if (!visible_ || IsHidden())
		return;

	cmBaseCell::Draw(pDC);

	DrawUtils::DrawIcon(pDC, checked_ ? IDI_CHK_CHECKED : IDI_CHK_UNCHECKED, iconRect_.TopLeft(), CHK_ICON_SIZE);

	CRect txtRect;
	CFont* font;

	txtRect = drawArea_;
	txtRect.DeflateRect(iconRect_.Width() + 2 * CHK_LEFT_BORDER, 0, CHK_LEFT_BORDER, 0);

	font = fontInfo_.CreateFont();
	if (checked_)
		DrawUtils::DrawString(pDC, checkedText_, txtRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE, foreColor_, font); 
	else
		DrawUtils::DrawString(pDC, uncheckedText_, txtRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE, foreColor_, font); 

	fontInfo_.ReleaseFont();
}

bool cmCheckBoxCell::HitTest(const CPoint& pt)
{
	return cmBaseCell::HitTest(pt);
}

void cmCheckBoxCell::SetCheckedText(const cmString& checkedText, bool redraw)
{
	checkedText_ = checkedText;
	if (parentWnd_ && redraw)
		parentWnd_->RedrawWindow(drawArea_);
}

cmString cmCheckBoxCell::GetCheckedText() const
{
	return checkedText_;
}

void cmCheckBoxCell::SetUncheckedText(const cmString& uncheckedText, bool redraw)
{
	uncheckedText_ = uncheckedText;

	if (parentWnd_ && redraw)
		parentWnd_->RedrawWindow(drawArea_);
}

cmString cmCheckBoxCell::GetUncheckedText() const
{
	return uncheckedText_;
}

bool cmCheckBoxCell::ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	cmBaseCell::ProcessMessage(msg, lParam, wParam, pt, parentRect, needRedraw);

	if (msg == WM_LBUTTONUP && iconRect_.PtInRect(pt) && !readOnly_) {
		SetChecked(!checked_);
	}

	return false;
}

void cmCheckBoxCell::KillFocus(bool storeData, bool redraw)
{
	cmBaseCell::KillFocus(storeData, redraw);
}

void cmCheckBoxCell::SetFocus(bool redraw)
{
	cmBaseCell::SetFocus(redraw);
}


void cmCheckBoxCell::SetChecked(bool checked, bool redraw)
{
	if (checked == checked_)
		return;

	if (cellListener_) {
		enCmValidationResult ret;

		ret = cellListener_->OnValueChanged(name_, !checked, checked);
		// if value has been refused keep the old value
		//
		if (ret & enCmValidationResult_Accepted)
			checked_ = checked;
		
		validationMarker_ = ret;
		
		redraw = true;
	} else {
		checked_ = checked;
	}
	
	if (parentWnd_ && redraw)
		parentWnd_->RedrawWindow(drawArea_);
}

bool cmCheckBoxCell::GetChecked() const
{
	return checked_;
}

void cmCheckBoxCell::SetReadOnly(bool readOnly)
{
	readOnly_ = readOnly;
}

bool cmCheckBoxCell::IsReadOnly() const
{
	return readOnly_;
}