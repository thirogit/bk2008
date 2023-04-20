// CAsGenericTextCell.h
//

#if !defined(_CASGENERICTEXTCELL_H_)
#define _CASGENERICTEXTCELL_H_

#include "cmBaseCell.h"
#include "cmString.h"

class cmGenericText;

class cmGenericTextCell: public cmBaseCell
{
public:
	cmGenericTextCell();
	virtual ~cmGenericTextCell();

	virtual void SetDrawArea(const CRect& drawArea);
	virtual CSize GetMinSize();
	virtual void Draw(CDC* pDC, const CPoint& mousePos);
	virtual bool HitTest(const CPoint& pt);
	virtual void SetVisible(bool visible);
	virtual void SetText(const cmString& text, bool redraw = true);
	
	virtual bool ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	virtual void SetFontInfo(const cmFontInfo& fontInfo);

	virtual void KillFocus(bool storeData = true, bool redraw = true);
	virtual void SetFocus(bool redraw = true);
	
	virtual cmString GetText();

protected:
	
	CRect GetTextBoxArea();
	void StoreData();

	cmGenericText* editBox_;

	CFont font_;
	cmFontInfo fontInfo_;
};

#endif //!defined(_CASGENERICTEXTCELL_H_)