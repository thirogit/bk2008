// cmComboBoxCell.cpp
//

#include "stdafx.h"
#include "cmComboBoxCell.h"
#include "cmComboBox.h"
#include "resource.h"
#include "DrawUtils.h"

cmComboBoxCell::cmComboBoxCell():
	comboBox_(NULL),
	selKey_(0),
	oldSelKey_(0)
{
}

cmComboBoxCell::~cmComboBoxCell()
{
	KillFocus(false);
}

void cmComboBoxCell::SetDrawArea(const CRect& drawArea)
{
	cmBaseCell::SetDrawArea(drawArea);

	if (HasFocus()) {
		CRect textArea;

		textArea = drawArea_;
		textArea.DeflateRect(6, 0, 0, 0);
		parentWnd_->RedrawWindow(drawArea_);
		comboBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	}
}

CSize cmComboBoxCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmComboBoxCell::Draw(CDC* pDC)
{
	if (!visible_ || IsHidden())
		return;

	if (HasFocus())
		text_ = comboBox_->GetSelString();
	
	cmBaseCell::Draw(pDC);

	if (!HasFocus())
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
		bmpSize = DrawUtils::GetBitmapSize(IDB_COMBO_BOX_NORMAL);

		bmpRect = clientRect;
		bmpRect.left = bmpRect.right - bmpSize.cx - 2;
		bmpRect.top = drawArea_.top + (bmpRect.Height() - bmpSize.cy) / 2;

		DrawUtils::DrawBitmap(pDC, IDB_COMBO_BOX_NORMAL, bmpRect.TopLeft(), DrawUtils::enBmp_AlignLeft);

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

bool cmComboBoxCell::ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;

	//processed = false;
	processed = cmBaseCell::ProcessMessage(msg, lParam, wParam, pt, parentRect, needRedraw);

	switch (msg) {
		
		case WM_LBUTTONDOWN:
			{
			
			
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
	if (HasFocus())
		return;

	cmBaseCell::SetFocus(false);
	
	CRect comboArea;

	comboArea = drawArea_;
	comboArea.DeflateRect(6, 0, 0, 0);
	if (comboBox_ == NULL) {
		comboBox_ = new cmComboBox;
		
		comboArea.InflateRect(0,0,0,100);	// this is for windows 2000
		comboBox_->Create(WS_VISIBLE | CBS_DROPDOWNLIST, comboArea, parentWnd_, -1);
		comboBox_->SetOwner(parentWnd_);
		
		if (!font_.GetSafeHandle()) {
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
		}
	}

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
	comboBox_->ShowDropDown();

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
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

	delete comboBox_;
	comboBox_ = NULL;

	if (font_.GetSafeHandle())
		font_.DeleteObject();

	if (redraw && parentWnd_ && parentWnd_->GetSafeHwnd())
		parentWnd_->RedrawWindow(drawArea_);
}

void cmComboBoxCell::StoreData()
{
	if (HasFocus()) {
		cmString tmp;
		
		comboBox_->GetWindowText(tmp);
		text_ = tmp;
		comboBox_->GetSelKey(selKey_);

		if (selKey_ != oldSelKey_ && cellListener_) {
			enCmValidationResult ret;

			ret = cellListener_->OnValueChanged(name_, oldText_, text_);

			// if value has been refused keep the old value
			//
			if (ret & enCmValidationResult_Refused) {
				comboBox_->SelectItem(oldSelKey_);
				comboBox_->SetWindowText(oldText_.c_str());
			}

			validationMarker_ = ret;
			if (parentWnd_)
				parentWnd_->RedrawWindow();
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
	//SetFocus(redraw);
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

	if (HasFocus())
		comboBox_->Clear();

	items_.clear();
}
