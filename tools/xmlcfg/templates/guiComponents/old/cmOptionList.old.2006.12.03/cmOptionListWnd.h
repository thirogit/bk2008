// AsOptionListWnd.h : header file
//
#if !defined(_ASOPTIONLISTWND_H_)
#define _ASOPTIONLISTWND_H_

#include "cmOptionList.h"
#include "cmFontInfo.h"
#include "ICmOptionListListener.h"
#include "cmString.h"

class ICmCellListener;

/////////////////////////////////////////////////////////////////////////////
// cmOptionListWnd window

class cmOptionListWnd : public CWnd,
						 public ICmOptionListListener
{
// Construction
public:
	cmOptionListWnd();

	// implementation of ICmOptionListListener
	//
	void OnInfoUpdate(const cmString& caption, 
					  const cmString& description, 
					  const cmString& tooltipText,
					  UINT tooltipIcon);

// Attributes
public:

// Operations
public:
	cmOptionList* GetOptionList();
	void SetFontInfo(const cmFontInfo& fontInfo);
	void SetFontInfo(const cmString& fontName, int fontSize, bool fontBold);

	void SetCellListener(ICmCellListener* cellListener);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmOptionListWnd)
	public:
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID = -1) ;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmOptionListWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmOptionListWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

	void UpdateWindowSize();
	void DrawBackground(CDC* pDC);
	void DrawInfo(CDC* pDC);

	cmOptionList* optionList_;
	cmFontInfo* fontInfo_;

	COLORREF bkColor_;
	COLORREF borderColor_;

	CRect infoRect_;

	cmString caption_;
	cmString description_;
	cmString tooltipText_;
	UINT tooltipIcon_;

	ICmCellListener* cellListener_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_ASOPTIONLISTWND_H_)
