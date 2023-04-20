// cmcompositewnd.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// cmCompositeWnd window

class cmInfoArea;

class cmCompositeWnd : public CWnd
{
// Construction
public:
	cmCompositeWnd();

// Attributes
public:
	cmInfoArea* GetInfoArea();

	void ShowInfoArea(bool enable, bool redraw = true);

	void SetBorderColor(COLORREF color, bool redraw = true);
	COLORREF GetBorderColor() const;

	void EnableBorder(bool enable, bool redraw = true);

	void SetSplitterColor(COLORREF color, bool enableGradient, bool redraw = true);
	COLORREF GetSplitterColor() const;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmCompositeWnd)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmCompositeWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmCompositeWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnCellSetFocus(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnCellLostFocus(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnCellSetSelection(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnCellLostSelection(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

	void UpdateWindowSize();
	
	void DrawBackground(CDC* pDC);
	void DrawBorders(CDC* pDC);
	void DrawSplitter(CDC* pDC);

	CRect GetDefaultSplitterRect(const CRect& rc);
	CRect GetSplitterRange();
	void UpdateSplitterRect();

	cmInfoArea* infoArea_;
	CWnd* upperPaneWnd_;

	bool borderEnabled_;
	bool infoAreaEnabled_;
	COLORREF borderColor_;
	COLORREF bkColor_;
	COLORREF splitterColor_;
	bool splitterGradient_;

	CRect splitterRect_;
	bool draggingSplitter_;
	CPoint lastDragPoint_;
	CRect lastRc_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
