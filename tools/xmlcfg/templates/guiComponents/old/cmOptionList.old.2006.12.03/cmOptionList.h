// AsOptionList.h : header file
//

#if !defined(_ASOPTIONLIST_H_)
#define _ASOPTIONLIST_H_

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

class cmOptionList : public CWnd,
					  public ICmOptionGroupListener
{
// Construction
public:
	cmOptionList();

// implementation of IAsOptionGroupListener
//
	void OnGroupSizeChanged(bool redraw = true);
	void OnSelectionInfoChanged(cmOptionGroup* selectedGroup, bool redraw = true);

	void GetSelectedItemInfo(cmString& title, 
							 cmString& description,
							 cmString& tooltipText,
							 UINT& iconId);

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

	cmOptionGroup* AddGroup(const cmString& name);
	cmOptionGroup* GetGroup(const cmString& name);

	void SetVerticalStripWidth(int width);
	int GetVerticalStripWidth() const;

	void SetVerticalSplitterPos(int pos);
	int GetVerticalSplitterPos() const;
		
	void SetFontInfo(const cmString& fontName, int fontSize = 10, bool fontBold = false);
	void SetFontInfo(const cmFontInfo& fontInfo);

	void RegisterListener(ICmOptionListListener* listener);

	void SetSorted(bool sorted);
	bool GetSorted() const;

	cmOptionRow* GetSelectedRow();

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
	afx_msg void OnMouseMove( UINT, CPoint );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

	void DrawBackground(CDC* pDC);
	void DrawGroups(CDC* pDC);
	void DrawBorder(CDC* pDC);
	long UpdateDrawArea();
	void UpdateWindowSize();
	void EnsureCellVisible(cmBaseCell* cell);
	bool HitSplitter(const CPoint& point);
	cmBaseCell* GetFocusedCell();

	CRect GetGlobalDrawArea();
    void ForwardMsgToGroups(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	COLORREF borderColor_;
	COLORREF bkColor_;
	COLORREF verticalSplitBkColor_;

	OptionGroupMap groups_;

	int verticalStripWidth_;
	int verticalSplitterPos_;
	long offset_;

	bool draggingSplitter_;
	bool sorted_;

	// font info
	//
	cmFontInfo fontInfo_;

	ICmOptionListListener* listener_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_ASOPTIONLIST_H_)
