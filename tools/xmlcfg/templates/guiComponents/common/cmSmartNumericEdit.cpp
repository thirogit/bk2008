// AsNumericEdit.cpp : implementation file
//

#include "stdafx.h"
#include "cmSmartNumericEdit.h"
#include "cmSmartNumericTextCell.h"
#include "cmBaseCell.h"
#include <assert.h>

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

/////////////////////////////////////////////////////////////////////////////
// cmNumericEdit

cmSmartNumericEdit::cmSmartNumericEdit():
	minMaxCheckEnable_(false),
	minValue_(0),
	maxValue_(0),
	hexDisplayMode_(false),
	digits_(0)
{
}

cmSmartNumericEdit::~cmSmartNumericEdit()
{
}


BEGIN_MESSAGE_MAP(cmSmartNumericEdit, CEdit)
	//{{AFX_MSG_MAP(cmSmartNumericEdit)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cmNumericEdit message handlers

BOOL cmSmartNumericEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		int startPos;
		int lastPos;
		cmString tmp;

		GetSel(startPos, lastPos);
		GetWindowText(tmp);
		lastChar_ = pMsg->wParam;
		switch (lastChar_) {
			case VK_DELETE:
				{
				if (hexDisplayMode_) {
						// character deletion: check if deleting a 'x' or 'X'
						//
						if (startPos == 1) { // convert to decimal number

							tmp = ConvertToDec(tmp);
							SetWindowText(tmp);
							SetSel(startPos, 0);
							hexDisplayMode_ = false;

							return TRUE;
						}
					}
				}
				return cmGenericText::PreTranslateMessage(pMsg);

			case VK_BACK:
				{
					if (hexDisplayMode_) {
						// character deletion: check if deleting a 'x' or 'X'
						//
						if (startPos == 2) { // convert to decimal number

							tmp = ConvertToDec(tmp);
							SetWindowText(tmp);
							SetSel(1, 0);
							hexDisplayMode_ = false;

							return TRUE;
						}
					}
				}
				return cmGenericText::PreTranslateMessage(pMsg);

			case VK_ESCAPE:
				
				assert(parentCell_);
				if (parentCell_)
					parentCell_->KillFocus(false);

				return TRUE;
				

			case VK_LEFT:
				if (cursorLocked_)
					return cmGenericText::PreTranslateMessage(pMsg);

			case VK_RIGHT:
				if (cursorLocked_)
					return cmGenericText::PreTranslateMessage(pMsg);

			case VK_RETURN:
			case VK_UP:
			case VK_DOWN:
				assert(parentCell_);
				if (parentCell_)
					parentCell_->KillFocus(true);
				
				return TRUE;
		}

		cmSmartNumericTextCell* parent;

		parent = dynamic_cast<cmSmartNumericTextCell*> (parentCell_);
		assert(parent);
		if (parent == NULL)
			return TRUE;

		if (!parent->IsValidChar(cmBaseCell::VirtualKeyCode2Ascii(pMsg->wParam, pMsg->lParam), tmp, startPos))
			return TRUE;
	}
	
	return cmGenericText::PreTranslateMessage(pMsg);
} 

void cmSmartNumericEdit::SetMinValue(long minValue)
{
	minValue_ = minValue;
}

long cmSmartNumericEdit::GetMinValue() const
{
	return minValue_;
}

void cmSmartNumericEdit::SetMaxValue(long maxValue)
{
	maxValue_ = maxValue;
}

long cmSmartNumericEdit::GetMaxValue() const
{
	return maxValue_;
}

void cmSmartNumericEdit::SetMinMaxCheckEnable(bool enable)
{
	minMaxCheckEnable_ = enable;
}

bool cmSmartNumericEdit::GetMinMaxCheckEnable() const
{
	return minMaxCheckEnable_;
}

long cmSmartNumericEdit::GetValue()
{
	Validate();
	
	return GetNumericValue();
}

long cmSmartNumericEdit::Validate()
{
	cmString tmp;
	long value;

	value = GetNumericValue();
	
	value = ValidateValue(value);

	tmp = FormatValue(value, hexDisplayMode_);
		
	SetWindowText(tmp);

	return value;
}

void cmSmartNumericEdit::OnKillfocus() 
{
	Validate();	
}

long cmSmartNumericEdit::ValidateString(const cmString& valueStr)
{
	long value;

	value = GetValue();

	return ValidateValue(value);
}

long cmSmartNumericEdit::ValidateValue(long value)
{
	if (minMaxCheckEnable_) {
		value = min(value, maxValue_);
		value = max(value, minValue_);
	}

	return value;
}

