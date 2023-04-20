// AsHexadecimalEdit.cpp : implementation file
//

#include "stdafx.h"
#include "cmHexadecimalEdit.h"
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
// cmHexadecimalEdit

cmHexadecimalEdit::cmHexadecimalEdit():
	bitCount_(32)
{
}

cmHexadecimalEdit::~cmHexadecimalEdit()
{
}

BEGIN_MESSAGE_MAP(cmHexadecimalEdit, cmGenericText)
	//{{AFX_MSG_MAP(CAsNumericEdit)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAsNumericEdit message handlers

void cmHexadecimalEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	
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

	value = valueStr.HexStringToNumber(_T(""));

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

BOOL cmHexadecimalEdit::PreTranslateMessage(MSG* pMsg) 
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
					parentCell_->KillFocus(true);
				
				return TRUE;
		}

		cmString tmp;

		GetWindowText(tmp);
		if (!parentCell_->IsValidChar(cmBaseCell::VirtualKeyCode2Ascii(pMsg->wParam, pMsg->lParam), tmp))
			return TRUE;
	}
	
	return cmGenericText::PreTranslateMessage(pMsg);
}

