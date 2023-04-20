#if !defined(AFX_DATESPAN1_H__E43C0612_A1A8_462D_95DA_0489ABB58CDB__INCLUDED_)
#define AFX_DATESPAN1_H__E43C0612_A1A8_462D_95DA_0489ABB58CDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DateSpan1.h : header file
//
#include "../classes/match/MatchDate.h"

/////////////////////////////////////////////////////////////////////////////
// CDateSpan window

class CDateSpan : public CStatic
{

//	DECLARE_DYNAMIC(CDateSpan)
// Construction
public:
	CDateSpan();
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle,
				const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);

	//void SetLabelText(LPCTSTR lpszString);
	BOOL InitControls();
	void PosControls(void);
	
	TIMESPAN GetSpan(CDateTime &t1, CDateTime &t2);

// Attributes
public:

	//CStatic *m_pLabel;
	CComboBox *m_pCombo;
	CDateTimeCtrl *m_pDate1;
	CStatic *m_pSeparator;
	CDateTimeCtrl *m_pDate2;

	CString m_StrLabel;
	CString m_StrSeparator;
	CString m_StrBetween;
	CString m_StrExcept;
	CString m_StrBefore;
	CString m_StrAfter;

// Operations
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL Create(LPCSTR lpszLabel,LPCSTR lpszSeparator,const RECT& rect, CWnd* pParentWnd, UINT nID);

	virtual void PreSubclassWindow();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDateSpan)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDateSpan();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDateSpan)
		afx_msg void OnSpanCombo();
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnEnable(BOOL bEnable); 
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATESPAN1_H__E43C0612_A1A8_462D_95DA_0489ABB58CDB__INCLUDED_)
