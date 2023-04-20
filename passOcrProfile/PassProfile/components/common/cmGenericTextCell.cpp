// cmGenericTextCell.cpp
//

#include "stdafx.h"
#include "cmGenericTextCell.h"
#include "cmDrawUtils.h"
#include "cmRendererSet.h"
#include "cmGenericText.h"
#include "cmCellDefs.h"
#include "cmCellBaseRenderer.h"
#include <assert.h>

cmGenericTextCell::cmGenericTextCell():
	editBox_(NULL)
{
	
}

cmGenericTextCell::~cmGenericTextCell()
{
	KillFocus(false, false);
}

void cmGenericTextCell::SetDrawArea(const CRect& drawArea)
{
	cmBaseCell::SetDrawArea(drawArea);

	if (hasFocus_) {
		CRect textArea;

		textArea = GetTextBoxArea();
		editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	}
}

CSize cmGenericTextCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmGenericTextCell::Draw(CDC* pDC, const CPoint& mousePos)
{
	//if (visible_ && !IsHidden()) {
		cmBaseCell::Draw(pDC, mousePos);

		if (hasFocus_) {
			if (editBox_)
				editBox_->RedrawWindow();
		}
	//}
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

	if (hasFocus_ && editBox_)
		editBox_->SetWindowText(text.c_str());

	if (parentWnd_ && redraw)
		parentWnd_->RedrawWindow();
}

bool cmGenericTextCell::ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;

	processed = cmBaseCell::ProcessMessage(msg, wParam, lParam, pt, parentRect, needRedraw);
	
	switch (msg) {
		case WM_LBUTTONDOWN: 
			if (hasFocus_)
				editBox_->LockCursor();
			break;

		case WM_KEYDOWN:
			if(isSelected_) {
				
				switch (wParam) {
					case VK_F2:
						SetFocus(false);
						editBox_->SetSel(text_.GetLength(), text_.GetLength());
						editBox_->LockCursor();
						processed = true;
						break;

					case VK_BACK:
						SetFocus(false);
						editBox_->SetWindowText(_T(""));
						processed = true;
						break;

					case VK_DELETE:
						text_.Empty();
						needRedraw = true;
						processed = true;
						break;
				
					default:
						{
							TCHAR c;

							// if WM_KEYDOWN the cell gets the focus
							//
							c = cmBaseCell::VirtualKeyCode2Ascii(wParam, lParam);
							if (IsValidChar(c, _T(""))) {
							
								TCHAR tmp[2] = {c, 0};
								
								SetFocus(false);
								editBox_->SetWindowText(tmp);
								editBox_->SetSel(1,1);	
							}
						}
				}

				
			}
			break;

			case WM_LBUTTONDBLCLK:
			{
				BOOL ret;
				bool changed = false;

				ret = drawArea_.PtInRect(pt);
				if (ret) {
					SetFocus(false);
					if (editBox_)
						editBox_->LockCursor();
					needRedraw = true;
				} else if (isSelected_) {
					KillSelection(false);
					if (!IsHidden())
						needRedraw = true;
				}
			}
			break;
	}

	return processed;
}

void cmGenericTextCell::SetFocus(bool redraw)
{
	if (hasFocus_ || readOnly_)
		return;
	
	cmBaseCell::SetFocus(false);
	
	CRect textArea;

	textArea = GetTextBoxArea();
	
	assert(editBox_ == NULL);
	
	editBox_ = new cmGenericText;
	editBox_->Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, textArea, parentWnd_, -1);
	
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
	
	editBox_->SetFont(&font_);
	editBox_->SetWindowText(text_.c_str());
	editBox_->SetFocus();
	editBox_->SetSel((int)text_.GetLength(), text_.GetLength());
	editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	editBox_->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	editBox_->SetParentCell(this);
	//editBox_->SetMargins(2, 2);

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

void cmGenericTextCell::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;

	if (hasFocus_ && editBox_) {
		
		fontInfo_.CreateFont();
	
		editBox_->SetFont(fontInfo_.GetFont());
	}
}

void cmGenericTextCell::KillFocus(bool storeData, bool redraw)
{	
	if (!hasFocus_)
		return;

	if (storeData)
		StoreData();
	
	cmBaseCell::KillFocus(storeData, false);
	
	if (editBox_ == NULL || !editBox_->GetSafeHwnd())
		return;
	
	UINT lastChar = editBox_->GetLastChar();

	delete editBox_;
	editBox_ = NULL;
		
	if (font_.GetSafeHandle())
		font_.DeleteObject();

	if (redraw && parentWnd_ && parentWnd_->GetSafeHwnd()) {
		parentWnd_->SendMessage(CMC_LOSTFOCUS, lastChar, 0);

		parentWnd_->RedrawWindow(drawArea_);
	}
}

void cmGenericTextCell::SetVisible(bool visible)
{
	cmBaseCell::SetVisible(visible);

	if (hasFocus_ && !visible)
		editBox_->ShowWindow(SW_HIDE);
}

CRect cmGenericTextCell::GetTextBoxArea()
{
	CRect tmp;

	tmp = drawArea_;
	//tmp.DeflateRect(7, 4, 5, 1);
	tmp.DeflateRect(1, 1, 1, 1);
	return tmp;
}

void cmGenericTextCell::StoreData()
{
	if (hasFocus_) {
		cmString tmp;
		cmString oldValue;

		oldValue = text_;
		editBox_->GetWindowText(tmp);
		
		text_ = tmp;

		if (text_ != oldValue && cellListener_) {
			bool ret;

			ret = cellListener_->OnValueChanged(this, oldValue, text_);
			if (!ret) {
				editBox_->SetWindowText(oldValue.c_str());
				text_ = oldValue;
			}
		}
	}
}

cmString cmGenericTextCell::GetText()
{
	//if (HasFocus())
	//	StoreData();

	return text_;
}

