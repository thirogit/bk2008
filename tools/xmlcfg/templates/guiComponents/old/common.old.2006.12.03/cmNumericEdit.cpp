// AsNumericEdit.cpp : implementation file
//

#include "stdafx.h"
#include "cmNumericEdit.h"

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
	decimalSeparator_('.')
{
}

cmNumericEdit::~cmNumericEdit()
{
}


BEGIN_MESSAGE_MAP(cmNumericEdit, CEdit)
	//{{AFX_MSG_MAP(cmNumericEdit)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cmNumericEdit message handlers

void cmNumericEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((char)nChar == decimalSeparator_) {
		cmString tmp;
		int pos;

		if (precision_ == 0)	// precision_ == 0 means integer value
			return;

		GetWindowText(tmp);

		pos = tmp.Find(decimalSeparator_);
		if (pos != -1)
			return;

	} else if (nChar == '-') {
		cmString tmp;

		GetWindowText(tmp);

		if (minMaxCheckEnable_ && minValue_ >= 0)
			return;

	} else if (nChar >= '0' && nChar <= '9' ||
			   nChar == VK_BACK ||
			   nChar == VK_LEFT	||
			   nChar == VK_RIGHT ||
			   nChar == VK_DELETE) {
		
	} else if (nChar == VK_RETURN) {
		Validate();
	} else {
		// any other invalid char
		//
		return;
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
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

	fmt.Format(_T("%%1.%df"), precision_);
	tmp.Format(fmt, value);

	return tmp;
}

void cmNumericEdit::SetValue(double value)
{
	value = ValidateValue(value);

	SetWindowText(FormatValue(value));
}
