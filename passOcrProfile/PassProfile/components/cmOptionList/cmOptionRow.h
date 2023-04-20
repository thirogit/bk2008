// AsOptionRow.h: interface for the cmOptionRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ASOPTIONROW_H_)
#define _ASOPTIONROW_H_

#include "cmBaseCell.h"
#include "cmCellEnum.h"
#include "cmCellContainer.h"
#include "cmBaseCellInfo.h"

class ICmOptionRowListener;

class cmOptionRow: public cmCellContainer
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

	// implementation of cmCellContainer
	//
	virtual void UpdateCells();


	void RegisterListener(ICmOptionRowListener* listener);

	cmBaseCell* GetCaptionCell();
	cmBaseCell* GetValueCell();

	void SetValueCellType(enCmCellType cellType);
	void Draw(CDC* pDC, const CPoint& mousePos);

	void SetDrawArea(const CRect& drawArea);
	CRect GetDrawArea() const;

	void SetVerticalSplitterPos(int pos);
	int GetVerticalSplitterPos() const;

	void SetParentWindow(CWnd* parentWnd);

	bool ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	static int SplitterArea_;

	enCmOptionRowHitTest HitTest(const CPoint& pt);

	void Select(bool selection, bool redraw);
	bool IsSelected() const;

	bool HasFocus();

	void SetCaption(const cmString& caption);
	cmString GetCaption();

	void SetDescription(const cmString& description);
	cmString GetDescription();

	void SetSplitterColor(COLORREF splitterColor);
	COLORREF GetSplitterColor() const;

	cmBaseCellInfo* GetInfo();

protected:

	cmBaseCell* CreateCell(enCmCellType cellType);
	void UpdateColors();
	
	void SetIcon(enCmCellStatus status, UINT iconId);

	cmBaseCell captionCell_;
	cmBaseCell* valueCell_;

	CRect drawArea_;

	int verticalSplitterPos_;

	COLORREF splitterColor_;
	CWnd* parentWnd_;	

	bool selected_;

	ICmOptionRowListener* listener_;
};

#endif // !defined(_ASOPTIONROW_H_)
