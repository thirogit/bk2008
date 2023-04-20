// AsBinaryWildCardCell.cpp: implementation for the CAsBinaryWildCard class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "cmBinaryWildCardsCell.h"
#include "cmDrawUtils.h"
#include "resource.h"

#define BIT_SIZE			16
#define CHK_LEFT_BORDER		6
#define CHK_TOP_BORDER		2

cmBinaryWildCardsCell::cmBinaryWildCardsCell():
	readOnly_(false),
	bitCount_(8)
{
}

cmBinaryWildCardsCell::~cmBinaryWildCardsCell()
{}

void cmBinaryWildCardsCell::SetDrawArea(const CRect& drawArea)
{
	if (!visible_)
		return;

	CRect tmp;

	tmp = drawArea;
	
	if (tmp.Height() < 2 * CHK_TOP_BORDER + BIT_SIZE)
		tmp.bottom = tmp.top + 2 * CHK_TOP_BORDER + BIT_SIZE;

	if (tmp.Width() < 2 * CHK_LEFT_BORDER + BIT_SIZE)
		tmp.right = tmp.left + 2 * CHK_LEFT_BORDER + BIT_SIZE;
	
	// calculate draw area for each bit
	//
	bitRect_.clear();

	tmp.left = drawArea.left;
	tmp.top = drawArea.top;
	tmp.bottom = drawArea.bottom;
	for (int i = 0; i < bitCount_; i++) {
		tmp.right = tmp.left + BIT_SIZE;

		bitRect_.push_back(tmp);

		tmp.OffsetRect(BIT_SIZE + 1, 0);
	}	
	
	cmBaseCell::SetDrawArea(tmp);
}

CSize cmBinaryWildCardsCell::GetMinSize()
{
	CSize minSize;

	minSize.cx = 2 * CHK_LEFT_BORDER + (BIT_SIZE + 1) * bitCount_;
	minSize.cy = 2 * CHK_TOP_BORDER + BIT_SIZE;

	return minSize;
}

void cmBinaryWildCardsCell::Draw(CDC* pDC, const CPoint& mousePos)
{
	if (!visible_ || IsHidden())
		return;

	cmBaseCell::Draw(pDC, mousePos);

	/***
	CFont* font;

	font = fontInfo_.CreateFont();
	
	for (int i = 0; i < bitRect_.size(); i++) {
		DrawUtils::DrawGradient(pDC, bitRect_[i], RGB(255,0,0), RGB(100,100,100), .5);
	}

	fontInfo_.ReleaseFont();
	***/
}

bool cmBinaryWildCardsCell::HitTest(const CPoint& pt)
{
	return cmBaseCell::HitTest(pt);
}

bool cmBinaryWildCardsCell::ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	cmBaseCell::ProcessMessage(msg, wParam, lParam, pt, parentRect, needRedraw);

	/*
	if (msg == WM_LBUTTONUP && iconRect_.PtInRect(pt) && !readOnly_) {
		SetChecked(!checked_);
	}
	*/

	return false;
}

void cmBinaryWildCardsCell::KillFocus(bool storeData, bool redraw)
{
	cmBaseCell::KillFocus(storeData, redraw);
}

void cmBinaryWildCardsCell::SetFocus(bool redraw)
{
	cmBaseCell::SetFocus(redraw);
}

void cmBinaryWildCardsCell::SetReadOnly(bool readOnly)
{
	readOnly_ = readOnly;
}

bool cmBinaryWildCardsCell::IsReadOnly() const
{
	return readOnly_;
}

void cmBinaryWildCardsCell::SetBitCount(long bitCount)
{
	bitCount_ = bitCount;
}

long cmBinaryWildCardsCell::GetBitCount() const
{
	return bitCount_;
}

void cmBinaryWildCardsCell::SetBitStatus(long bitIndex, enCmBitStatus status, bool redraw)
{
	if (bitIndex < 0 || bitIndex >= bitCount_)
		return;

	bitStatus_[bitIndex] = status;

	if (redraw)
		parentWnd_->RedrawWindow(drawArea_);
}

cmBinaryWildCardsCell::enCmBitStatus cmBinaryWildCardsCell::GetBitStatus(long bitIndex) const
{
	if (bitIndex < 0 || bitIndex >= bitCount_)
		return enCmBitStatus_Unchecked;

	return bitStatus_[bitIndex];
}

void cmBinaryWildCardsCell::SetBinaryString(const cmString& checkedText, bool redraw)
{

}

cmString cmBinaryWildCardsCell::GetBinaryString() const
{
	return binaryString_;
}

void cmBinaryWildCardsCell::SetValue(long value, bool redraw)
{

}

long cmBinaryWildCardsCell::GetValue() const
{
	return value_;
}
