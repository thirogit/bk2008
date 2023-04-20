// cmNumericTextCell.cpp
//

#include "stdafx.h"
#include <assert.h>
#include "cmSmartNumericTextCell.h"
#include "cmSmartNumericEdit.h"
#include "DrawUtils.h"

cmSmartNumericTextCell::cmSmartNumericTextCell():
	editBox_(NULL),
	minValue_(0),
	maxValue_(0),
	minMaxCheckEnable_(false),
	value_(0),
	readOnly_(false),
	hexDisplayMode_(false)
{

}

cmSmartNumericTextCell::~cmSmartNumericTextCell()
{
	KillFocus(false, false);
}

void cmSmartNumericTextCell::SetDrawArea(const CRect& drawArea)
{
	cmBaseCell::SetDrawArea(drawArea);

	if (HasFocus()) {
		CRect textArea;

		textArea = GetTextBoxArea();
		editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	}
}

CSize cmSmartNumericTextCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmSmartNumericTextCell::Draw(CDC* pDC)
{
	if (!visible_ || IsHidden())
		return;

	cmBaseCell::Draw(pDC);

	if (HasFocus())		
		editBox_->RedrawWindow(drawArea_);
}

bool cmSmartNumericTextCell::HitTest(const CPoint& pt)
{
	BOOL ret;

	ret = drawArea_.PtInRect(pt);

	return ret == TRUE;
}

void cmSmartNumericTextCell::SetVisible(bool visible)
{
	cmBaseCell::SetVisible(visible);

	if (visible)
		SetFocus();
	else
		KillFocus(false);
}

void cmSmartNumericTextCell::SetText(const cmString& text)
{
	cmString tmp;

	tmp = ValidateText(text);
	
	cmBaseCell::SetText(tmp);

	if (HasFocus())
		editBox_->SetWindowText(tmp);
}

bool cmSmartNumericTextCell::ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;
	
	bool selected;
	
	selected = IsSelected();

	// if selection changed after a WM_LBUTTOMDOWON SetFocus
	//
	if (!selected && IsSelected())
		SetFocus(false);

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

void cmSmartNumericTextCell::SetFocus(bool redraw)
{
	if (HasFocus() || readOnly_)
		return;

	cmBaseCell::SetFocus(false);
	
	CRect textArea;

	textArea = GetTextBoxArea();
	assert(editBox_ == NULL);

	editBox_ = new cmSmartNumericEdit;
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
					 fontInfo_.GetFontName());
		
	editBox_->SetFont(&font_);
	editBox_->SetMinValue(minValue_);
	editBox_->SetMaxValue(maxValue_);
	editBox_->SetMinMaxCheckEnable(minMaxCheckEnable_);
	editBox_->SetWindowText(text_);
	editBox_->SetFocus();
	editBox_->SetHexDisplayMode(hexDisplayMode_);
	editBox_->SetValue(value_);
	editBox_->SetSel((int)text_.GetLength(), text_.GetLength());
	editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	editBox_->ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

void cmSmartNumericTextCell::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;

//	if (editMode_)
//		editBox_->SetFont(fontInfo_.GetFont());
}

void cmSmartNumericTextCell::KillFocus(bool storeData, bool redraw)
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


void cmSmartNumericTextCell::SetMinValue(long minValue)
{
	minValue_ = minValue;

	if (editBox_)
		editBox_->SetMinValue(minValue);
}

long cmSmartNumericTextCell::GetMinValue() const
{
	return minValue_;
}

void cmSmartNumericTextCell::SetMaxValue(long maxValue)
{
	maxValue_ = maxValue;

	if (editBox_)
		editBox_->SetMaxValue(maxValue);
}

long cmSmartNumericTextCell::GetMaxValue() const
{
	return maxValue_;
}

void cmSmartNumericTextCell::SetMinMaxCheckEnable(bool enable)
{
	if (editBox_)
		editBox_->SetMinMaxCheckEnable(enable);

	minMaxCheckEnable_ = enable;
}

