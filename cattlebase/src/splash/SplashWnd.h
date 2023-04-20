#if !defined(__SPLASHWND_H__)
#define __SPLASHWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSplashWnd

class CSplashWnd : public CWnd
{
protected:
	CWnd m_wndInvisibleParent;
	CBitmap m_bitmap;
	CSize m_sizeBitmap;
	CStringArray m_arrTextLines;
// Construction
public:

	CSplashWnd(
		CWnd * pWndParent = CWnd::GetDesktopWindow(),
		UINT nBitmapID = IDB_BITMAP_SPLASH
		);

// Attributes
public:
	CFont m_font;
	INT m_nMaxTextLines, m_nLineHeight;
	COLORREF m_clrText;
	CRect m_rcText;

// Operations
public:
	bool Create(
		CWnd * pWndParent,
		UINT nBitmapID
		);

	static bool RegisterSplashWndClass();

	void AddTextLine(
		LPCTSTR sText = NULL
		);
	void ReplaceLastLine(
		LPCTSTR sText = NULL
		);
	void ClearLines();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashWnd)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSplashWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSplashWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#define __SIMPLE_SPLASH_WINDOW_CLASS_NAME__ _T("__SIMPLE_SPLASH_WINDOW_CLASS_NAME__")

#endif 
