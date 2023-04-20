// cmTimeCell.h: interface for the cmTimeCell class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "cmBaseCell.h"
#include "cmMonthCalCtrl.h"
#include "cmTimePicker.h"

class CDateTimeCtrl;

class cmTimeCell: public cmBaseCell 
{
public:
	cmTimeCell();
	virtual ~cmTimeCell();

	virtual void SetDrawArea(const CRect& drawArea);
	virtual CSize GetMinSize();
	virtual void Draw(CDC* pDC, const CPoint& mousePos);
	virtual bool HitTest(const CPoint& pt);
	virtual void SetVisible(bool visible);
	virtual void SetText(const cmString& text);
	
	virtual COleDateTime GetTime() const;

	virtual void SetTime(const COleDateTime& t, bool redraw = true);
	virtual void SetTime(const SYSTEMTIME& t, bool redraw = true);

	virtual bool ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	virtual void SetFontInfo(const cmFontInfo& fontInfo);

	virtual void KillFocus(bool storeData = true, bool redraw = true);
	virtual void SetFocus(bool redraw = true);

	virtual cmString GetText();
	virtual void SetSelection(bool redraw = true);

	void SetDisplaySeconds(bool displaySeconds);
	bool GetDisplaySeconds() const;

protected:
		
	void StoreData();
	cmString GetTimeStr() const;

	cmTimePicker* timeCtrl_;
	CFont font_;
	
	cmFontInfo fontInfo_;
	COleDateTime time_;
	bool displaySeconds_;
};
