// MainTreeView.h : interface of the CMainTreeCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(_MAINTREECTRL_H_)
#define _PROJECTTREECTRL_H_

#include "stdafx.h"
#include <afxcview.h>

class IMainTreeCtrlListener;

class CMainTreeCtrl : public CTreeView
{
protected: // create from serialization only
	CMainTreeCtrl();
	DECLARE_DYNCREATE(CMainTreeCtrl)

// Attributes
public:
	void RegisterListener(IMainTreeCtrlListener* listener);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainTreeCtrl)
	public:
	
	protected:
	
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainTreeCtrl();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CProjectTreeView)
	afx_msg BOOL OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

	IMainTreeCtrlListener* listener_;
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_MAINTREECTRL_H_)