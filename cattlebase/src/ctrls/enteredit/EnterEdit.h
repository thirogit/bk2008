#if !defined(AFX_ENTEREDIT_H__3F2FF2A0_C05C_11D5_A04D_006067718D04__INCLUDED_)
#define AFX_ENTEREDIT_H__3F2FF2A0_C05C_11D5_A04D_006067718D04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../FilterEdit/FilterEdit.h"

// EnterEdit.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CEnterEdit window

#define EN_EDITCOMPLETE (WM_APP + 100)

class CEnterEdit : public CFilterEdit
{
// Construction
public:
	CEnterEdit();
	CEnterEdit(const TCHAR *pszRegEx);
	~CEnterEdit();
	bool IsUncommited();
	virtual void SetCommitedText(LPCSTR szText);
protected:
	virtual void OnReturnKey();

	COLORREF m_crBackGnd;
	CBrush m_brBackGnd;
	CString m_prevText;
	bool m_bUncommited;
	void CreateBackgroudBrush();
	void DeleteBackgroundBrush();

	virtual void PreSubclassWindow();
	virtual bool OnSetText(const TCHAR *szText);
	bool IsTextChanged();
	

protected:
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
};

/****************************************************************************
*                              UWM_EDIT_COMPLETE
* Inputs:
*       WPARAM: Control ID of the control whose edit completed
*	LPARAM: CWnd * of the control whose edit completed
* Result: LRESULT
*       Logically void, 0, always
* Effect: 
*       Posted/Sent to the parent of this control to indicate that the
*	edit has completed, either by the user typing <Enter> or focus leaving
****************************************************************************/

//#define UWM_EDIT_COMPLETE_MSG _T("UWM_EDIT_COMPLETE-{165BBEA0-C1A8-11d5-A04D-006067718D04}")


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTEREDIT_H__3F2FF2A0_C05C_11D5_A04D_006067718D04__INCLUDED_)
