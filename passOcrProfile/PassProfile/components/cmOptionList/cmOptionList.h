// cmOptionList.h : header file
//

#pragma once

#include <map>
#include "ICmOptionGroupListener.h"
#include "cmFontInfo.h"
#include "cmString.h"

using namespace std;

class cmOptionGroup;
class cmOptionRow;
class cmBaseCell;
class ICmOptionListListener;

/////////////////////////////////////////////////////////////////////////////
// cmOptionList window

typedef map<cmString, cmOptionGroup*> OptionGroupMap;

enum enOptionGroupType
{
	enOptionGroupType_Classic = 0,
	enOptionGroupType_Style1
};

class cmOptionList : public CWnd,
					  public ICmOptionGroupListener
{
// Construction
public:
	cmOptionList();

// implementation of IAsOptionGroupListener
//
	void OnGroupSizeChanged(bool redraw = true);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmOptionList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmOptionList();
	
	BOOL Create(const CRect& rc, CWnd* parentWnd, DWORD style = WS_CHILD | WS_VISIBLE, UINT nID = 0);

	void SetBorderColor(COLORREF borderColor, bool redraw = true );
	COLORREF GetBorderColor() const;

	void SetBkColor(COLORREF bkColor, bool redraw = true);
	COLORREF GetBkColor() const;

	OptionGroupMap* GetOptionGroupMap();

	cmOptionGroup* AddGroup(const cmString& name, enOptionGroupType groupType = enOptionGroupType_Style1); 
	cmOptionGroup* GetGroup(const cmString& name);
	cmOptionGroup* GetGroup(unsigned int index);

	void SetVerticalStripWidth(int width);
	int GetVerticalStripWidth() const;

	void SetVerticalSplitterPos(int pos);
	int GetVerticalSplitterPos() const;
	
	void RegisterListener(ICmOptionListListener* listener);

	void SetSorted(bool sorted);
	bool GetSorted() const;

	cmOptionRow* GetSelectedRow();

	void Clear();

	void StoreData(bool redraw = true);

	void SetBorderSize(int s);
	int GetBorderSize() const;

	void SetGroupSpacing(int s);
	int GetGroupSpacing() const;

	// Generated message map functions
protected:
	//{{AFX_MSG(cmOptionList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnCellSetFocus(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnCellLostFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCellSetSelection(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnCellLostSelection(WPARAM wParam, LPARAM lParam); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

	void DrawBackground(CDC* pDC);
	void DrawGroups(CDC* pDC, const CPoint& mousePos);
	void DrawBorder(CDC* pDC);
	long UpdateDrawArea();
	void UpdateWindowSize();
	void EnsureCellVisible(cmBaseCell* cell);
	bool HitSplitter(const CPoint& point);
	cmBaseCell* GetFocusedCell();
	cmOptionGroup* GetSelectedGroup(unsigned int& index);
	bool MoveSelection(int verticalShift, bool redraw);
	void KillCellFocus(bool storeData, bool redraw = true);
	cmString GetGroupSortKey(unsigned int index);

	CRect GetGlobalDrawArea();

	COLORREF borderColor_;
	COLORREF bkColor_;
	COLORREF verticalSplitBkColor_;
	COLORREF cellBkColor_;

	OptionGroupMap groups_;

	int verticalStripWidth_;
	int verticalSplitterPos_;
	long offset_;

	bool draggingSplitter_;
	bool sorted_;

	ICmOptionListListener* listener_;

	int borderSize_;
	int groupSpacing_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

