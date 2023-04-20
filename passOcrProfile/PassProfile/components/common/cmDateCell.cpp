// cmDateCell.cpp: implementation of the cmDateCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmDrawUtils.h"
#include "cmDateCell.h"
#include "cmCellBaseRenderer.h"
#include "resource.h"
#include <afxdtctl.h>
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmDateCell::cmDateCell():
	monthCal_(NULL)
{
	date_.m_status = COleDateTime::DateTimeStatus::null;
}

cmDateCell::~cmDateCell()
{
	KillFocus(false);
}


void cmDateCell::SetDrawArea(const CRect& drawArea)
{
	cmBaseCell::SetDrawArea(drawArea);

	if (hasFocus_) {
		CRect textArea;
		CRect comboArea;
		CRect monthRc;
		CPoint pt;

		textArea = drawArea_;
		textArea.DeflateRect(0/*6*/, 0, 0, 0);
		parentWnd_->RedrawWindow(drawArea_);

		comboArea = GetComboBoxArea();
		pt.x = comboArea.right - comboArea.Width();
		pt.y = comboArea.bottom + 1;
		parentWnd_->ScreenToClient(&pt);
		monthCal_->GetWindowRect(monthRc);
		monthCal_->SetWindowPos(&CWnd::wndTop, pt.x, pt.y, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
	}
}

CSize cmDateCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmDateCell::Draw(CDC* pDC, const CPoint& mousePos)
{
	if (!visible_ || IsHidden())
		return;

	if (date_.GetStatus() == COleDateTime::DateTimeStatus::valid) {
		text_ = date_.Format(VAR_DATEVALUEONLY);
	} else {
		text_ = _T("");
	}
	cmBaseCell::Draw(pDC, mousePos);

	DrawCalendarButton(pDC);
}


void cmDateCell::DrawCalendarButton(CDC* pDC)
{
	if (isSelected_) {
	
		CPoint iconTopLeft;
		CRect clientRect;

		clientRect = drawArea_;
		
		iconTopLeft.x = drawArea_.right - 16 - 2;
		iconTopLeft.y = drawArea_.top + 1;

		cmDrawUtils::DrawIcon(pDC, IDI_CALENDAR, iconTopLeft, 16);

	}
	//DrawUtils::DrawString(pDC, text_, clientRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE, ::GetSysColor(COLOR_BTNTEXT), &font_);
}


bool cmDateCell::HitTest(const CPoint& pt)
{
	BOOL ret;

	ret = drawArea_.PtInRect(pt);

	return ret == TRUE;
}

void cmDateCell::SetVisible(bool visible)
{
	cmBaseCell::SetVisible(visible);

	if (visible)
		SetFocus();
	else
		KillFocus(false);
}

void cmDateCell::SetText(const cmString& text)
{
	cmBaseCell::SetText(text);

	//if (HasFocus())
	///	dateCtrl_->SetWindowText(text.c_str());
}

bool cmDateCell::ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;
	bool isSelected;
	bool mouseOver;

	isSelected = isSelected_;
	//processed = false;
	processed = cmBaseCell::ProcessMessage(msg, wParam, lParam, pt, parentRect, needRedraw);

	mouseOver = (drawArea_.PtInRect(pt) == TRUE);
	switch (msg) {
		
		case WM_LBUTTONDOWN:
			{
				if (HasFocus() && mouseOver) {
					KillFocus(false, false);
					//processed = true;
					needRedraw = true;
				} else if ((isSelected_ == isSelected) && mouseOver) {
					SetFocus(false);
					//processed = true;
					needRedraw = true;
				} else {
					KillFocus(false, false);
				}

				/***
				if ((isSelected_ == isSelected) && drawArea_.PtInRect(pt)) {
					SetFocus(false);
					processed = true;
					needRedraw = true;
				} else if (!isSelected_) {
					KillFocus(false, false);
				}
				***/
			}
			break;

		case WM_KEYDOWN:
			{
				if (HasFocus()) {
					switch (wParam) {
						case VK_ESCAPE:
							KillFocus(false, false);
							processed = false;
							needRedraw = true;
							break;

						case VK_RETURN:
							KillFocus(true, false);
							processed = false;
							needRedraw = true;
							break;

						case VK_UP:
						case VK_DOWN:
							//if (dateCtrl_)
							//	dateCtrl_->SendMessage(msg, wParam, lParam);
							processed = true;
							needRedraw = true;
							break;
					}
				} if (IsSelected()) {
					switch (wParam) {
						case VK_DOWN:
							if (GetAsyncKeyState(VK_LCONTROL)) {
								SetFocus();
								processed = true;
								needRedraw = true;
							}
							break;
						case VK_UP:
							if (HasFocus()) {
								processed = true;
								needRedraw = true;
							}
							break;

						case VK_F2:
							SetFocus();
							processed = true;
							needRedraw = true;
							break;
					}
				}
			}
			break;

		case WM_COMMAND:
			{
				if (lParam == CBN_SELCHANGE) {
					StoreData();
				}
			}
	} // close switch

	return processed;
}

