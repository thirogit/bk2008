// cmTable.h : header file
//
#if !defined(_CM_TABLE_H_)
#define _CM_TABLE_H_

#include <vector>

using namespace std;

class cmTableRow;
class cmBaseCell;

/////////////////////////////////////////////////////////////////////////////
// cmTable window

class cmTable : public CWnd
{
// Construction / destruction
public:
	cmTable();
	virtual ~cmTable();

// Attributes
public:
	void SetFixedRowsNumber(unsigned int number);
	unsigned int GetFixedRowsNumber() const;

	void SetFixedColumnsNumber(unsigned int number);
	unsigned int GetFixedColumnsNumber() const;

	void SetColumnWidth(unsigned int index, int width);
	int GetColumnWidth(unsigned int index);

	void SetColumnsNumber(unsigned int number, int width = 80);
	unsigned int GetColumnsNumber() const;

	cmBaseCell* GetSelectedCell(unsigned int& row, unsigned int& col);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmTable)
	public:
	virtual BOOL Create(const CRect& rc, CWnd* parentWnd, DWORD style, UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	cmTableRow* AppendRow(unsigned int numColumns, bool redraw = true);
	cmTableRow* InsertRow(unsigned int index, unsigned int numColumns, bool redraw = true);
	cmTableRow* GetRow(unsigned int index);
	void DeleteRow(unsigned int index, bool redraw = true);
	unsigned int GetRowCount() const;
	void DeleteAll();
	void DeleteNonFixedRows();
	bool GetSelectedItem(cmTableRow** row, cmBaseCell** cell, unsigned int& rowIndex, unsigned int& cellIndex);

	// Generated message map functions
protected:
	//{{AFX_MSG(cmTable)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

	void Clear();
	void UpdateWindowSize(bool redraw = true);
	void UpdateFirstNonFixedRowIndex();
	void UpdateFirstNonFixedColumnIndex();
	void UpdateDrawArea();

	void DrawBackground(CDC* pDC);
	void DrawBorders(CDC* pDC);
	void DrawRows(CDC* pDC);
	void DrawSelectedCell(CDC* pDC);

	unsigned int GetNonFixedRowsPerPage();
	unsigned int GetNonFixedColumnsPerPage();
	
	CRect GetFixedRowsArea();
	CRect GetFixedColsArea();
	int HitSplitter(const CPoint& point);

	bool MoveSelection(int horizontalShift, int verticalShift, bool redraw = true);
	bool GetFocusedItem(cmTableRow** row, cmBaseCell** cell, unsigned int& rowIndex, unsigned int& cellIndex);
	void SetCellFocus(bool redraw = true);
	void KillCellFocus(bool storeData);
	void EnsureVisible(unsigned int rowIndex, unsigned int cellIndex, bool redraw = true);

	vector<cmTableRow*> rows_;

	COLORREF bkColor_;
	COLORREF borderColor_;
	int defaultRowHeight_;
	int defaultCellWidth_;

	int horizontalOffset_;
	int rowsNumber_;
	unsigned int fixedRowsNumber_;
	unsigned int columnsNumber_;
	unsigned int fixedColumnsNumber_;

	unsigned int firstNonFixedRowIndex_;
	unsigned int firstNonFixedColumnIndex_;

	int vScrollMinPos_;
	int vScrollMaxPos_;
	int vScrollPageSize_;

	int hScrollMinPos_;
	int hScrollMaxPos_;
	int hScrollPageSize_;

	vector<int> colWidth_;

	int selSplitterIndex_;
	bool draggingSplitter_;
	int splitterRowIndex_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CM_TABLE_H_)
