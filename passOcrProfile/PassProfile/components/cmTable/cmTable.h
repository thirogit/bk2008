// cmTable.h : header file
//
#if !defined(_CM_TABLE_H_)
#define _CM_TABLE_H_

#include "cmString.h"
#include <vector>

using namespace std;

class cmTableRow;
class cmBaseCell;

#define WM_CELLSELECTION	WM_USER + 1

/////////////////////////////////////////////////////////////////////////////
// cmTable window
enum cmHeaderSelectionMode
{
	cmHeaderSelectionMode_None		= 0,
	cmHeaderSelectionMode_Single,
	cmHeaderSelectionMode_Multiple
};

enum enTableRowType
{
	enTableRowType_Classic = 0,
	enTableRowType_Style1
};

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
	cmBaseCell* GetSelectedHeader(unsigned int& row, unsigned int& col);
	cmBaseCell* GetCell(unsigned int row, unsigned int col);
	bool GetSelectedCell(cmTableRow** row, cmBaseCell** cell, unsigned int& rowIndex, unsigned int& cellIndex);

	void SetGridColor(COLORREF color);
	COLORREF GetGridColor() const;

	void SetBkColor(COLORREF color);
	COLORREF GetBkColor() const;

	void SetColumnSelectionMode(cmHeaderSelectionMode mode);
	cmHeaderSelectionMode SetColumnSelectionMode() const;

	void SetRowSelectionMode(cmHeaderSelectionMode mode);
	cmHeaderSelectionMode SetRowSelectionMode() const;

	void EnableBorder(bool enable);

	vector<unsigned int> GetSelColList();
	vector<unsigned int> GetSelRowList();

	void SetColumnsInfo(const vector<cmString>& names, const vector<cmString>& descriptions);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmTable)
	public:
	virtual BOOL Create(const CRect& rc, CWnd* parentWnd, DWORD style, UINT nID);
	//}}AFX_VIRTUAL

// Implementation
public:
	cmTableRow* AppendRow(unsigned int numColumns, enTableRowType type = enTableRowType_Classic, bool redraw = true);
	cmTableRow* InsertRow(unsigned int index, unsigned int numColumns, enTableRowType type = enTableRowType_Classic, bool redraw = true);
	cmTableRow* GetRow(unsigned int index);
	void DeleteRow(unsigned int index, bool redraw = true);
	unsigned int GetRowCount() const;
	void DeleteAll(bool redraw = true);
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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnCellSetFocus(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnCellLostFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCellSetSelection(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnCellLostSelection(WPARAM wParam, LPARAM lParam); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

	void Clear();
	void UpdateWindowSize(bool redraw = true);
	void UpdateFirstNonFixedRowIndex();
	void UpdateFirstNonFixedColumnIndex();
	void UpdateDrawArea();

	void DrawBackground(CDC* pDC);
	void DrawBorder(CDC* pDC);
	void DrawGrid(CDC* pDC);
	void DrawRows(CDC* pDC, const CPoint& mousePos);
	void DrawSelectedCell(CDC* pDC);

	unsigned int GetNonFixedRowsPerPage();
	unsigned int GetNonFixedColumnsPerPage();
	
	CRect GetFixedRowsArea();
	CRect GetFixedColsArea();
	unsigned int HitSplitter(const CPoint& point);

	bool MoveSelection(int horizontalShift, int verticalShift, bool redraw = true);
	bool GetFocusedItem(cmTableRow** row, cmBaseCell** cell, unsigned int& rowIndex, unsigned int& cellIndex);
	void SetCellFocus(bool redraw = true);
	void KillCellFocus(bool storeData);
	void EnsureVisible(unsigned int rowIndex, unsigned int cellIndex, bool redraw = true);

	void SelectColumn(unsigned int colIndex, bool redraw = false);
	void SelectRow(unsigned int rowIndex, bool redraw = false);
	void ClearSelection(bool redraw = false);

	vector<cmTableRow*> rows_;

	COLORREF bkColor_;
	COLORREF gridColor_;
	int defaultRowHeight_;
	int defaultCellWidth_;

	int horizontalOffset_;
	unsigned int rowsNumber_;
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

	int selSplitterIndex_;
	bool draggingSplitter_;
	unsigned int splitterRowIndex_;

	unsigned int selectedRowIndex_;
	unsigned int selectedCellIndex_;

	cmHeaderSelectionMode rowSelectionMode_;
	cmHeaderSelectionMode columnSelectionMode_;

	int selectedRowsCount_;
	int selectedColsCount_;

	bool borderEnabled_;

	vector<unsigned int> colWidth_;
	vector<cmString> colNames_;
	vector<cmString> colDescriptions_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CM_TABLE_H_)
