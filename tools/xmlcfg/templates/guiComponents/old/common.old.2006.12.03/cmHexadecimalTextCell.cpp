// cmHexadecimalTextCell.cpp
//

#include "stdafx.h"
#include <assert.h>
#include "cmHexadecimalTextCell.h"
#include "cmHexadecimalEdit.h"
#include "DrawUtils.h"

cmHexadecimalTextCell::cmHexadecimalTextCell():
	editBox_(NULL),
	bitCount_(32),
	value_(0),
	readOnly_(false)
{

}

cmHexadecimalTextCell::~cmHexadecimalTextCell()
{
	KillFocus(false, false);
}

void cmHexadecimalTextCell::SetDrawArea(const CRect& drawArea)
{
	cmBaseCell::SetDrawArea(drawArea);

	if (HasFocus()) {
		CRect textArea;

		textArea = GetTextBoxArea();
		editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	}
}

CSize cmHexadecimalTextCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmHexadecimalTextCell::Draw(CDC* pDC)
{
	if (!visible_ || IsHidden())
		return;

	cmBaseCell::Draw(pDC);

	if (HasFocus())
		editBox_->RedrawWindow(drawArea_);
}

bool cmHexadecimalTextCell::HitTest(const CPoint& pt)
{
	BOOL ret;

	ret = drawArea_.PtInRect(pt);

	return ret == TRUE;
}

void cmHexadecimalTextCell::SetVisible(bool visible)
{
	cmBaseCell::SetVisible(visible);

	if (visible)
		SetFocus();
	else
		KillFocus(false);
}

void cmHexadecimalTextCell::SetText(const cmString& text)
{
	cmBaseCell::SetText(text);

	if (HasFocus())
		editBox_->SetWindowText(text);
}

bool cmHexadecimalTextCell::ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;
	
	processed = cmBaseCell::ProcessMessage(msg, lParam, wParam, pt, parentRect, needRedraw);
	//processed = false;
	switch (msg) {
		case WM_LBUTTONDOWN:
		{
			BOOL ret;

			ret = parentRect.PtInRect(drawArea_.TopLeft()) && drawArea_.PtInRect(pt);
			if (ret)
				SetFocus();
			else
				KillFocus(true);
			break;
		}
		
		case WM_KEYDOWN:
		{
			if (HasFocus()) {
				switch (wParam) {
					case VK_ESCAPE:
						KillFocus(false, false);
						processed = true;
						needRedraw = true;
						break;

					case VK_RETURN:
						KillFocus(true, false);
						processed = true;
						needRedraw = true;
						break;
				}
			}
		}
	}

	return processed;
}

void cmHexadecimalTextCell::SetFocus(bool redraw)
{
	if (HasFocus() || readOnly_)
		return;

	cmBaseCell::SetFocus(false);
	
	CRect textArea;

	textArea = GetTextBoxArea();
	assert(editBox_ == NULL);

	editBox_ = new cmHexadecimalEdit;
	editBox_->Create(WS_CHILD | WS_VISIBLE, textArea, parentWnd_, -1);
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
					 fontInfo_.GetFontName());
		
	editBox_->SetFont(&font_);
	editBox_->SetBitCount(bitCount_);
	//editBox_->SetWindowText(text_.c_str());
	editBox_->SetFocus();
	editBox_->SetValue(value_);
	editBox_->SetSel((int)text_.GetLength(), text_.GetLength());
	editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	editBox_->SetMargins(0, 0);
	editBox_->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	text_ = _T("0x");
	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

void cmHexadecimalTextCell::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;

//	if (editMode_)
//		editBox_->SetFont(fontInfo_.GetFont());
}

void cmHexadecimalTextCell::KillFocus(bool storeData, bool redraw)
{	
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


void cmHexadecimalTextCell::SetBitCount(long bitCount)
{
	if (bitCount < 1)
		bitCount = 1;
	else if (bitCount > 32)
		bitCount = 32;

	bitCount_ = bitCount;

	if (editBox_)
		editBox_->SetBitCount(bitCount);
}

long cmHexadecimalTextCell::GetBitCount()
{
	return bitCount_;
}

void cmHexadecimalTextCell::SetValue(long value, bool redraw)
{
	cmString newText;

	value = ValidateValue(value);
	
	value_ = value;

	if (editBox_)
		editBox_->SetValue(value);

	newText = HasFocus() ? _T("0x") : FormatValue(value);
	
	if (text_ != newText) {
		text_ = newText;
		if (parentWnd_ && redraw)
			parentWnd_->RedrawWindow();
	}
}

long cmHexadecimalTextCell::GetValue()
{
	if (HasFocus())
		StoreData();

	return value_;
}

long cmHexadecimalTextCell::ValidateValue(long value)
{
	value &= (0xffffffff >> (32 - bitCount_));
	
	return value;
}

cmString cmHexadecimalTextCell::FormatValue(long value)
{
	cmString tmp;
	
	tmp.Format(_T("0x%08X"), value);
	
	return tmp;
}

CRect cmHexadecimalTextCell::GetTextBoxArea()
{
	CRect tmp;

	tmp = drawArea_;
	//tmp.DeflateRect(7, 4, 5, 1);
	tmp.DeflateRect(22, 4, 5, 1);
	return tmp;
}

void cmHexadecimalTextCell::StoreData()
{
	if (HasFocus()) {
		CString tmp;
		long oldValue;
		
		oldValue = value_;

		editBox_->Validate();
		value_ = editBox_->GetValue();
		text_ = FormatValue(value_);

		if (value_ != oldValue && cellListener_) {
			enCmValidationResult ret;

			ret = cellListener_->OnValueChanged(name_, oldValue, value_);

			// if value has been refused keep the old value
			//
			if (ret & enCmValidationResult_Refused) {
				editBox_->SetWindowText(FormatValue(oldValue));
				text_ = FormatValue(oldValue);
			} else {
				editBox_->SetWindowText(FormatValue(value_));
				text_ = FormatValue(value_);
			}

			validationMarker_ = ret;
			if (parentWnd_)
				parentWnd_->RedrawWindow();
		}
	}
}

void cmHexadecimalTextCell::SetReadOnly(bool readOnly)
{
	readOnly_ = readOnly;
	KillFocus(true, true);
}

bool cmHexadecimalTextCell::IsReadOnly() const
{
	return readOnly_;
}