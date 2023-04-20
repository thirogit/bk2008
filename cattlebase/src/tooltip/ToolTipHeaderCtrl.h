#if !defined(AFX_TOOLTIPHEADERCTRL_H__FED6DC79_DF23_4F7D_A9C7_D9A899632EE9__INCLUDED_)
#define AFX_TOOLTIPHEADERCTRL_H__FED6DC79_DF23_4F7D_A9C7_D9A899632EE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTipHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolTipHeaderCtrl window

#define MAX_TIP_LENGTH 80


class CToolTipHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CToolTipHeaderCtrl();

// Attributes
public:

// Operations
public:
	int AddToolTip(int nCol, LPCTSTR lpszTip, int nFormat);
	void RecalcToolRect();

protected:
	CToolTipCtrl m_Tooltip;

// Attributes
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTipHeaderCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolTipHeaderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolTipHeaderCtrl)
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPHEADERCTRL_H__FED6DC79_DF23_4F7D_A9C7_D9A899632EE9__INCLUDED_)