void cmDateCell::SetFocus(bool redraw)
{
	if (HasFocus() || readOnly_)
		return;

	cmBaseCell::SetFocus(false);
	
	CRect comboArea;
	CPoint pt;
	CRect monthRc;

	comboArea = GetComboBoxArea();
	
	assert(monthCal_ == NULL);
	
	pt.x = comboArea.left;
	pt.y = comboArea.bottom + 1;

	parentWnd_->ClientToScreen(&pt);	
	monthCal_ = new  cmMonthCalCtrl();

	if (!desktopWindow_.GetSafeHwnd())
		desktopWindow_.Attach(::GetDesktopWindow());

	monthCal_->Create(WS_CHILD | WS_BORDER, pt, &desktopWindow_, -1);
	monthCal_->SetOwner(parentWnd_);
			
	// get font info from the top renderer
	//
	cmCellBaseRenderer* rend;
	
	rend = GetCurrentRenderer();
	fontInfo_ = rend->GetFontInfo();

	//font_.CreateFont(-fontInfo_.GetFontSize(), 
	//				 0,										/* width */ 
	//				 0,										/* escapement */
	//				 0,										/* orientation */
	//				 fontInfo_.GetFontBold() ? FW_BOLD : FW_NORMAL,		/* weight*/
	//				 0,										/* italic*/
	//				 0,										/* underline */
	//				 0,										/* strikeout */
	//				 ANSI_CHARSET,							/* charset */
	//				 OUT_DEFAULT_PRECIS,					/* out precision */
	//				 CLIP_DEFAULT_PRECIS,					/* clip precision */
	//				 DEFAULT_QUALITY,						/* quality */
	//				 DEFAULT_PITCH,							/* pitch family */
	//				 fontInfo_.GetFontName().c_str());
	

	//monthCal_->SetFont(&font_);
	monthCal_->GetWindowRect(monthRc);
	pt.x = comboArea.right - monthRc.Width();
	pt.y = comboArea.bottom + 1;
	parentWnd_->ClientToScreen(&pt);
	monthCal_->SetFocus();
	monthCal_->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	monthCal_->SetParentCell(this);
	monthCal_->SetDate(date_);
	monthCal_->GetWindowRect(monthRc);
	monthCal_->SetWindowPos(&CWnd::wndTop, pt.x, pt.y, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
	
	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

CRect cmDateCell::GetComboBoxArea()
{
	CRect tmp;

	tmp = drawArea_;
	//tmp.DeflateRect(1, 1, 1, 1);
	

	return tmp;
}

void cmDateCell::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;

//	if (editMode_)
//		dateCtrl_->SetFont(fontInfo_.GetFont());
}

void cmDateCell::KillFocus(bool storeData, bool redraw)
{	
	if (!hasFocus_)
		return;

	if (storeData)
		StoreData();

	cmBaseCell::KillFocus(storeData, false);

	if (monthCal_ == NULL)
		return;

	//UINT lastChar;

	//lastChar = comboBox_->GetLastChar();
	monthCal_->DestroyWindow();
	delete monthCal_;
	monthCal_ = NULL;

	if (desktopWindow_.GetSafeHwnd())
		desktopWindow_.Detach();
		
	if (font_.GetSafeHandle())
		font_.DeleteObject();

	if (redraw && parentWnd_ && parentWnd_->GetSafeHwnd()) {
		//parentWnd_->SendMessage(CMC_LOSTFOCUS, lastChar, 0);
		parentWnd_->RedrawWindow(drawArea_);
	}
	
	parentWnd_->SetFocus();
}

void cmDateCell::StoreData()
{
	if (HasFocus()) {
		cmString tmp;
		COleDateTime newDate;

		newDate = monthCal_->GetDate();
		
		/***
		if (newDateTime.GetStatus() == COleDateTime::DateTimeStatus::valid) {
			text_ = dateTime_.Format(VAR_DATEVALUEONLY);
		} else {
			text_ = _T("");
		}
		***/

		if (cellListener_) {
			if (date_ != newDate) {
				bool ret;

				ret = cellListener_->OnValueChanged(this, date_, newDate);
				if (ret) {
					date_ = newDate;
				}
			}
		} else {
			date_ = newDate;
		}
	}
}

cmString cmDateCell::GetText()
{
	if (HasFocus())
		StoreData();

	return text_;
}

void cmDateCell::SetSelection(bool redraw)
{
	cmBaseCell::SetSelection(redraw);
	//SetFocus(redraw);
}

void cmDateCell::SetDate(const COleDateTime& t, bool redraw)
{
	date_ = t;

	if (redraw && parentWnd_ && parentWnd_->GetSafeHwnd())
		parentWnd_->RedrawWindow(drawArea_);
}

void cmDateCell::SetDate(const SYSTEMTIME& t, bool redraw)
{
	date_ = t;

	if (redraw && parentWnd_ && parentWnd_->GetSafeHwnd())
		parentWnd_->RedrawWindow(drawArea_);
}

COleDateTime cmDateCell::GetDate() const
{
	return date_;
}