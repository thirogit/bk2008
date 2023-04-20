// cmGenericText.h : header file
//
#if !defined(_CMGENERICTEXT_H_)
#define _CMGENERICTEXT_H_

/////////////////////////////////////////////////////////////////////////////
// cmGenericText window
class cmBaseCell;

class cmGenericText : public CEdit
{
// Construction
public:
	cmGenericText();

// Attributes
public:
	void SetParentCell(cmBaseCell* parent);
	cmBaseCell* GetParentCell() const;

	UINT GetLastChar() const;
	void LockCursor();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmGenericText)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmGenericText();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmGenericText)
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();

	cmBaseCell* parentCell_;
	UINT lastChar_;
	bool cursorLocked_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CMGENERICTEXT_H_)