cmString cmSmartNumericEdit::FormatValue(long value, bool hexDisplayMode)
{
	cmString tmp;
	
	if (hexDisplayMode)
		tmp = cmString::FormatHexNumber(hexPrefix_, digits_, value);
	else
		tmp = cmString::FormatDecimalNumber(digits_, 0, value);

	return tmp;
}

void cmSmartNumericEdit::SetValue(long value)
{
	value = ValidateValue(value);

	SetWindowText(FormatValue(value, hexDisplayMode_));
}

void cmSmartNumericEdit::SetHexDisplayMode(bool hexDisplay)
{
	hexDisplayMode_ = hexDisplay;
}

bool cmSmartNumericEdit::GetHexDisplayMode() const
{
	return hexDisplayMode_;
}

/***
cmString cmSmartNumericEdit::ConvertToHex(const cmString& txt)
{
	DWORD value;
	cmString ret;
	
	value = (long)txt;

	ret.Format(_T("0x%X"), value);
	
	return ret;
}
***/

cmString cmSmartNumericEdit::ConvertToDec(const cmString& txt)
{
	DWORD value;
	cmString ret;

	if (txt.GetLength() <= 2)
		return _T("0");

	value = txt.HexStringToNumber(hexPrefix_);
	
	ret = cmString::FormatDecimalNumber(digits_, 0, value);
	
	return ret;
}

long cmSmartNumericEdit::GetNumericValue()
{
	cmString tmp;

	GetWindowText(tmp);

	hexDisplayMode_ = tmp.IsHexadecimalNumber(hexPrefix_);
	if (hexDisplayMode_)
		tmp = ConvertToDec(tmp);

	return tmp;
}

BOOL cmSmartNumericEdit::ProcessChar(UINT nChar)
{
	cmString tmp;
	int startPos;
	int lastPos;
	
	GetWindowText(tmp);
	GetSel(startPos, lastPos);
	
	hexDisplayMode_ = ((tmp.Find('X') != -1) || (tmp.Find('x') != -1));

	if (nChar >= _T('0') && nChar <= '9')
		return FALSE;

	if (nChar == VK_BACK) {
	
		if (hexDisplayMode_) {
			// character deletion: check if deleting a 'x' or 'X'
			//
			if (startPos <= 2) { // convert to decimal number
	
				tmp = ConvertToDec(tmp);
				SetWindowText(tmp);
				SetSel(startPos, 0);
				hexDisplayMode_ = false;

				return TRUE;
			}
		}

		return FALSE;
	}
	
	if (nChar == VK_DELETE) {
		if (hexDisplayMode_) {
			// character deletion: check if deleting a 'x' or 'X'
			//
			if (startPos < 2) { // convert to decimal number
	
				tmp = ConvertToDec(tmp);
				SetWindowText(tmp);
				SetSel(startPos, 0);
				hexDisplayMode_ = false;

				return TRUE;
			}
		}

		return FALSE;
	}

	if (nChar == VK_LEFT || nChar == VK_RIGHT)
		return FALSE;

	if (nChar == VK_RETURN) {
		Validate();
		return FALSE;
	} 
	
	if ((nChar == _T('x') || nChar == _T('X')) && !hexDisplayMode_) {
		// check: x is allowed only if cursor is at index 0 or 1(and at 0 there is a '0')
		//
		if (startPos > 1)
			return TRUE;

		if (startPos == 1 && tmp.GetAt(0) != _T('0'))
			return TRUE;

		if (startPos == 1) {
			if (tmp.GetLength() > 1)
				tmp = _T("0x") + tmp.Mid(startPos);
			else
				tmp = _T("0x");
		} else {
			tmp = _T("0x") + tmp;
		}

		hexDisplayMode_ = true;
		SetWindowText(tmp);
		SetSel(2, 2, TRUE);

		return TRUE;
		
	} 
	
	if (hexDisplayMode_ && 
		((nChar >= _T('a') && nChar <= _T('f')) || 
		 (nChar >= _T('A') && nChar <= _T('F')))) {
		return FALSE;
		
	}
	
	// for any other invalid char
	//
	return TRUE;
}

void cmSmartNumericEdit::SetHexPrefix(const cmString& prefix)
{
	hexPrefix_ = prefix;
}

cmString cmSmartNumericEdit::GetHexPrefix() const
{
	return hexPrefix_;
}

void cmSmartNumericEdit::SetDigits(int digits)
{
	digits_ = digits;
}

int cmSmartNumericEdit::GetDigits() const
{
	return digits_;
}