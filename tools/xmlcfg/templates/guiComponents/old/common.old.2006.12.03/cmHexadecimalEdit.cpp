// AsHexadecimalEdit.cpp : implementation file
//

#include "stdafx.h"
#include "cmHexadecimalEdit.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

/////////////////////////////////////////////////////////////////////////////
// cmHexadecimalEdit

cmHexadecimalEdit::cmHexadecimalEdit():
	bitCount_(32)
{
}

cmHexadecimalEdit::~cmHexadecimalEdit()
{
}


BEGIN_MESSAGE_MAP(cmHexadecimalEdit, CEdit)
	//{{AFX_MSG_MAP(CAsNumericEdit)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAsNumericEdit message handlers

void cmHexadecimalEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar >= '0' && nChar <= '9' ||
			   nChar >= 'a' && nChar <= 'f' ||
			   nChar >= 'A' && nChar <= 'F' ||
			   nChar == VK_BACK ||
			   nChar == VK_LEFT	||
			   nChar == VK_RIGHT ||
			   nChar == VK_DELETE ) {
		
	} else if (nChar == VK_RETURN) {
		Validate();
	} else {
		// any other invalid char
		//
		return;
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


void cmHexadecimalEdit::SetBitCount(long bitCount)
{
	if (bitCount < 1)
		bitCount = 1;
	else if (bitCount > 32)
		bitCount = 32;

	bitCount_ = bitCount;
}

long cmHexadecimalEdit::GetBitCount() const
{
	return bitCount_;
}

long cmHexadecimalEdit::GetValue()
{
	return Validate();
}

long cmHexadecimalEdit::Validate()
{
	cmString tmp;
	long value;

	GetWindowText(tmp);

	value = ValidateString(tmp);

	tmp = FormatValue(value);
		
	SetWindowText(tmp);

	return value;
}

void cmHexadecimalEdit::OnKillfocus() 
{
	Validate();	
}

long cmHexadecimalEdit::ValidateString(const cmString& valueStr)
{
	long value;

	if (valueStr.GetLength()) {
#ifdef _UNICODE
		swscanf((const TCHAR*)valueStr, _T("%08X"), &value);
#else
		sscanf((const TCHAR*)valueStr, _T("%08X"), &value);
#endif
	} else {
		value = 0;
	}
	return ValidateValue(value);
}

long cmHexadecimalEdit::ValidateValue(long value)
{
	value &= (0xffffffff >> (32 - bitCount_));
	
	return value;
}

cmString cmHexadecimalEdit::FormatValue(long value)
{
	cmString tmp;
	
	value = ValidateValue(value);
	tmp.Format(_T("%08X"), value);

	return tmp;
}

void cmHexadecimalEdit::SetValue(long value)
{
	value = ValidateValue(value);

	SetWindowText(FormatValue(value));
}
