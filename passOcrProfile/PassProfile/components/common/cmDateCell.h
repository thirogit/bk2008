// cmDateCell.h: interface for the cmDateCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMDATECELL_H_)
#define _CMDATECELL_H_

#include "cmBaseCell.h"
#include "cmMonthCalCtrl.h"

class CDateTimeCtrl;

class cmDateCell: public cmBaseCell 
{
public:
	cmDateCell();
	virtual ~cmDateCell();

	virtual void SetDrawArea(const CRect& drawArea);
	virtual CSize GetMinSize();
	virtual void Draw(CDC* pDC, const CPoint& mousePos);
	virtual bool HitTest(const CPoint& pt);
	virtual void SetVisible(bool visible);
	virtual void SetText(const cmString& text);
	
	virtual COleDateTime GetDate() const;

	virtual void SetDate(const COleDateTime& t, bool redraw = true);
	virtual void SetDate(const SYSTEMTIME& t, bool redraw = true);

	virtual bool ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	virtual void SetFontInfo(const cmFontInfo& fontInfo);

	virtual void KillFocus(bool storeData = true, bool redraw = true);
	virtual void SetFocus(bool redraw = true);

	virtual cmString GetText();
	virtual void SetSelection(bool redraw = true);

protected:
	
	void DrawCalendarButton(CDC* pDC);
	void StoreData();
	CRect GetComboBoxArea();

	cmMonthCalCtrl* monthCal_;
	CFont font_;
	
	cmFontInfo fontInfo_;
	COleDateTime date_;

	CWnd desktopWindow_;
};

#endif // !defined(_CMDATECELL_H_)
