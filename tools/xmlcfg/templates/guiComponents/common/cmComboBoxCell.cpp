// cmComboBoxCell.cpp
//

#include "stdafx.h"
#include <assert.h>
#include "resource.h"
#include "cmDrawUtils.h"
#include "cmComboBoxCell.h"
#include "cmComboBox.h"
#include "cmCellDefs.h"
#include "cmCellBaseRenderer.h"

cmComboBoxCell::cmComboBoxCell():
	comboBox_(NULL),
	selKey_(0),
	oldSelKey_(0)
{
}

cmComboBoxCell::~cmComboBoxCell()
{
	KillFocus(false);
	ReleaseCombo();
}

void cmComboBoxCell::SetDrawArea(const CRect& drawArea)
{
	cmBaseCell::SetDrawArea(drawArea);

	if (hasFocus_) {
		CRect textArea;

		textArea = drawArea_;
		textArea.DeflateRect(0/*6*/, 0, 0, 0);
		parentWnd_->RedrawWindow(drawArea_);
		comboBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	}
}

CSize cmComboBoxCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmComboBoxCell::Draw(CDC* pDC, const CPoint& mousePos)
{
	if (!visible_ || IsHidden())
		return;

	if (hasFocus_)
		text_ = comboBox_->GetSelString();
	
	cmBaseCell::Draw(pDC, mousePos);

	if (!hasFocus_)
		DrawComboButton(pDC);
	else
		comboBox_->RedrawWindow();	
}


void cmComboBoxCell::DrawComboButton(CDC* pDC)
{
	if (isSelected_) {
		CSize bmpSize;
		CRect bmpRect;
		CRect clientRect;

		clientRect = drawArea_;
		bmpSize = cmDrawUtils::GetBitmapSize(IDB_COMBO_BOX_NORMAL);

		bmpRect = clientRect;
		bmpRect.left = bmpRect.right - bmpSize.cx - 2;
		bmpRect.top = drawArea_.top + (bmpRect.Height() - bmpSize.cy) / 2;

		cmDrawUtils::DrawBitmap(pDC, IDB_COMBO_BOX_NORMAL, bmpRect.TopLeft(), cmDrawUtils::enBmp_AlignLeft);

		clientRect.DeflateRect(4,0,bmpRect.Width() + 2, 0);
	}
	//DrawUtils::DrawString(pDC, text_, clientRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE, ::GetSysColor(COLOR_BTNTEXT), &font_);
}


bool cmComboBoxCell::HitTest(const CPoint& pt)
{
	BOOL ret;

	ret = drawArea_.PtInRect(pt);

	return ret == TRUE;
}

void cmComboBoxCell::SetVisible(bool visible)
{
	cmBaseCell::SetVisible(visible);

	if (visible)
		SetFocus();
	else
		KillFocus(false);
}

void cmComboBoxCell::SetText(const cmString& text)
{
	cmBaseCell::SetText(text);

	//if (HasFocus())
	///	comboBox_->SetWindowText(text.c_str());
}

bool cmComboBoxCell::ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;
	bool isSelected;

	isSelected = isSelected_;
	//processed = false;
	processed = cmBaseCell::ProcessMessage(msg, wParam, lParam, pt, parentRect, needRedraw);

	switch (msg) {
		
		case WM_LBUTTONDOWN:
			{
				if ((isSelected_ == isSelected) && drawArea_.PtInRect(pt)) {
					SetFocus(false);
					processed = true;
					needRedraw = true;
				} else if (!isSelected_) {
					KillFocus(false, false);
				}
			}
			break;

		case WM_KEYDOWN:
			{
				if (HasFocus()) {
					switch (wParam) {
						case VK_ESCAPE:
							KillFocus(false, false);
							processed = false;
							needRedraw = true;
							break;

						case VK_RETURN:
							KillFocus(true, false);
							processed = false;
							needRedraw = true;
							break;

						case VK_UP:
						case VK_DOWN:
							if (comboBox_)
								comboBox_->SendMessage(msg, wParam, lParam);
							processed = true;
							needRedraw = true;
							break;
					}
				} if (IsSelected()) {
					switch (wParam) {
						case VK_DOWN:
							if (GetAsyncKeyState(VK_LCONTROL)) {
								SetFocus();
								processed = true;
								needRedraw = true;
							}
							break;
						case VK_UP:
							if (HasFocus()) {
								processed = true;
								needRedraw = true;
							}
							break;

						case VK_F2:
							SetFocus();
							processed = true;
							needRedraw = true;
							break;
					}
				}
			}
			break;

		case WM_COMMAND:
			{
				if (lParam == CBN_SELCHANGE) {
					StoreData();
				}
			}
	} // close switch

	return processed;
}

