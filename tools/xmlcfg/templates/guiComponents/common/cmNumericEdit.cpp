// cmNumericEdit.cpp : implementation file
//

#include "stdafx.h"
#include "cmNumericEdit.h"
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

cmNumericEdit::cmNumericEdit():
	minMaxCheckEnable_(false),
	minValue_(0),
	maxValue_(0),
	precision_(1),
	digits_(1),
	decimalSeparator_('.')
{
}

cmNumericEdit::~cmNumericEdit()
{
}


BEGIN_MESSAGE_MAP(cmNumericEdit, cmGenericText)
	//{{AFX_MSG_MAP(cmNumericEdit)
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_WM_GETDLGCODE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cmNumericEdit message handlers

void cmNumericEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void cmNumericEdit::SetMinValue(double minValue)
{
	minValue_ = minValue;
}

double cmNumericEdit::GetMinValue() const
{
	return minValue_;
}

void cmNumericEdit::SetMaxValue(double maxValue)
{
	maxValue_ = maxValue;
}

double cmNumericEdit::GetMaxValue() const
{
	return maxValue_;
}

void cmNumericEdit::SetPrecision(int precision)
{
	if (precision < 0)
		precision = 0;

	precision_ = precision;
}

int cmNumericEdit::GetPrecision() const
{
	return precision_;
}

void cmNumericEdit::SetMinMaxCheckEnable(bool enable)
{
	minMaxCheckEnable_ = enable;
}

bool cmNumericEdit::GetMinMaxCheckEnable() const
{
	return minMaxCheckEnable_;
}

double cmNumericEdit::GetValue()
{
	Validate();

	cmString tmp;

	GetWindowText(tmp);
	
	return tmp;
}

double cmNumericEdit::Validate()
{
	cmString tmp;
	double value;

	GetWindowText(tmp);

	value = ValidateString(tmp);

	tmp = FormatValue(value);
		
	SetWindowText(tmp);

	return value;
}

void cmNumericEdit::OnKillfocus() 
{
	Validate();	
}

double cmNumericEdit::ValidateString(const cmString& valueStr)
{
	return ValidateValue(valueStr);
}

double cmNumericEdit::ValidateValue(double value)
{
	if (minMaxCheckEnable_) {
		value = min(value, maxValue_);
		value = max(value, minValue_);
	}

	return value;
}

cmString cmNumericEdit::FormatValue(double value)
{
	cmString tmp;
	cmString fmt;

	fmt.Format(_T("%%%d.%df"), digits_, precision_);
	tmp.Format(fmt, value);

	return tmp;
}

void cmNumericEdit::SetValue(double value)
{
	value = ValidateValue(value);

	SetWindowText(FormatValue(value));
}

void cmNumericEdit::SetDigits(int digits)
{
	if (digits < 0)
		digits = 0;

	digits_ = digits;	
}

int cmNumericEdit::GetDigits() const
{
	return digits_;
}

void cmNumericEdit::SetDecimalSeparator(TCHAR decimalSeparator)
{
	decimalSeparator_ = decimalSeparator;
}

TCHAR cmNumericEdit::GetDecimalSeparator() const
{
	return decimalSeparator_;
}

UINT cmNumericEdit::OnGetDlgCode() 
{
	return DLGC_WANTARROWS | DLGC_WANTALLKEYS | DLGC_WANTCHARS;
}

BOOL cmNumericEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN) {
		lastChar_ = pMsg->wParam;
		switch (lastChar_) {
			case VK_DELETE:
			case VK_BACK:
				return cmGenericText::PreTranslateMessage(pMsg);

			case VK_ESCAPE:
				
				assert(parentCell_);
				if (parentCell_)
					parentCell_->KillFocus(false);

				return cmGenericText::PreTranslateMessage(pMsg);
				

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
					parentCell_->KillFocus(true, true);
				
				return TRUE;
		}

		cmString tmp;

		GetWindowText(tmp);
		if (!parentCell_->IsValidChar(cmBaseCell::VirtualKeyCode2Ascii(pMsg->wParam, pMsg->lParam), tmp))
			return TRUE;
	}
	
	return cmGenericText::PreTranslateMessage(pMsg);
}

void cmNumericEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	cmGenericText::OnLButtonDblClk(nFlags, point);
}
