// cmTimePicker.h : header file
//

#pragma once

#include "cmBaseCell.h"

/////////////////////////////////////////////////////////////////////////////
// cmTimePicker window

class cmTimePicker : public CDateTimeCtrl
{
// Construction
public:
	cmTimePicker();

	void SetParentCell(cmBaseCell* parent);
	cmBaseCell* GetParentCell() const;

	void EnableSeconds();
	void DisableSeconds();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmTimePicker)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmTimePicker();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmTimePicker)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();

	static LRESULT CALLBACK MouseProc(	int nCode,      // hook code
										WPARAM wParam,  // message identifier
										LPARAM lParam   // mouse coordinates
										);


	cmBaseCell* parentCell_;
	bool displaySeconds_;

	static HHOOK spinBtnHook_;
	static cmTimePicker* hookOwner_;
	static CWnd* spinBtn_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
