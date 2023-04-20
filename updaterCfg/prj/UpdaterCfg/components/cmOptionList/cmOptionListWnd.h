// AsOptionListWnd.h : header file
//
#if !defined(_ASOPTIONLISTWND_H_)
#define _ASOPTIONLISTWND_H_

#include "cmOptionList.h"
#include "cmFontInfo.h"
//#include "ICmOptionListListener.h"
#include "cmString.h"
#include "cmCompositeWnd.h"

class ICmCellListener;

/////////////////////////////////////////////////////////////////////////////
// cmOptionListWnd window

class cmOptionListWnd : public cmCompositeWnd//,						 public ICmOptionListListener
{
// Construction
public:
	cmOptionListWnd();

	// implementation of ICmOptionListListener
	//
/*	void OnInfoUpdate(	const cmString& caption, 
						const cmString& description, 
						const cmString& tooltipText,
						UINT tooltipIcon);
*/

// Operations
public:
	cmOptionList* GetOptionList();

	void SetCellListener(ICmCellListener* cellListener);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmOptionListWnd)
	public:
	virtual BOOL Create(const CRect& rc, CWnd* parentWnd, DWORD style, UINT nID = - 1) ;
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmOptionListWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmOptionListWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();


	cmOptionList* optionList_;

	COLORREF bkColor_;

	ICmCellListener* cellListener_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_ASOPTIONLISTWND_H_)
