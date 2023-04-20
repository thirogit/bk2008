// cmNumericTextCell.cpp
//

#include "stdafx.h"
#include <assert.h>
#include "cmSmartNumericTextCell.h"
#include "cmSmartNumericEdit.h"
#include "cmCellDefs.h"
#include "cmCellBaseRenderer.h"
#include "cmDrawUtils.h"

cmSmartNumericTextCell::cmSmartNumericTextCell():
	editBox_(NULL),
	minValue_(0),
	maxValue_(0),
	minMaxCheckEnable_(false),
	value_(0),
	hexDisplayMode_(false),
	digits_(0),
	hexPrefix_(_T("0x"))
{

}

cmSmartNumericTextCell::~cmSmartNumericTextCell()
{
	KillFocus(false, false);
}

void cmSmartNumericTextCell::SetDrawArea(const CRect& drawArea)
{
	cmBaseCell::SetDrawArea(drawArea);

	if (hasFocus_) {
		CRect textArea;

		textArea = GetTextBoxArea();
		editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	}
}

CSize cmSmartNumericTextCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmSmartNumericTextCell::Draw(CDC* pDC, const CPoint& mousePos)
{
	if (!visible_ || IsHidden())
		return;

	cmBaseCell::Draw(pDC, mousePos);

	if (hasFocus_)		
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

	if (hasFocus_)
		editBox_->SetWindowText(tmp);
}

bool cmSmartNumericTextCell::ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;
	
	processed = cmBaseCell::ProcessMessage(msg, wParam, lParam, pt, parentRect, needRedraw);
	//processed = false;
	switch (msg) {
		case WM_LBUTTONDOWN:
		{
			if (hasFocus_)
				editBox_->LockCursor();
			break;
		}
		
		case WM_KEYDOWN:
		{
			if(isSelected_) {
				TCHAR c;

				hexDisplayMode_ = text_.IsHexadecimalNumber();
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
						if (editBox_) {
							if (hexDisplayMode_) {
								editBox_->SetWindowText(hexPrefix_);
								editBox_->SetSel(hexPrefix_.GetLength(), hexPrefix_.GetLength());
							} else {
								editBox_->SetWindowText(_T(""));
							}
						}
						processed = true;
						break;

					case VK_DELETE:
						SetValue(0, false);
						if (hexDisplayMode_) {
							text_ = cmString::FormatHexNumber(hexPrefix_, digits_, 0);
						} else {
							text_ = cmString::FormatDecimalNumber(digits_, 0, 0);
						}
						needRedraw = true;
						processed = true;
						break;
				
					case VK_UP:
					case VK_DOWN:
					case VK_LEFT:
					case VK_RIGHT:
						break;

					default:
						{
							// automatic insertion of prefix with 1 char
							//
							int pos;

							c = cmBaseCell::VirtualKeyCode2Ascii(wParam, lParam);
							if (c) {
								pos = hexPrefix_.Find(c);
								if (pos != -1) {
									SetFocus(false);
									if (editBox_) {
										editBox_->SetWindowText(hexPrefix_);
										editBox_->SetSel(hexPrefix_.GetLength(), hexPrefix_.GetLength());	
									}
								} else if (IsValidChar(c, _T(""), -1)) {
									TCHAR tmp[2] = {c, 0};
									
									SetFocus(false);
									if (editBox_) {
										if (hexDisplayMode_) {
											editBox_->SetWindowText(hexPrefix_ + tmp);
											editBox_->SetSel(hexPrefix_.GetLength() + 1, hexPrefix_.GetLength() + 1);
										} else {
											editBox_->SetWindowText(tmp);
											editBox_->SetSel(1,1);	
										}
									}
								}
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
	}

	return processed;
}

void cmSmartNumericTextCell::SetFocus(bool redraw)
{
	if (hasFocus_ || readOnly_)
		return;

	cmBaseCell::SetFocus(false);
	
	CRect textArea;

	textArea = GetTextBoxArea();
	assert(editBox_ == NULL);

	hexDisplayMode_ = text_.IsHexadecimalNumber();

	editBox_ = new cmSmartNumericEdit;
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
	editBox_->SetMinMaxCheckEnable(minMaxCheckEnable_);
	editBox_->SetWindowText(text_);
	editBox_->SetFocus();
	editBox_->SetHexDisplayMode(hexDisplayMode_);
	editBox_->SetValue(value_);
	editBox_->SetSel((int)text_.GetLength(), text_.GetLength());
	editBox_->SetWindowPos(&CWnd::wndTop, textArea.left, textArea.top, textArea.Width(), textArea.Height(), SWP_SHOWWINDOW);
	editBox_->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	editBox_->SetParentCell(this);
	editBox_->SetHexPrefix(hexPrefix_);

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

	if (redraw && parentWnd_) {
		parentWnd_->SendMessage(CMC_LOSTFOCUS, lastChar, 0);
		parentWnd_->RedrawWindow(drawArea_);
	}
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

	newText = FormatValue(value);
	if (text_ != newText) {
		text_ = newText;
		if (parentWnd_ && redraw)
			parentWnd_->RedrawWindow();
	}
}

long cmSmartNumericTextCell::GetValue()
{
	if (hasFocus_)
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
	cmString ret;

	if (hexDisplay)
		ret = cmString::FormatHexNumber(hexPrefix_, digits_, value, false);
	else
		ret = cmString::FormatDecimalNumber(digits_, 0, value);

	return ret;
}

CRect cmSmartNumericTextCell::GetTextBoxArea()
{
	CRect tmp;

	tmp = drawArea_;
	//tmp.DeflateRect(7, 4, 5, 1);
	tmp.DeflateRect(1, 1, 1, 1);

	return tmp;
}

void cmSmartNumericTextCell::StoreData()
{
	if (hasFocus_) {
		CString tmp;
		long oldValue;
		
		oldValue = value_;

		editBox_->Validate();
		value_ = editBox_->GetValue();
		text_ = FormatValue(value_, editBox_->GetHexDisplayMode());

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
	
	if (text.IsHexadecimalNumber(hexPrefix_)) {
		hexDisplayMode_ = true;
		value = text.HexStringToNumber(hexPrefix_);
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

bool cmSmartNumericTextCell::IsValidChar(TCHAR c, const cmString& text, int cursorPos)
{
	int pos;
	
	// does text contain hexPrefix_ ?
	//
	if (HexPrefixFound(text))
		return cmString::IsHexChar(c);

	pos = hexPrefix_.Find(c);
	if (pos != -1 || cmString::IsDecChar(c))
		return true;

	return false;
}

void cmSmartNumericTextCell::SetDigits(int digits)
{
	digits_ = digits;
}

int cmSmartNumericTextCell::GetDigits() const
{
	return digits_;
}

void cmSmartNumericTextCell::SetHexPrefix(const cmString& prefix)
{
	hexPrefix_ = prefix;
}

cmString cmSmartNumericTextCell::GetHexPrefix() const
{
	return hexPrefix_;
}

bool cmSmartNumericTextCell::HexPrefixFound(const cmString& text)
{
	int pos;

	pos = text.Find(hexPrefix_);

	return (pos != -1);
}
