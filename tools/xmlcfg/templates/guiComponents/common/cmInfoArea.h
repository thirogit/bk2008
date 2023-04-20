// cmInfoArea.h : header file
//

#pragma once

#include "cmString.h"
#include "cmBaseCellInfo.h"
#include "cmCellEnum.h"

/////////////////////////////////////////////////////////////////////////////
// cmInfoArea window

class cmInfoArea : public CWnd
{
// Construction
public:
	cmInfoArea();

// Attributes
public:
	// Attributes
public:
	void SetBkColor(COLORREF bkColor);
	COLORREF GetBkColor() const;

	void SetTextColor(COLORREF textColor);
	COLORREF GetTextColor() const;

	void SetExtraMsgTextColor(COLORREF textColor);
	COLORREF GetExtraMsgTextColor() const;

	void SetTitle(const cmString& title);
	cmString GetTitle() const;

	void SetDescription(const cmString& description);
	cmString GetDescription() const;

	void SetExtraMsg(const cmString& msg);
	cmString GetExtraMsg() const;

	static void SetExtraIcons(UINT infoIconId, UINT warningIconId, UINT questionIconId, UINT errorIconId);
	
	void SetCellInfo(cmBaseCellInfo* info);

	static UINT GetExtraIcon(enCmCellExtraMsgType msgType);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmInfoArea)
	public:
	virtual BOOL Create(const CRect& rc, CWnd* parentWnd, DWORD style, UINT nID = - 1) ;
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmInfoArea();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmInfoArea)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

	//void UpdateWindowSize();
	void DrawBackground(CDC* pDC);
	void DrawText(CDC* pDC);
	void DrawTooltip(CDC* pDC);

	COLORREF bkColor_;
	COLORREF textColor_;
	COLORREF extraMsgColor_;

	cmString title_;
	cmString description_;
	cmString extraMsg_;
	
	static UINT infoExtraIconId_;
	static UINT warningExtraIconId_;
	static UINT questionExtraIconId_;
	static UINT errorExtraIconId_;

	UINT currentExtraIconId_;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
