// AsOptionRow.h: interface for the cmOptionRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ASOPTIONROW_H_)
#define _ASOPTIONROW_H_

#include "cmBaseCell.h"
#include "cmFontInfo.h"
#include "cmCellEnum.h"

class ICmOptionRowListener;

class cmOptionRow  
{
public:
	cmOptionRow();
	virtual ~cmOptionRow();

	enum enCmOptionRowHitTest {
		enCmOptionRowHitTest_None = 0,
		enCmOptionRowHitTest_Caption,
		enCmOptionRowHitTest_Value,
		enCmOptionRowHitTest_Splitter
	};

	void RegisterListener(ICmOptionRowListener* listener);

	cmBaseCell* GetCaptionCell();
	cmBaseCell* GetValueCell();

	void SetValueCellType(enCmCellType cellType);
	void Draw(CDC* pDC);

	void SetDrawArea(const CRect& drawArea);
	CRect GetDrawArea() const;

	void SetVerticalSplitterPos(int pos);
	int GetVerticalSplitterPos() const;

	void SetParentWindow(CWnd* parentWnd);

	bool ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	static int SplitterArea_;

	void SetFontInfo(const cmString& fontName, int fontSize = 10, bool fontBold = false);
	void SetFontInfo(const cmFontInfo& fontInfo);

	enCmOptionRowHitTest HitTest(const CPoint& pt);

	void Select(bool selection, bool redraw);
	bool IsSelected() const;

	bool HasFocus();

	void SetCaption(const cmString& caption);
	cmString GetCaption() const;

	void SetDescription(const cmString& description);
	cmString GetDescription() const;

	void SetTooltipText(const cmString& tooltip);
	cmString GetTooltipText() const;

	void SetTooltipIcon(UINT iconId);
	UINT GetTooltipIcon() const;

protected:

	cmBaseCell* CreateCell(enCmCellType cellType);
	void UpdateColors();
	
	cmBaseCell captionCell_;
	cmBaseCell* valueCell_;

	CRect drawArea_;

	int verticalSplitterPos_;

	COLORREF borderColor_;

	CWnd* parentWnd_;

	cmFontInfo fontInfo_;

	bool selected_;

	cmString caption_;
	cmString description_;
	cmString tooltipText_;
	UINT tooltipIconId_;

	ICmOptionRowListener* listener_;
};

#endif // !defined(_ASOPTIONROW_H_)
