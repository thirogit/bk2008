// cmTimeCell.cpp: implementation of the cmTimeCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmDrawUtils.h"
#include "cmTimeCell.h"
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

cmTimeCell::cmTimeCell():
	timeCtrl_(NULL),
	displaySeconds_(true)
{
	time_.m_status = COleDateTime::DateTimeStatus::null;
}

cmTimeCell::~cmTimeCell()
{
	KillFocus(false);
}

void cmTimeCell::SetDrawArea(const CRect& drawArea)
{
	cmBaseCell::SetDrawArea(drawArea);

	if (hasFocus_) {
		CRect textArea;
		CPoint pt;

		textArea = drawArea_;
		textArea.DeflateRect(0/*6*/, 0, 0, 0);
		parentWnd_->RedrawWindow(drawArea_);

		timeCtrl_->SetWindowPos(&CWnd::wndTop, 
								drawArea_.left, 
								drawArea_.top, 
								drawArea_.Width(), 
								drawArea.Height(), 
								SWP_SHOWWINDOW);
	}
}

CSize cmTimeCell::GetMinSize()
{
	return drawArea_.Size();
}

void cmTimeCell::Draw(CDC* pDC, const CPoint& mousePos)
{
	if (!visible_ || IsHidden())
		return;

	if (time_.GetStatus() != COleDateTime::DateTimeStatus::valid) {
		text_ = _T("");
	} else {
		text_ = GetTimeStr(); 
	}
	cmBaseCell::Draw(pDC, mousePos);
}


bool cmTimeCell::HitTest(const CPoint& pt)
{
	BOOL ret;

	ret = drawArea_.PtInRect(pt);

	return ret == TRUE;
}

void cmTimeCell::SetVisible(bool visible)
{
	cmBaseCell::SetVisible(visible);

	if (visible)
		SetFocus();
	else
		KillFocus(false);
}

void cmTimeCell::SetText(const cmString& text)
{
	cmBaseCell::SetText(text);

	//if (HasFocus())
	///	dateCtrl_->SetWindowText(text.c_str());
}

