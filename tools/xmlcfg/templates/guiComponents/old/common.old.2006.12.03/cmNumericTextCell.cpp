// cmNumericTextCell.cpp
//

#include "stdafx.h"
#include <assert.h>
#include "cmNumericTextCell.h"
#include "cmNumericEdit.h"
#include "DrawUtils.h"

cmNumericTextCell::cmNumericTextCell():
	editBox_(NULL),
	minValue_(0),
	maxValue_(0),
	minMaxCheckEnable_(false),
	precision_(1),
	value_(0),
	readOnly_(false)
{

}

cmNumericTextCell::~cmNumericTextCell()
{
	KillFocus(false, false);
}

void cmNumericTextCell::SetDrawArea(const CRect& drawArea)
{
	cmBaseCell::SetDrawArea(drawArea);

	if (HasFocus()) {
		CRect textArea;

		textArea = GetTextBoxArea();
		editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	}
}

CSize cmNumericTextCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmNumericTextCell::Draw(CDC* pDC)
{
	if (!visible_ || IsHidden())
		return;

	cmBaseCell::Draw(pDC);

	if (HasFocus())		
		editBox_->RedrawWindow(drawArea_);
}

bool cmNumericTextCell::HitTest(const CPoint& pt)
{
	BOOL ret;

	ret = drawArea_.PtInRect(pt);

	return ret == TRUE;
}

void cmNumericTextCell::SetVisible(bool visible)
{
	cmBaseCell::SetVisible(visible);

	if (visible)
		SetFocus();
	else
		KillFocus(false);
}

void cmNumericTextCell::SetText(const cmString& text)
{
	cmBaseCell::SetText(text);

	if (HasFocus())
		editBox_->SetWindowText(text);
}

bool cmNumericTextCell::ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
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

void cmNumericTextCell::SetFocus(bool redraw)
{
	if (HasFocus() || readOnly_)
		return;

	cmBaseCell::SetFocus(false);
	
	CRect textArea;

	textArea = GetTextBoxArea();
	assert(editBox_ == NULL);

	editBox_ = new cmNumericEdit;
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
	editBox_->SetPrecision(precision_);
	editBox_->SetMinMaxCheckEnable(minMaxCheckEnable_);
	editBox_->SetWindowText(text_);
	editBox_->SetFocus();
	editBox_->SetValue(value_);
	editBox_->SetSel((int)text_.GetLength(), text_.GetLength());
	editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	editBox_->ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

void cmNumericTextCell::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;

//	if (editMode_)
//		editBox_->SetFont(fontInfo_.GetFont());
}

void cmNumericTextCell::KillFocus(bool storeData, bool redraw)
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


void cmNumericTextCell::SetMinValue(double minValue)
{
	minValue_ = minValue;

	if (editBox_)
		editBox_->SetMinValue(minValue);
}

double cmNumericTextCell::GetMinValue() const
{
	return minValue_;
}

void cmNumericTextCell::SetMaxValue(double maxValue)
{
	maxValue_ = maxValue;

	if (editBox_)
		editBox_->SetMaxValue(maxValue);
}

double cmNumericTextCell::GetMaxValue() const
{
	return maxValue_;
}

void cmNumericTextCell::SetPrecision(int precision)
{
	if (precision < 0)
		precision = 0;

	precision_ = precision;

	if (editBox_)
		editBox_->SetPrecision(precision);
}

int cmNumericTextCell::GetPrecision()
{
	return precision_;
}

void cmNumericTextCell::SetMinMaxCheckEnable(bool enable)
{
	if (editBox_)
		editBox_->SetMinMaxCheckEnable(enable);

	minMaxCheckEnable_ = enable;
}

bool cmNumericTextCell::GetMinMaxCheckEnable() const
{
	return minMaxCheckEnable_;
}

void cmNumericTextCell::SetValue(double value, bool redraw)
{
	cmString newText;

	value = ValidateValue(value);
	
	value_ = value;

	if (editBox_)
		editBox_->SetValue(value);

	newText = FormatValue(value);
	if (text_ != newText) {
		text_ = newText;
		if (parentWnd_ && redraw)
			parentWnd_->RedrawWindow();
	}
}

double cmNumericTextCell::GetValue()
{
	if (HasFocus())
		StoreData();

	return value_;
}

double cmNumericTextCell::ValidateValue(double value)
{
	if (minMaxCheckEnable_) {
		value = min(value, maxValue_);
		value = max(value, minValue_);
	}
	
	return value;
}

cmString cmNumericTextCell::FormatValue(double value)
{
	cmString tmp;
	cmString fmt;

	fmt.Format(_T("%%1.%df"), precision_);
	tmp.Format(fmt, value);

	return tmp;
}

CRect cmNumericTextCell::GetTextBoxArea()
{
	CRect tmp;

	tmp = drawArea_;
	//tmp.DeflateRect(6, 3, 5, 1);
	tmp.DeflateRect(7, 4, 5, 1);

	return tmp;
}

void cmNumericTextCell::StoreData()
{
	if (HasFocus()) {
		cmString tmp;
		double oldValue;
		
		oldValue = value_;
		editBox_->Validate();
		editBox_->GetWindowText(tmp);
		text_ = tmp;
		value_ = editBox_->GetValue();

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

void cmNumericTextCell::SetReadOnly(bool readOnly)
{
	readOnly_ = readOnly;
	KillFocus(true, true);
}

bool cmNumericTextCell::IsReadOnly() const
{
	return readOnly_;
}