bool cmSmartNumericTextCell::GetMinMaxCheckEnable() const
{
	return minMaxCheckEnable_;
}

void cmSmartNumericTextCell::SetValue(long value, bool redraw)
{
	cmString newText;

	value = ValidateValue(value);
	
	value_ = value;

	if (editBox_)
		editBox_->SetValue(value);

	newText = FormatValue(value, hexDisplayMode_);
	if (text_ != newText) {
		text_ = newText;
		if (parentWnd_ && redraw)
			parentWnd_->RedrawWindow();
	}
}

long cmSmartNumericTextCell::GetValue()
{
	if (HasFocus())
		StoreData();

	return value_;
}

long cmSmartNumericTextCell::ValidateValue(long value)
{
	if (minMaxCheckEnable_) {
		value = min(value, maxValue_);
		value = max(value, minValue_);
	}
	
	return value;
}

cmString cmSmartNumericTextCell::FormatValue(long value, bool hexDisplay)
{
	cmString tmp;
	cmString fmt;

	if (hexDisplay)
		tmp.Format(_T("0x%X"), value);
	else
		tmp.Format(_T("%d"), value);

	return tmp;
}

CRect cmSmartNumericTextCell::GetTextBoxArea()
{
	CRect tmp;

	tmp = drawArea_;
	//tmp.DeflateRect(6, 3, 5, 1);
	tmp.DeflateRect(7, 4, 5, 1);

	return tmp;
}

void cmSmartNumericTextCell::StoreData()
{
	if (HasFocus()) {
		cmString tmp;
		long oldValue;
		
		oldValue = value_;
		editBox_->Validate();
		editBox_->GetWindowText(tmp);
		text_ = tmp;
		value_ = editBox_->GetValue();
		hexDisplayMode_ = editBox_->GetHexDisplayMode();

		if (value_ != oldValue && cellListener_) {
			enCmValidationResult ret;

			ret = cellListener_->OnValueChanged(name_, oldValue, value_);

			// if value has been refused keep the old value
			//
			if (ret & enCmValidationResult_Refused) {
				editBox_->SetWindowText(FormatValue(oldValue, hexDisplayMode_));
				text_ = FormatValue(oldValue, hexDisplayMode_);
			} else {
				editBox_->SetWindowText(FormatValue(value_, hexDisplayMode_));
				text_ = FormatValue(value_, hexDisplayMode_);
			}

			validationMarker_ = ret;
			if (parentWnd_)
				parentWnd_->RedrawWindow();
		}
	}
}

void cmSmartNumericTextCell::SetReadOnly(bool readOnly)
{
	readOnly_ = readOnly;
	KillFocus(true, true);
}

bool cmSmartNumericTextCell::IsReadOnly() const
{
	return readOnly_;
}

void cmSmartNumericTextCell::SetHexDisplayMode(bool hexDisplay)
{
	hexDisplayMode_ = hexDisplay;
}

bool cmSmartNumericTextCell::GetHexDisplayMode() const
{
	return hexDisplayMode_;
}

cmString cmSmartNumericTextCell::ValidateText(const cmString& text)
{
	long value;
	
	if (text.IsHexadecimalNumber(_T("0x"))) {
		hexDisplayMode_ = true;
		value = text.HexStringToNumber(_T("0x"));
	} else if (text.IsHexadecimalNumber(_T("0X"))) {
		hexDisplayMode_ = true;
		value = text.HexStringToNumber(_T("0X"));
	} else if (text.IsHexadecimalNumber(_T(""))) {
		hexDisplayMode_ = true;
		value = text.HexStringToNumber(_T(""));
	} else if (text.IsDecimalNumber()) {
		hexDisplayMode_ = false;
		value = (long)text;
	} else {
		hexDisplayMode_ = false;
		value = 0;
	}
		
	value = ValidateValue(value);

	return FormatValue(value);
}