bool cmTimeCell::ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw)
{
	bool processed;
	bool isSelected;
	bool mouseOver;

	// store isSelected_ flag before the cmBaseCell::ProcessMessage modifies it
	//
	isSelected = isSelected_;
	processed = cmBaseCell::ProcessMessage(msg, wParam, lParam, pt, parentRect, needRedraw);

	mouseOver = (drawArea_.PtInRect(pt) == TRUE);
	switch (msg) {
		
		case WM_LBUTTONDOWN:
			{
				if (isSelected && !isSelected_) {
					// before the message the cell was selected now it is not
					//
					KillFocus(true, false);
				} else if (!isSelected && isSelected_) {
					// before the message the cell was NOT selected now it is
					//
				}

				needRedraw |= (isSelected != isSelected_);
				processed = false;

				/*** OLD 2007.04.07
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
				***/

			}
			break;

		case WM_KEYDOWN:
			{
				if (hasFocus_) {
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
							if (hasFocus_) {
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
			break;

		case WM_LBUTTONDBLCLK:
			{
				BOOL ret;
				bool changed = false;

				ret = drawArea_.PtInRect(pt);
				if (ret) {
					SetFocus(false);
					needRedraw = true;
				} else if (isSelected_) {
					KillSelection(false);
					if (!IsHidden())
						needRedraw = true;
				}
			}

	} // close switch

	return processed;
}

void cmTimeCell::SetFocus(bool redraw)
{
	if (hasFocus_ || readOnly_)
		return;

	cmBaseCell::SetFocus(false);
	
	CPoint pt;
	CRect timeRc(drawArea_);
	
	timeRc.InflateRect(0,0,0,1);
	//assert(timeCtrl_ == NULL);
	if (timeCtrl_ == NULL) {
		//::ReleaseCapture();
		timeCtrl_ = new cmTimePicker();

		timeCtrl_->Create(WS_CHILD | WS_VISIBLE | DTS_TIMEFORMAT | DTS_UPDOWN | WS_TABSTOP | DTS_RIGHTALIGN, timeRc, parentWnd_, 2005);
		
	}
	timeCtrl_->ModifyStyleEx(WS_EX_CLIENTEDGE, WS_EX_CONTROLPARENT /* | WS_EX_NOPARENTNOTIFY*/);
	timeCtrl_->SetOwner(parentWnd_);
	
	// get font info from the top renderer
	//
	/*cmCellBaseRenderer* rend;
	
	rend = GetCurrentRenderer();
	fontInfo_ = rend->GetFontInfo();
*/
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
	timeCtrl_->SetParentCell(this);
	timeCtrl_->SetTime(time_);
	if (displaySeconds_)
		timeCtrl_->SetFormat(_T("HH':'mm':'ss"));
	else
		timeCtrl_->SetFormat(_T("HH':'mm"));
	timeCtrl_->SetFocus();

	SetDrawArea(drawArea_);
	
	if (redraw && parentWnd_)
		parentWnd_->RedrawWindow(drawArea_);
}

void cmTimeCell::SetFontInfo(const cmFontInfo& fontInfo)
{
	fontInfo_ = fontInfo;

//	if (editMode_)
//		dateCtrl_->SetFont(fontInfo_.GetFont());
}

void cmTimeCell::KillFocus(bool storeData, bool redraw)
{	
	if (!hasFocus_)
		return;

	if (storeData)
		StoreData();

	cmBaseCell::KillFocus(storeData, false);

	if (timeCtrl_ == NULL)
		return;

	//UINT lastChar;

	//lastChar = comboBox_->GetLastChar();
	timeCtrl_->DestroyWindow();
	delete timeCtrl_;
	timeCtrl_ = NULL;
	
	if (font_.GetSafeHandle())
		font_.DeleteObject();

	if (redraw && parentWnd_ && parentWnd_->GetSafeHwnd()) {
		//parentWnd_->SendMessage(CMC_LOSTFOCUS, lastChar, 0);
		parentWnd_->RedrawWindow(drawArea_);
	}
	
	parentWnd_->SetFocus();
}

void cmTimeCell::StoreData()
{
	if (hasFocus_) {
		cmString tmp;
		COleDateTime newTime;

		//timeCtrl_->GetTime(newTime);
		timeCtrl_->GetWindowText(text_);
		timeCtrl_->GetTime(newTime);
		if (cellListener_) {
			if (time_ != newTime) {
				bool ret;

				ret = cellListener_->OnValueChanged(this, time_, newTime);
				if (ret) {
					time_ = newTime;
				}
			}
		} else {
			time_ = newTime;
		}
	}
}

cmString cmTimeCell::GetText()
{
	if (hasFocus_)
		StoreData();

	return text_;
}

void cmTimeCell::SetSelection(bool redraw)
{
	cmBaseCell::SetSelection(redraw);
	//SetFocus(redraw);
}

void cmTimeCell::SetTime(const COleDateTime& t, bool redraw)
{
	time_ = t;

	if (redraw && parentWnd_ && parentWnd_->GetSafeHwnd())
		parentWnd_->RedrawWindow(drawArea_);
}

void cmTimeCell::SetTime(const SYSTEMTIME& t, bool redraw)
{
	time_ = t;

	if (redraw && parentWnd_ && parentWnd_->GetSafeHwnd())
		parentWnd_->RedrawWindow(drawArea_);
}

COleDateTime cmTimeCell::GetTime() const
{
	return time_;
}

void cmTimeCell::SetDisplaySeconds(bool displaySeconds)
{
	displaySeconds_ = displaySeconds;
}

bool cmTimeCell::GetDisplaySeconds() const
{
	return displaySeconds_;
}

cmString cmTimeCell::GetTimeStr() const
{
	cmString tmp;

	if (displaySeconds_) 
		tmp.Format(_T("%02d:%02d:%02d"), time_.GetHour(), time_.GetMinute(), time_.GetSecond());
	else
		tmp.Format(_T("%02d:%02d"), time_.GetHour(), time_.GetMinute());

	return tmp;
}