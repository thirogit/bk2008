// AsCheckBoxCell.cpp: implementation for the cmBaseCell class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "cmDrawUtils.h"
#include "resource.h"
#include "ICmCellListener.h"
#include "cmCheckBoxCell.h"
#include "cmCellDefs.h"
#include "cmRenderer_CaptionCell.h"
#include <assert.h>

cmCheckBoxCell::cmCheckBoxCell():
	checked_(true),		// this value is set to force the icon
	iconRect_(0,0,0,0)
{
	SetChecked(false);
}

cmCheckBoxCell::~cmCheckBoxCell()
{}

void cmCheckBoxCell::SetDrawArea(const CRect& drawArea)
{
	if (!visible_)
		return;
	
	iconRect_.left = drawArea.left + CHK_LEFT_BORDER;
	iconRect_.top = drawArea.top + (drawArea.Height() - CHK_ICON_SIZE) / 2;
	iconRect_.right = iconRect_.left + CHK_ICON_SIZE;
	iconRect_.bottom = iconRect_.top + CHK_ICON_SIZE;
	
	cmBaseCell::SetDrawArea(drawArea);
}

CSize cmCheckBoxCell::GetMinSize()
{
	CSize minSize;

	minSize.cx = 2 * CHK_LEFT_BORDER + CHK_ICON_SIZE;
	minSize.cy = 2 * CHK_TOP_BORDER + CHK_ICON_SIZE;

	return minSize;
}


void cmCheckBoxCell::Draw(CDC* pDC, const CPoint& mousePos)
{
	if (!visible_ || IsHidden())
		return;

	cmBaseCell::Draw(pDC, mousePos);
}


bool cmCheckBoxCell::HitTest(const CPoint& pt)
{
	return cmBaseCell::HitTest(pt);
}

void cmCheckBoxCell::SetCheckedText(const cmString& checkedText, bool redraw)
{
	checkedText_ = checkedText;
	if (checked_)
		text_ = checkedText;
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
	if (!checked_)
		text_ = uncheckedText;

	if (parentWnd_ && redraw)
		parentWnd_->RedrawWindow(drawArea_);
}

cmString cmCheckBoxCell::GetUncheckedText() const
{
	return uncheckedText_;
}

bool cmCheckBoxCell::ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	cmBaseCell::ProcessMessage(msg, wParam, lParam, pt, parentRect, needRedraw);

	switch (msg) {
		case WM_LBUTTONUP:
			if (iconRect_.PtInRect(pt) && !readOnly_)
				SetChecked(!checked_);		
			break;

		case WM_KEYDOWN:
			if (wParam == VK_SPACE && isSelected_) {
				SetChecked(!checked_, false);
				needRedraw = true;
			}
			break;
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
		bool ret;

		ret = cellListener_->OnValueChanged(this, checked_, checked);
		if (ret)
			checked_ = checked;
	} else {
		checked_ = checked;
	}
	
	SetIcon(checked_ ? IDI_CHK_CHECKED : IDI_CHK_UNCHECKED);
	text_ = checked_ ? checkedText_ : uncheckedText_;
	if (parentWnd_ && redraw)
		parentWnd_->RedrawWindow(drawArea_);
}

bool cmCheckBoxCell::GetChecked() const
{
	return checked_;
}

CRect cmCheckBoxCell::GetIconRect() const
{
	return iconRect_;
}

void cmCheckBoxCell::Initialize(bool createStockRenderers)
{
	cmBaseCell::Initialize(createStockRenderers);

	SetIcon(IDI_CHK_UNCHECKED);
}

void cmCheckBoxCell::SetIcon(UINT iconId)
{
	SetIcon(enCmCellStatus_Normal, iconId);
	SetIcon(enCmCellStatus_Selected, iconId);
	SetIcon(enCmCellStatus_NormalMouseOver, iconId);
	SetIcon(enCmCellStatus_SelectedMouseOver, iconId);
}

void cmCheckBoxCell::SetIcon(enCmCellStatus status, UINT iconId)
{
	cmCellBaseRenderer* baseRend;
	cmRenderer_CaptionCell* rend;

	baseRend = GetRenderer(status);
	rend = dynamic_cast<cmRenderer_CaptionCell*>(baseRend);
	assert(rend);
	if (rend)
		rend->SetIconId(iconId);
}

void cmCheckBoxCell::SetRenderer(enCmCellStatus status, cmCellBaseRenderer* rend)
{
	cmRenderer_CaptionCell* tmpRend;

	cmBaseCell::SetRenderer(status, rend);

	tmpRend = dynamic_cast<cmRenderer_CaptionCell*>(rend);
	assert(tmpRend);
	if (tmpRend)
		tmpRend->SetIconId(checked_ ? IDI_CHK_CHECKED : IDI_CHK_UNCHECKED);
}