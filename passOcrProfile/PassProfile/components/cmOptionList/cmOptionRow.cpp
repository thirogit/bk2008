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
#include "cmRenderer_CaptionCell.h"
#include "cmDateCell.h"
#include "cmTimeCell.h"
#include "cmCellEnum.h"
#include "cmCellFactory.h"
#include "cmBaseCellInfo.h"
#include "cmInfoArea.h"
#include "ICmOptionRowListener.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int cmOptionRow::SplitterArea_ = 4;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmOptionRow::cmOptionRow():
	valueCell_(NULL),
	verticalSplitterPos_(200),
	splitterColor_(::GetSysColor(COLOR_3DSHADOW)),
	parentWnd_(NULL),
	selected_(false),
	listener_(NULL)
{
	captionCell_.SetInfo(cmCellFactory::CreateCellInfo());
	captionCell_.GetInfo()->SetCellContainer(this);
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

void cmOptionRow::Draw(CDC* pDC, const CPoint& mousePos)
{
	captionCell_.Draw(pDC, mousePos);
	valueCell_->Draw(pDC, mousePos);

	// draw vertical divider
	//
	CPen pen(PS_SOLID, 1, splitterColor_);
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

		case enCmCellType_Date:
			valueCell_ = new cmDateCell;
			break;

		case enCmCellType_Time:
			valueCell_ = new cmTimeCell;
			break;

	}

	valueCell_->SetParentWindow(parentWnd_);
	valueCell_->SetInfo(captionCell_.GetInfo());
		
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

bool cmOptionRow::ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed = false;
	bool oldSelection;

	oldSelection = selected_;
	processed = captionCell_.ProcessMessage(msg, lParam, wParam, pt, parentRect, needRedraw);
	processed = valueCell_->ProcessMessage(msg, lParam, wParam, pt, parentRect, needRedraw);
	
	Select(valueCell_->IsSelected() || captionCell_.IsSelected(), false);
	
	if (oldSelection != selected_)
		needRedraw = true;

	return processed;
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
	selected_ = selection;
	
	if (selected_) {
		if (listener_)
			listener_->OnRowSelection(this, redraw);
		captionCell_.SetSelection(redraw);
		valueCell_->SetSelection(redraw);
	} else {
		valueCell_->KillFocus(true, redraw);
		valueCell_->KillSelection(redraw);
		captionCell_.KillSelection(redraw);
	}

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(captionCell_.GetDrawArea());
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
	captionCell_.SetText(caption);

	captionCell_.GetInfo()->SetTitle(caption);
}

cmString cmOptionRow::GetCaption()
{
	return captionCell_.GetInfo()->GetTitle();
}

void cmOptionRow::SetDescription(const cmString& description)
{
	captionCell_.GetInfo()->SetDescription(description);
}

cmString cmOptionRow::GetDescription()
{
	return captionCell_.GetInfo()->GetTitle();
}

void cmOptionRow::RegisterListener(ICmOptionRowListener* listener)
{
	listener_ = listener;
}
/*
void cmOptionRow::SetTooltipIcon(UINT iconId)
{
	tooltipIconId_ = iconId;

	SetIcon(enCmCellStatus_Normal, iconId);
	SetIcon(enCmCellStatus_Selected, iconId);
	SetIcon(enCmCellStatus_NormalMouseOver, iconId);
	SetIcon(enCmCellStatus_SelectedMouseOver, iconId);
}
*/

void cmOptionRow::SetIcon(enCmCellStatus status, UINT iconId)
{
	cmCellBaseRenderer* baseRend;
	cmRenderer_CaptionCell* rend;

	baseRend = captionCell_.GetRenderer(status);
	rend = dynamic_cast<cmRenderer_CaptionCell*>(baseRend);
	assert(rend);
	if (rend)
		rend->SetIconId(iconId);
}

void cmOptionRow::SetSplitterColor(COLORREF splitterColor)
{
	splitterColor_ = splitterColor;
}

COLORREF cmOptionRow::GetSplitterColor() const
{
	return splitterColor_;
}

void cmOptionRow::UpdateCells()
{
	UINT iconId;
	
	iconId = cmInfoArea::GetExtraIcon(captionCell_.GetInfo()->GetExtraMsgType());
	
	SetIcon(enCmCellStatus_Normal, iconId);
	SetIcon(enCmCellStatus_Selected, iconId);
	SetIcon(enCmCellStatus_NormalMouseOver, iconId);
	SetIcon(enCmCellStatus_SelectedMouseOver, iconId);

	if (parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

cmBaseCellInfo* cmOptionRow::GetInfo()
{
	return captionCell_.GetInfo();
}
