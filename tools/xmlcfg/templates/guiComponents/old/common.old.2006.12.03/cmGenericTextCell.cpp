// cmGenericTextCell.cpp
//

#include "stdafx.h"
#include "cmGenericTextCell.h"
#include "DrawUtils.h"
#include <assert.h>

cmGenericTextCell::cmGenericTextCell():
	editBox_(NULL)
{
	fontInfo_.SetFontName(_T("courier"));
	fontInfo_.SetFontBold(false);
	fontInfo_.SetFontSize(10);
}

cmGenericTextCell::~cmGenericTextCell()
{
	KillFocus(false, false);
}

void cmGenericTextCell::SetDrawArea(const CRect& drawArea)
{
	cmBaseCell::SetDrawArea(drawArea);

	if (HasFocus()) {
		CRect textArea;

		textArea = GetTextBoxArea();
		editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	}
}

CSize cmGenericTextCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmGenericTextCell::Draw(CDC* pDC)
{
	if (visible_ && !IsHidden()) {
		cmBaseCell::Draw(pDC);

		if (HasFocus()) {
			if (editBox_)
				editBox_->RedrawWindow();
		}
	}
}

bool cmGenericTextCell::HitTest(const CPoint& pt)
{
	BOOL ret;

	ret = drawArea_.PtInRect(pt);

	return ret == TRUE;
}

void cmGenericTextCell::SetText(const cmString& text, bool redraw)
{
	cmBaseCell::SetText(text, false);

	if (HasFocus() && editBox_)
		editBox_->SetWindowText(text.c_str());

	if (parentWnd_ && redraw)
		parentWnd_->RedrawWindow();
}

bool cmGenericTextCell::ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;
	bool selected;
	
	selected = IsSelected();

	processed = cmBaseCell::ProcessMessage(msg, lParam, wParam, pt, parentRect, needRedraw);
	
	// if selection changed after a WM_LBUTTOMDOWON SetFocus
	//
	if (!selected && IsSelected())
		SetFocus(false);

	switch (msg) {
				
		case WM_KEYDOWN:
		{
			if (HasFocus()) {
				switch (wParam) {
					case VK_ESCAPE:
						KillFocus(false, false);
						processed = true;
						needRedraw = true;
						break;

					case VK_DOWN:
					case VK_RETURN:
						KillFocus(true, false);
						processed = true;
						needRedraw = true;
						break;


				}
			} else if (IsSelected()) {
				//cmString tmp;

				//tmp = _TCHAR(wParam);
				
				//if (!GetAsyncKeyState(VK_LSHIFT) || !GetAsyncKeyState(VK_RSHIFT))
				//	tmp.MakeLower();
				/*SetFocus();
				text_ = (const char*) "";
				editBox_->SetWindowText(_T(""));
				editBox_->SendMessage(msg, wParam, lParam);
				processed = true;
				*/
				//editBox_->SetSel((int)text_.length(), text_.length());

				// TODO !!!
			}
			break;
		}
	}

	return processed;
}

void cmGenericTextCell::SetFocus(bool redraw)
{
	TRACE1("Text Set Focus %s\n", text_.c_str());

	if (HasFocus())
		return;
	
	cmBaseCell::SetFocus(false);
	
	CRect textArea;

	textArea = GetTextBoxArea();
	
	assert(editBox_ == NULL);
	
	editBox_ = new CEdit;
	editBox_->Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, textArea, parentWnd_, -1);
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
	
	editBox_->SetFont(&font_);
	editBox_->SetWindowText(text_.c_str());
	editBox_->SetFocus();
	editBox_->SetSel((int)text_.GetLength(), text_.GetLength());
	editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	editBox_->ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

void cmGenericTextCell::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;

	if (HasFocus() && editBox_) {
		
		fontInfo_.CreateFont();
	
		editBox_->SetFont(fontInfo_.GetFont());
	}
}

void cmGenericTextCell::KillFocus(bool storeData, bool redraw)
{	
	TRACE1("Text Kill Focus %s\n", text_.c_str());

	if (!HasFocus())
		return;

	if (storeData)
		StoreData();

	cmBaseCell::KillFocus(storeData, false);
	
	if (editBox_ == NULL || !editBox_->GetSafeHwnd())
		return;
	
	delete editBox_;
	editBox_ = NULL;
		
	if (font_.GetSafeHandle())
		font_.DeleteObject();

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

void cmGenericTextCell::SetVisible(bool visible)
{
	cmBaseCell::SetVisible(visible);

	if (HasFocus() && !visible)
		editBox_->ShowWindow(SW_HIDE);
}

CRect cmGenericTextCell::GetTextBoxArea()
{
	CRect tmp;

	tmp = drawArea_;
	//tmp.DeflateRect(6, 3, 5, 1);
	tmp.DeflateRect(7, 4, 5, 1);
	return tmp;
}

void cmGenericTextCell::StoreData()
{
	if (HasFocus()) {
		cmString tmp;
		cmString oldValue;

		oldValue = text_;
		editBox_->GetWindowText(tmp);
		
		text_ = tmp;

		if (text_ != oldValue && cellListener_) {
			enCmValidationResult ret;

			ret = cellListener_->OnValueChanged(name_, oldValue, text_);
			
			// if value has been refused keep the old value
			//
			if (ret & enCmValidationResult_Refused) {
				editBox_->SetWindowText(oldValue);
				text_ = oldValue;
			} else {
				editBox_->SetWindowText(text_);
			}

			validationMarker_ = ret;
			if (parentWnd_)
				parentWnd_->RedrawWindow();
		}
	}
}

cmString cmGenericTextCell::GetText()
{
	if (HasFocus())
		StoreData();

	return text_;
}
