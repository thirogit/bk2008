// cattlebasefrmView.h : interface of the CCattlebaseView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CATTLEBASEFRMVIEW_H__D1C6035C_5594_4D1F_A0D9_0880161B37B9__INCLUDED_)
#define AFX_CATTLEBASEFRMVIEW_H__D1C6035C_5594_4D1F_A0D9_0880161B37B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cattlebaseDoc.h"

class CCattlebaseView : public CView
{
protected: // create from serialization only
	CCattlebaseView();
	DECLARE_DYNCREATE(CCattlebaseView)

// Attributes
public:
	CCattlebaseDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCattlebaseView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCattlebaseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCattlebaseView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in cattlebasefrmView.cpp
inline CCattlebaseDoc* CCattlebaseView::GetDocument()
   { return (CCattlebaseDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATTLEBASEFRMVIEW_H__D1C6035C_5594_4D1F_A0D9_0880161B37B9__INCLUDED_)
