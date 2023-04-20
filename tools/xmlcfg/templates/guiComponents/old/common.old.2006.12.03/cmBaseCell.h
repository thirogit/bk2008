// cmBaseCell.h: interface for the cmBaseCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMBASECELL_H_)
#define _CMBASECELL_H_

#include "cmFontInfo.h"
#include "ICmCellListener.h"
#include "cmString.h"

class cmBaseCell  
{
public:
	cmBaseCell();
	virtual ~cmBaseCell();

	virtual void SetDrawArea(const CRect& drawArea);
	CRect GetDrawArea() const;

	virtual CSize GetMinSize();

	virtual void Draw(CDC* pDC);
	virtual void DrawText(CDC* pDC);

	virtual bool HitTest(const CPoint& pt);

	//void OnMouseMove(const CPoint& pt);

	virtual void SetVisible(bool visible);
	bool IsVisible() const;

	bool IsHidden();

	virtual void SetText(const cmString& text, bool redraw = true);
	virtual cmString GetText();

	void SetParentWindow(CWnd* parentWnd);

	virtual bool ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	virtual void SetFontInfo(const cmString& fontName, int fontSize = 10, bool fontBold = false);
	virtual void SetFontInfo(const cmFontInfo& fontInfo);

	virtual void KillFocus(bool storeData = true, bool redraw = true);
	virtual void SetFocus(bool redraw);
	virtual bool HasFocus();

	virtual void SetSelection(bool redraw = true);
	virtual void KillSelection(bool redraw = true);
	virtual bool IsSelected();

	void SetBackColor(COLORREF backColor);
	COLORREF GetBackColor() const;

	void SetForeColor(COLORREF foreColor);
	COLORREF GetForeColor() const;

	void SetCellListener(ICmCellListener* listener);

	void SetName(const cmString& name);
	cmString GetName() const;

	void SetLeftMargin(int l);
	void SetTopMargin(int t);
	void SetRightMargin(int r);
	void SetBottomMargin(int b);

	void SetIcon(UINT iconId);

protected:

	cmString text_;

	CRect drawArea_;
	bool visible_;

	CWnd* parentWnd_;

	cmFontInfo fontInfo_;

	COLORREF backColor_;
	COLORREF foreColor_;
	bool hasFocus_;
	bool isSelected_;
	cmString name_;

	enCmValidationResult validationMarker_;

	ICmCellListener* cellListener_;

	int leftMargin_;
	int topMargin_;
	int rightMargin_;
	int bottomMargin_;

	UINT iconId_;
};

#endif // !defined(_CMBASECELL_H_)