void cmComboBoxCell::SetFocus(bool redraw)
{
	if (HasFocus() || readOnly_)
		return;

	cmBaseCell::SetFocus(false);
	
	CRect comboArea;

	comboArea = GetComboBoxArea();
	
	ReleaseCombo();
	
	comboBox_ = new cmComboBox;
		
	comboArea.InflateRect(0,0,0,100);	// this is for windows 2000
	comboBox_->Create(WS_VISIBLE | CBS_DROPDOWNLIST, comboArea, parentWnd_, -1);
	comboBox_->SetOwner(parentWnd_);
	
	// get font info from the top renderer
	//
	cmCellBaseRenderer* rend;
	
	rend = GetCurrentRenderer();
	fontInfo_ = rend->GetFontInfo();

	font_.CreateFont(-fontInfo_.GetFontSize(), 
					 0,										/* width */ 
					 0,										/* escapement */
					 0,										/* orientation */
					 fontInfo_.GetFontBold() ? FW_BOLD : FW_NORMAL,		/* weight*/
					 0,										/* italic*/
					 0,										/* underline */
					 0,										/* strikeout */
					 ANSI_CHARSET,							/* charset */
					 OUT_DEFAULT_PRECIS,					/* out precision */
					 CLIP_DEFAULT_PRECIS,					/* clip precision */
					 DEFAULT_QUALITY,						/* quality */
					 DEFAULT_PITCH,							/* pitch family */
					 fontInfo_.GetFontName().c_str());
	
	map<int, cmString>::iterator it;

	comboBox_->SetFont(&font_);
	comboBox_->ResetContent();
	for (it = items_.begin(); it != items_.end(); it++)
		comboBox_->AddItem(it->second, it->first);

	oldSelKey_ = selKey_;
	oldText_ = text_;
	comboBox_->SelectItem(selKey_);
	comboBox_->SetWindowText(text_.c_str());
	comboBox_->SetFocus();
	comboBox_->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	comboBox_->SetParentCell(this);
	comboBox_->ShowDropDown();

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

CRect cmComboBoxCell::GetComboBoxArea()
{
	CRect tmp;

	tmp = drawArea_;
	//tmp.DeflateRect(1, 1, 1, 1);
	

	return tmp;
}

void cmComboBoxCell::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;

//	if (editMode_)
//		comboBox_->SetFont(fontInfo_.GetFont());
}

void cmComboBoxCell::KillFocus(bool storeData, bool redraw)
{	
	if (!HasFocus())
		return;

	if (storeData)
		StoreData();

	cmBaseCell::KillFocus(storeData, false);

	if (comboBox_ == NULL)
		return;

	UINT lastChar;

	lastChar = comboBox_->GetLastChar();
	
	ReleaseCombo();
	if (redraw && parentWnd_ && parentWnd_->GetSafeHwnd()) {
		parentWnd_->SendMessage(CMC_LOSTFOCUS, lastChar, 0);
		parentWnd_->RedrawWindow(drawArea_);
		parentWnd_->SetFocus();
	}
}

void cmComboBoxCell::StoreData()
{
	if (HasFocus() && comboBox_) {
		cmString tmp;
		
		comboBox_->GetWindowText(tmp);
		text_ = tmp;
		comboBox_->GetSelKey(selKey_);

		if (selKey_ != oldSelKey_ && cellListener_) {
			bool ret;

			ret = cellListener_->OnValueChanged(this, oldText_, text_);
			if (!ret) {
				comboBox_->SelectItem(oldSelKey_);
				comboBox_->SetWindowText(oldText_.c_str());
			}
		}
	}
}

cmString cmComboBoxCell::GetText()
{
	if (HasFocus())
		StoreData();

	return text_;
}

void cmComboBoxCell::SetSelection(bool redraw)
{
	cmBaseCell::SetSelection(redraw);
	
}

void cmComboBoxCell::KillSelection(bool redraw)
{
	cmBaseCell::KillSelection(redraw);
}

void cmComboBoxCell::AddItem(const cmString& caption, int key)
{
	if (comboBox_)
		comboBox_->AddItem(caption, key);

	items_[key] = caption;	
}

void cmComboBoxCell::SelectItem(const cmString& caption)
{
	
	map<int, cmString>::iterator it;

	for (it = items_.begin(); it != items_.end(); it++) {
		if (it->second == caption) {
			SelectItem(it->first);
			return;
		}
	}
}

void cmComboBoxCell::SelectItem(int key)
{
	if (comboBox_)
		comboBox_->SelectItem(key);

	map<int, cmString>::iterator it;

	it = items_.find(key);
	if (it != items_.end()) {
		selKey_ = key;
		text_ = it->second;
	} else {
		selKey_ = 0;
		text_ = _T("");
	}
}

int cmComboBoxCell::GetSelectedItemKey()
{
	int ret;

	if (comboBox_)
		comboBox_->GetSelKey(ret);
	else
		ret = selKey_;
	
	return ret;
}

cmString cmComboBoxCell::GetSelectedItemCaption()
{
	cmString ret;

	if (comboBox_)
		ret = comboBox_->GetSelString();
	else
		ret = text_;

	return ret;
}

void cmComboBoxCell::DeleteAllItems()
{
	return;

	if (hasFocus_)
		comboBox_->Clear();

	items_.clear();
}

void cmComboBoxCell::ReleaseCombo()
{
	if (comboBox_) {
		delete comboBox_;
		comboBox_ = NULL;
	}

	if (font_.GetSafeHandle())
		font_.DeleteObject();
}