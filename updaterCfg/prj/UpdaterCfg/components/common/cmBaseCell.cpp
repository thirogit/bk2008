// AsBaseCell.cpp: implementation of the cmBaseCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmBaseCell.h"
#include "cmDrawUtils.h"
#include "cmRenderer_CaptionCell.h"
#include "cmRenderer_GradientCell.h"
#include "cmBaseCellInfo.h"
#include "cmCellFactory.h"
#include "cmCellDefs.h"
#include <assert.h>

#define RENDER_INDEX_NORMAL			0
#define RENDER_INDEX_FOCUS			1
#define RENDER_INDEX_MOUSEOVER		2
#define RENDER_INDEX_FOCUSMOUSEOVER 3


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmBaseCell::cmBaseCell():
	descriptor_(NULL),
	userId_(0),
	readOnly_(false),
	info_(NULL)
{
	Initialize(true);
}

cmBaseCell::cmBaseCell(bool createStockRenderers):
	descriptor_(NULL),
	userId_(0),
	info_(NULL)
{
	Initialize(createStockRenderers);
}

void cmBaseCell::Initialize(bool createStockRenderers)
{
	parentWnd_ = NULL;
	drawArea_ = CRect(0,0,0,0);
	hasFocus_ = false;
	isSelected_ = false;
	visible_ = true;
	cellListener_ = NULL;
	type_ = enCmCellType_Caption;
	mouseOver_ = false;
	renderingMode_ = enCmRenderingMode_Stock;

	if (createStockRenderers) {
		// create "normal" renderer
		//
		{
			cmRenderer_CaptionCell* rend = new cmRenderer_CaptionCell;

			rend->SetBkColor(::GetSysColor(COLOR_WINDOW));
			rend->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			rend->GetFontInfo().SetFontSize(12);
			stockRenderers_[enCmCellStatus_Normal] = rend;
		}

		// create "selected" rendering set
		//
		{
			cmRenderer_CaptionCell* rend = new cmRenderer_CaptionCell;

			rend->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			rend->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			rend->GetFontInfo().SetFontSize(12);
			
			stockRenderers_[enCmCellStatus_Selected] = rend;
		}

		// create "normal + mouseover" rendering set
		//
		{
			cmRenderer_CaptionCell* rend = new cmRenderer_CaptionCell;

			rend->SetBkColor(::GetSysColor(COLOR_WINDOW));
			rend->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			rend->GetFontInfo().SetFontSize(12);
		
			stockRenderers_[enCmCellStatus_NormalMouseOver] = rend;
		}

		// create "selected + mouseover" rendering set
		//
		{
			cmRenderer_CaptionCell* rend = new cmRenderer_CaptionCell;

			rend->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			rend->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			rend->GetFontInfo().SetFontSize(12);
			
			stockRenderers_[enCmCellStatus_SelectedMouseOver] = rend;
		}
	}
}

cmBaseCell::~cmBaseCell()
{
	ReleaseStockRenderers();

	if (info_)
		cmCellFactory::ReleaseCellInfo(info_);
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

void cmBaseCell::Draw(CDC* pDC, const CPoint& mousePos)
{
	if (!visible_)
		return;
	
	int index;
	cmCellBaseRenderer* rend;

	mouseOver_ = (drawArea_.PtInRect(mousePos) == TRUE);
	index = mouseOver_ * 2 + isSelected_;

	if (renderingMode_ == enCmRenderingMode_Stock)
		rend = stockRenderers_[index];
	else
		rend = customRenderers_[index];

	rend->Draw(pDC, this);
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

bool cmBaseCell::ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
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
				SetSelection(false);
				needRedraw = true;
			} else {
				if (isSelected_) {
					KillSelection(false);
					if (!IsHidden())
						needRedraw = true;
				}
			}
			break;
		}
		break;


	}

	return processed;
}

void cmBaseCell::KillFocus(bool storeData, bool redraw)
{
	if (!hasFocus_)
		return;

	hasFocus_ = false;
	if (parentWnd_) {
		parentWnd_->SendMessage(CMC_LOSTFOCUS, 0, (LPARAM)info_);

		if (redraw)
		parentWnd_->RedrawWindow(drawArea_);
}
}

