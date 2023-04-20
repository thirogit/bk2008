// rectdlg.h : header file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


/////////////////////////////////////////////////////////////////////////////
// CRectDlg dialog

#ifndef __RECTDLG_H__
#define __RECTDLG_H__

#include "DrawObj.h"

class CBaseDlg : public CPropertyPage
{
public:
	CBaseDlg(UINT nIDTemplate) : CPropertyPage(nIDTemplate) {};
	virtual void DoChanges(CDrawObj *pObj) = 0;
};


class CRectDlg : public CBaseDlg
{
	void _UpdateControls();

// Construction
public:
	DECLARE_DYNCREATE(CRectDlg);
	CRectDlg(CWnd* pParent = NULL); // standard constructor
	CRectDlg(BOOL bNoFill,UINT nPenSize,COLORREF clrFill,COLORREF clrOutline,CString &lable);
	bool Init(BOOL bNoFill,UINT nPenSize,COLORREF clrFill,COLORREF clrOutline,CString &lable);
	void DoChanges(CDrawObj *pObj);

	static COLORREF g_clrTransparent;
private:
	COLORREF m_clrFill, m_clrOutline;
	enum { IDD = IDD_PROP_RECT };
	CSpinButtonCtrl m_SpinCtrl;
	BOOL    m_bNoFill;
	UINT    m_nPenSize;
	CExtColorButton	m_BtnColorOutline;
	CExtColorButton	m_BtnColorFill;
	CString m_label;

	
	bool m_bInitialized;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CRectDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnInternalUpdateControls(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif __RECTDLG_H__

/////////////////////////////////////////////////////////////////////////////
