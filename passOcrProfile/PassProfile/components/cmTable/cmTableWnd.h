// cmTableWnd.h : header file
//
#pragma once

#include "cmTable.h"
#include "cmCompositeWnd.h"

/////////////////////////////////////////////////////////////////////////////
// cmTableWnd window

class cmTableWnd : public cmCompositeWnd
{
// Construction
public:
	cmTableWnd();

// Operations
public:
	cmTable* GetInnerTable();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmTableWnd)
	virtual BOOL Create(const CRect& rc, CWnd* parentWnd, DWORD style, UINT nID = - 1) ;
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmTableWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmTableWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnGetDlgCode();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

	cmTable* table_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
