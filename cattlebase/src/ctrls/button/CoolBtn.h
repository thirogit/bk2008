#if !defined(AFX_COOLBTN_H__3A90681F_CE5F_11D3_808C_005004D6CF90__INCLUDED_)
#define AFX_COOLBTN_H__3A90681F_CE5F_11D3_808C_005004D6CF90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoolBtn window


class CCoolBtn : public CButton
{
// Construction
public:
	CCoolBtn();
	BOOL Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
	BOOL SetButtonImage(UINT nResourceId, COLORREF crMask);
	BOOL AddMenuItem(UINT nMenuId,const CString& sItemText, UINT nFlags);
	BOOL EnableMenuItem(UINT nMenuId, BOOL nEnable);
	BOOL RemoveMenuItem(UINT nMenuId, UINT nMenuFlags);
	void AddImage( int nIndex, CBitmap& bmpEnabled, CBitmap& bmpDisabled );
	void ChangeMenuFlags(UINT nMenuId, UINT nMenuFlags);
	void SetDefaultButton(BOOL bDefault);
	void EnableMenu(BOOL bEnable);

// Attributes
protected:
  CMenu        m_menu;
  CBitmap      m_btnImage;
  CImageList   m_IL;
  BOOL         m_bPushed;
  BOOL         m_bMenuPushed;
  BOOL         m_bMenuLoaded;
  BOOL         m_bDefaultBtn;
  BOOL         m_bLoaded;
  BITMAP       m_bm;
  CWnd*        m_pParentWnd;
  COLORREF	   m_crMask;
  HBITMAP	   m_hbmpDisabled;	
  BOOL         m_IsMenuEnabled;


// Operations
public:
protected:
  void DrawArrow(CDC* pDC,CPoint ArrowTip);
  BOOL HitMenuBtn(CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolBtn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCoolBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCoolBtn)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLBTN_H__3A90681F_CE5F_11D3_808C_005004D6CF90__INCLUDED_)
