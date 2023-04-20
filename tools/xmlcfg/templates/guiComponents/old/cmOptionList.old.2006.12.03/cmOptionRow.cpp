// AsOptionRow.cpp: implementation of the cmOptionRow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmOptionRow.h"
#include "cmBaseCell.h"
#include "cmGenericTextCell.h"
#include "cmCheckBoxCell.h"
#include "cmComboBoxCell.h"
#include "cmNumericTextCell.h"
#include "cmHexadecimalTextCell.h"
#include "cmBinaryWildCardsCell.h"
#include "cmSmartNumericTextCell.h"
#include "ICmOptionRowListener.h"
#include "cmCellEnum.h"

/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/
int cmOptionRow::SplitterArea_ = 4;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmOptionRow::cmOptionRow():
	valueCell_(NULL),
	verticalSplitterPos_(200),
	borderColor_(::GetSysColor(COLOR_3DSHADOW)),
	parentWnd_(NULL),
	selected_(false),
	listener_(NULL),
	tooltipIconId_(0)
{
	
}

cmOptionRow::~cmOptionRow()
{
	if (valueCell_)
		delete valueCell_;
}

cmBaseCell* cmOptionRow::GetCaptionCell()
{
	return &captionCell_;
}


cmBaseCell* cmOptionRow::GetValueCell()
{
	return valueCell_;
}

void cmOptionRow::Draw(CDC* pDC)
{
	captionCell_.Draw(pDC);
	//if (!selected_)
	valueCell_->Draw(pDC);

	// draw vertical divider
	//
	CPen pen(PS_SOLID, 1, borderColor_);
	CPen* oldPen;
	
	oldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(verticalSplitterPos_, drawArea_.top);
	pDC->LineTo(verticalSplitterPos_, drawArea_.bottom + 1);
	
	pDC->MoveTo(drawArea_.right, drawArea_.top);
	pDC->MoveTo(drawArea_.right, drawArea_.bottom + 1);

	pDC->SelectObject(oldPen);

	//TRACE1("DrawRow caption =%s\n", caption_.c_str());
}

void cmOptionRow::SetValueCellType(enCmCellType cellType)
{
	CreateCell(cellType);

}

cmBaseCell* cmOptionRow::CreateCell(enCmCellType cellType)
{
	if(valueCell_)
		delete valueCell_;

	switch (cellType) {
		case enCmCellType_Caption:
			valueCell_ = new cmBaseCell;
			break;

		case enCmCellType_TextBox:
			valueCell_ = new cmGenericTextCell;
			break;

		case enCmCellType_NumTextBox:
			valueCell_ = new cmNumericTextCell;
			break;

		case enCmCellType_CheckBox:
			valueCell_ = new cmCheckBoxCell;
			break;

		case enCmCellType_ComboBox:
			valueCell_ = new cmComboBoxCell;
			break;

		case enCmCellType_HexTextBox:
			valueCell_ = new cmHexadecimalTextCell;
			break;

		case enCmCellType_BinaryWildCards:
			valueCell_ = new cmBinaryWildCardsCell;
			break;

		case enCmCellType_SmartNumTextBox:
			valueCell_ = new cmSmartNumericTextCell;
			break;

	}

	valueCell_->SetParentWindow(parentWnd_);
	valueCell_->SetFontInfo(fontInfo_);

	return valueCell_;
}

void cmOptionRow::SetDrawArea(const CRect& drawArea)
{
	drawArea_ = drawArea;

	CRect tmp;

	tmp = drawArea_;
	tmp.right = verticalSplitterPos_;
	captionCell_.SetDrawArea(tmp);

	tmp = drawArea_;
	tmp.left = verticalSplitterPos_ + 1;
	valueCell_->SetDrawArea(tmp);
}

CRect cmOptionRow::GetDrawArea() const
{
	return drawArea_;
}

void cmOptionRow::SetVerticalSplitterPos(int pos)
{
	verticalSplitterPos_ = pos;

	SetDrawArea(drawArea_);
}

