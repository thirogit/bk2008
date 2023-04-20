// cmCheckBoxCell.h: interface for the CAsBaseCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ASCHECKBOXCELL_H_)
#define _ASCHECKBOXCELL_H_

#include "cmBaseCell.h"
#include "cmString.h"

class cmCheckBoxCell: public cmBaseCell 
{
public:
	cmCheckBoxCell();
	virtual ~cmCheckBoxCell();

	virtual void SetDrawArea(const CRect& drawArea);

	virtual CSize GetMinSize();

	virtual void Draw(CDC* pDC);

	virtual bool HitTest(const CPoint& pt);

	virtual void SetCheckedText(const cmString& checkedText, bool redraw = true);
	virtual cmString GetCheckedText() const;

	virtual void SetUncheckedText(const cmString& uncheckedText, bool redraw = true);
	virtual cmString GetUncheckedText() const;

	virtual bool ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	virtual void KillFocus(bool storeData = true, bool redraw = true);
	virtual void SetFocus(bool redraw = true);

	virtual void SetChecked(bool checked, bool redraw = true);
	virtual bool GetChecked() const;

	virtual void SetReadOnly(bool readOnly);
	virtual bool IsReadOnly() const;

protected:

	cmString checkedText_;
	cmString uncheckedText_;

	CRect iconRect_;
	bool checked_;
	bool readOnly_;

};

#endif // !defined(_ASCHECKBOXCELL_H_)
