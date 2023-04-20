// AsTableRow.h: interface for the CAsTableRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ASTABLEROW_H_)
#define _ASTABLEROW_H_

#include <vector>
#include "cmTableCellEnum.h"
#include "cmFontInfo.h"

using namespace std;

class cmBaseCell;

class cmTableRow  
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

	// attributes
	//
	void SetNumColumns(unsigned int numColumns);
	unsigned int GetNumColumns() const;

	void SetDrawArea(const CRect& drawArea, bool forceRecalc);
	CRect GetDrawArea() const;

	void SetParentWnd(CWnd* parentWnd);

	void SetRowHeight(int height);
	int GetRowHeight() const;

	void SetColumnWidthList(const vector<int>& widthList);
	void SetColumnWidth(unsigned int index, int width);

	void SetFixedColumnsNumber(unsigned int number);
	unsigned int GetFixedColumnsNumber() const;

	void SetFirstNonFixedColIndex(unsigned int index);
	unsigned int GetFirstNonFixedColIndex() const;

	static int SplitterArea_;

	// operations
	//
	void Draw(CDC* pDC);

	cmBaseCell* CreateCell(unsigned int index, enCmTableCellType cellType);
	cmBaseCell* GetCell(unsigned int index);
	cmBaseCell* GetFocusedCell(unsigned int& cellIndex);
	cmBaseCell* GetSelectedCell(unsigned int& cellIndex);

	enCmTableRowHitTest HitTest(const CPoint& pt, unsigned int& index);

	bool ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	void SetVisible(bool visible);
	bool IsVisible() const;

	
protected:

	void DrawBackground(CDC* pDC);
	void DrawBorders(CDC* pDC);
	void DrawCells(CDC* pDC);

	void Clear();

	unsigned int numColumns_;
	int rowHeight_;
	CRect drawArea_;
	CWnd* parentWnd_;
	cmFontInfo fontInfo_;
	vector<cmBaseCell*> cells_;

	COLORREF borderColor_;

	vector<int> colWidthList_;
	unsigned int fixedColumnsNumber_;
	unsigned int firstNonFixedColIndex_;

	bool visible_;
};

#endif // !defined(_ASTABLEROW_H_)