int cmOptionRow::GetVerticalSplitterPos() const
{
	return verticalSplitterPos_;
}

void cmOptionRow::SetParentWindow(CWnd* parentWnd)
{
	parentWnd_ = parentWnd;
	
	captionCell_.SetParentWindow(parentWnd_);
}

bool cmOptionRow::ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed = false;
	bool oldSelection;

	oldSelection = selected_;
	processed = captionCell_.ProcessMessage(msg, lParam, wParam, pt, parentRect, needRedraw);
	processed = valueCell_->ProcessMessage(msg, lParam, wParam, pt, parentRect, needRedraw);
	
	if (oldSelection != selected_)
		needRedraw = true;

	Select(valueCell_->IsSelected() || captionCell_.IsSelected(), false);
	UpdateColors();

	return processed;
}

void cmOptionRow::SetFontInfo(const cmString& fontName, int fontSize, bool fontBold)
{
	cmFontInfo tmp;

	tmp.SetFontName(fontName);
	tmp.SetFontSize(fontSize);
	tmp.SetFontBold(fontBold);

	SetFontInfo(tmp);
}

void cmOptionRow::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;

	captionCell_.SetFontInfo(fontInfo);

	if(valueCell_)
		valueCell_->SetFontInfo(fontInfo);
}

cmOptionRow::enCmOptionRowHitTest cmOptionRow::HitTest(const CPoint& pt)
{
	if (pt.x >= verticalSplitterPos_ - SplitterArea_ && pt.x <= verticalSplitterPos_ + SplitterArea_)
		return enCmOptionRowHitTest_Splitter;

	if (captionCell_.HitTest(pt))
		return enCmOptionRowHitTest_Caption;

	if (valueCell_->HitTest(pt))
		return enCmOptionRowHitTest_Value;

	return enCmOptionRowHitTest_None;
}

void cmOptionRow::Select(bool selection, bool redraw)
{
	if (selected_ == selection && (valueCell_->IsSelected() == selection || valueCell_->HasFocus() == selection))
		return;

	selected_ = selection;

	UpdateColors();
	
	if (selected_) {
		if (listener_)
			listener_->OnRowSelection(this, redraw);
		captionCell_.SetSelection(redraw);
		if (!valueCell_->IsSelected() && !valueCell_->HasFocus())
			valueCell_->SetSelection(redraw);
	} else {
		valueCell_->KillFocus(true, redraw);
		valueCell_->KillSelection(redraw);
		captionCell_.KillSelection(redraw);
		//parentWnd_->SetFocus();
	}

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(captionCell_.GetDrawArea());
}

void cmOptionRow::UpdateColors()
{
	if (selected_) {
		captionCell_.SetBackColor(::GetSysColor(COLOR_HIGHLIGHT));
		captionCell_.SetForeColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	} else {
		captionCell_.SetBackColor(::GetSysColor(COLOR_WINDOW));
		captionCell_.SetForeColor(::GetSysColor(COLOR_WINDOWTEXT));
	}	
}

bool cmOptionRow::IsSelected() const
{
	return selected_;
}

bool cmOptionRow::HasFocus()
{
	return valueCell_->HasFocus();
}

void cmOptionRow::SetCaption(const cmString& caption)
{
	caption_ = caption;

	captionCell_.SetText(caption);
}

cmString cmOptionRow::GetCaption() const
{
	return caption_;
}

void cmOptionRow::SetDescription(const cmString& description)
{
	description_ = description;
}

cmString cmOptionRow::GetDescription() const
{
	return description_;
}

void cmOptionRow::RegisterListener(ICmOptionRowListener* listener)
{
	listener_ = listener;
}

void cmOptionRow::SetTooltipText(const cmString& tooltip)
{
	tooltipText_ = tooltip;
}

cmString cmOptionRow::GetTooltipText() const
{
	return tooltipText_;
}

void cmOptionRow::SetTooltipIcon(UINT iconId)
{
	tooltipIconId_ = iconId;
}

UINT cmOptionRow::GetTooltipIcon() const
{
	return tooltipIconId_;
}