void cmBaseCell::SetFocus(bool redraw)
{
	if (hasFocus_)
		return;

	hasFocus_ = true;

	if (parentWnd_) {
		parentWnd_->SendMessage(CMC_SETFOCUS, 0, (LPARAM)info_);

		if (redraw)
		parentWnd_->RedrawWindow(drawArea_);
}
}

bool cmBaseCell::HasFocus()
{
	return hasFocus_;
}

void cmBaseCell::SetSelection(bool redraw)
{
	if (isSelected_)
		return;

	isSelected_ = true;

	if (parentWnd_) {
		parentWnd_->SendMessage(CMC_SETSEL, 0, (LPARAM)info_);

		if (redraw)
		parentWnd_->RedrawWindow(drawArea_);
}
}

void cmBaseCell::KillSelection(bool redraw)
{
	if (!isSelected_)
		return;
		
	KillFocus(true, redraw);
	isSelected_ = false;

	if (parentWnd_) {
		
		parentWnd_->SendMessage(CMC_LOSTSEL, 0, (LPARAM)info_);
		if (redraw)
		parentWnd_->RedrawWindow(drawArea_);
}
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

void cmBaseCell::SetType(enCmCellType type)
{
	type_ = type;
}

enCmCellType cmBaseCell::GetType() const
{
	return type_;
}

void cmBaseCell::SetRenderer(enCmCellStatus status, cmCellBaseRenderer* rend)
{
	customRenderers_[status] = rend;
}

cmCellBaseRenderer* cmBaseCell::GetCurrentRenderer()
{
	int index;
	
	index = mouseOver_ * 2 + isSelected_;

	if (renderingMode_ == enCmRenderingMode_Stock)
		return stockRenderers_[index];

	return customRenderers_[index];
}

cmCellBaseRenderer* cmBaseCell::GetRenderer(enCmCellStatus status)
{
	if (renderingMode_ == enCmRenderingMode_Stock)
		return stockRenderers_[status];

	return customRenderers_[status];
}

bool cmBaseCell::IsMouseOver() const
{
	return mouseOver_;
}

void cmBaseCell::SetRenderingMode(enCmRenderingMode mode)
{
	renderingMode_ = mode;
}

enCmRenderingMode cmBaseCell::GetRenderingMode() const
{
	return renderingMode_;
}

void cmBaseCell::SetDescriptor(cmCellDescriptor* desc)
{
	descriptor_ = desc;
}

cmCellDescriptor* cmBaseCell::GetDescriptor()
{
	return descriptor_;
}

bool cmBaseCell::IsPrintable(TCHAR c)
{
	bool ret;
	
#ifdef UNICODE
	ret = (iswprint(c) != 0);
#else
	ret = (isprint(c) != 0);
#endif

	return ret;
}

bool cmBaseCell::IsValidChar(TCHAR c, const cmString& text)
{
	return IsPrintable(c);
}

TCHAR cmBaseCell::VirtualKeyCode2Ascii(WPARAM wParam, LPARAM lParam)
{
	BYTE keyState[256];
	WORD out;
	HKL layout;
	
	layout = GetKeyboardLayout(0);

	GetKeyboardState(keyState);
	out = 0;
	ToAsciiEx(wParam, lParam, keyState, &out, 0, layout);

	return (TCHAR)out;
}

void cmBaseCell::SetUserId(DWORD id)
{
	userId_ = id;
}

DWORD cmBaseCell::GetUserId() const
{
	return userId_;
}


void cmBaseCell::SetReadOnly(bool readOnly)
{
	readOnly_ = readOnly;
}

bool cmBaseCell::GetReadOnly() const
{
	return readOnly_;
}

void cmBaseCell::ReleaseCustomRenderers()
{
	for (int i = 0; i < 4; i++) {
		if (customRenderers_[i]) {
			delete customRenderers_[i];
			customRenderers_[i] = NULL;
		}
	}
}

void cmBaseCell::ReleaseStockRenderers()
{
	for (int i = 0; i < 4; i++) {
		if (stockRenderers_[i]) {
			delete stockRenderers_[i];
			stockRenderers_[i] = NULL;
		}
	}
}

void cmBaseCell::SetInfo(cmBaseCellInfo* info)
{
	assert(info_ == NULL);

	info_ = info;
}

cmBaseCellInfo* cmBaseCell::GetInfo()
{
	return info_;
}
