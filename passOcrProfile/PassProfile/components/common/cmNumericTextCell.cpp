// cmNumericTextCell.cpp
//

#include "stdafx.h"
#include <assert.h>
#include "cmNumericTextCell.h"
#include "cmNumericEdit.h"
#include "cmCellDefs.h"
#include "cmCellBaseRenderer.h"
#include "cmDrawUtils.h"

cmNumericTextCell::cmNumericTextCell():
	editBox_(NULL),
	minValue_(0),
	maxValue_(0),
	minMaxCheckEnable_(false),
	precision_(1),
	value_(0),
	digits_(1),
	decimalSeparator_(_T('.'))
{
}

cmNumericTextCell::~cmNumericTextCell()
{
	KillFocus(false, false);
}

void cmNumericTextCell::SetDrawArea(const CRect& drawArea)
{
	cmBaseCell::SetDrawArea(drawArea);

	if (hasFocus_) {
		CRect textArea;

		textArea = GetTextBoxArea();
		editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	}
}

CSize cmNumericTextCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmNumericTextCell::Draw(CDC* pDC, const CPoint& mousePos)
{
	if (!visible_ || IsHidden())
		return;

	cmBaseCell::Draw(pDC, mousePos);

	if (hasFocus_)		
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

	if (hasFocus_)
		editBox_->SetWindowText(text);
}

bool cmNumericTextCell::ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
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
				TCHAR c;

				switch (wParam) {
					case VK_F2:
						SetFocus(false);
						if (editBox_) {
							editBox_->SetSel(text_.GetLength(), text_.GetLength());
							editBox_->LockCursor();
						}
						processed = true;
						break;

					case VK_BACK:
						SetFocus(false);
						if (editBox_)
							editBox_->SetWindowText(_T(""));
						processed = true;
						break;

					case VK_DELETE:
						SetValue(0, false);
						needRedraw = true;
						processed = true;
						break;
				
					default:
						c = cmBaseCell::VirtualKeyCode2Ascii(wParam, lParam);
						if (IsValidChar(c, _T(""))) {
							TCHAR tmp[2] = {c, 0};
							
							SetFocus(false);
							if (editBox_) {
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

void cmNumericTextCell::SetFocus(bool redraw)
{
	if (hasFocus_ || readOnly_)
		return;

	cmBaseCell::SetFocus(false);
	
	CRect textArea;

	textArea = GetTextBoxArea();
	assert(editBox_ == NULL);

	editBox_ = new cmNumericEdit;
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
	editBox_->SetParentCell(this);
	//editBox_->SetMargins(2, 2);	

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
	if (!hasFocus_)
		return;
	
	if (storeData)
		StoreData();
	
	cmBaseCell::KillFocus(storeData, false);

	if (editBox_ == NULL || !editBox_->GetSafeHwnd())
		return;

	UINT lastChar = editBox_->GetLastChar();

	editBox_->DestroyWindow();
	delete editBox_;
	editBox_ = NULL;
	
	if (font_.GetSafeHandle())
		font_.DeleteObject();

	if (redraw && parentWnd_) {
		parentWnd_->SendMessage(CMC_LOSTFOCUS, lastChar, 0);
		parentWnd_->RedrawWindow(drawArea_);
	}
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

int cmNumericTextCell::GetPrecision() const
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
	if (hasFocus_)
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
	
	//tmp.DeflateRect(7, 4, 5, 1);
	tmp.DeflateRect(1, 1, 1, 1);

	return tmp;
}

void cmNumericTextCell::StoreData()
{
	if (hasFocus_) {
		cmString tmp;
		double oldValue;
		
		oldValue = value_;
		editBox_->Validate();
		editBox_->GetWindowText(tmp);
		text_ = tmp;
		value_ = editBox_->GetValue();

		if (value_ != oldValue && cellListener_) {
			bool ret;

			ret = cellListener_->OnValueChanged(this, oldValue, value_);
			if (!ret) {
				editBox_->SetWindowText(FormatValue(oldValue));
				text_ = FormatValue(oldValue);
			}
		}
	}
}

void cmNumericTextCell::SetDigits(int digits)
{
	if (digits < 0)
		digits = 0;

	digits_ = digits;

	if (editBox_)
		editBox_->SetDigits(digits);
}

int cmNumericTextCell::GetDigites() const
{
	return digits_;
}

bool cmNumericTextCell::IsValidChar(TCHAR c, const cmString& text)
{
	if (c == decimalSeparator_) {
		int pos;

		if (precision_ == 0)	// precision_ == 0 means integer value -> separator not allowed
			return false;

		pos = text.Find(decimalSeparator_);
		if (pos != -1)
			return false;

		return true;
	} 
	
	if (c == _T('-')) {
		int pos;

		pos = text.Find(decimalSeparator_);
		if (pos != -1)
			return false;

		if (minMaxCheckEnable_ && minValue_ >= 0)
			return false;

		return true;

	} 
	
	if (c >= '0' && c <= '9')
		return true;

	return false;
}

void cmNumericTextCell::SetDecimalSeparator(TCHAR decimalSeparator)
{
	decimalSeparator_ = decimalSeparator;
}

TCHAR cmNumericTextCell::GetDecimalSeparator() const
{
	return decimalSeparator_;
}

bool cmNumericTextCell::IsDecimalSeparator(TCHAR c)
{
	return c == decimalSeparator_;
}