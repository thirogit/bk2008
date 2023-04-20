// AsTableRow.h: interface for the CAsTableRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ASTABLEROW_H_)
#define _ASTABLEROW_H_

#include <vector>
#include "cmCellEnum.h"
#include "cmFontInfo.h"
#include "cmCellContainer.h"
#include "cmString.h"

using namespace std;

class cmBaseCell;
class cmBaseCellInfo;

class cmTableRow : public cmCellContainer
{
public:
	cmTableRow();
	virtual ~cmTableRow();

	enum enCmTableRowHitTest {
		enCmTableRowHitTest_None = 0,
		enCmTableRowHitTest_Splitter,
		enCmTableRowHitTest_FixedColumn,
		enCmTableRowHitTest_NonFixedColumn
	};

	// implementation of cmCellContainer
	//
	virtual void UpdateCells();

	// attributes
	//
	virtual void SetNumColumns(unsigned int numColumns);
	unsigned int GetNumColumns() const;

	void SetDrawArea(const CRect& drawArea, bool forceRecalc);
	CRect GetDrawArea() const;

	void SetParentWnd(CWnd* parentWnd);

	void SetRowHeight(int height);
	int GetRowHeight() const;

	void SetColumnWidthList(const vector<unsigned int>& widthList);
	void SetColumnWidth(unsigned int index, int width);

	void SetFixedColumnsNumber(unsigned int number);
	unsigned int GetFixedColumnsNumber() const;

	void SetFirstNonFixedColIndex(unsigned int index);
	unsigned int GetFirstNonFixedColIndex() const;

	static int SplitterArea_;

	// operations
	//
	void Draw(CDC* pDC, const CPoint& mousePos);

	virtual cmBaseCell* SetCellType(unsigned int index, enCmCellType cellType);
	virtual cmBaseCell* GetCell(unsigned int index);
	virtual cmBaseCell* GetFocusedCell(unsigned int& cellIndex);
	virtual cmBaseCell* GetSelectedCell(unsigned int& cellIndex);
	virtual cmBaseCell* GetSelectedItem(unsigned int& cellIndex);

	enCmTableRowHitTest HitTest(const CPoint& pt, unsigned int& index);

	bool ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	void SetVisible(bool visible);
	bool IsVisible() const;

	void SetGridColor(COLORREF color);
	COLORREF GetGridColor() const;

	void ClearSelection(bool redraw = true);

	void SetSelection(bool select, bool redraw = true);

	void SetColumnsInfo(const vector<cmString>& names, const vector<cmString>& descriptions);

protected:

	void DrawBackground(CDC* pDC);
	void DrawBorders(CDC* pDC);
	void DrawCells(CDC* pDC, const CPoint& mousePos);

	void Clear();

	void SetIcon(cmBaseCell* cell, enCmCellStatus status, UINT iconId);

	unsigned int numColumns_;
	int rowHeight_;
	CRect drawArea_;
	CWnd* parentWnd_;
	cmFontInfo fontInfo_;
	vector<cmBaseCell*> cells_;

	COLORREF gridColor_;

	vector<unsigned int> colWidthList_;
	unsigned int fixedColumnsNumber_;
	unsigned int firstNonFixedColIndex_;

	bool visible_;

	cmBaseCellInfo* info_;
};

#endif // !defined(_ASTABLEROW_H_)
