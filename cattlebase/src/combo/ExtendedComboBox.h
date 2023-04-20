// ExtendedComboBox.h : header file
//

class CExtendedComboBox;

/////////////////////////////////////////////////////////////////////////////
// CListBoxInsideComboBox window

class CListBoxInsideComboBox : public CWnd
{
// Construction
public:
	CListBoxInsideComboBox();

// Attributes
public:
	CExtendedComboBox *m_Parent;
	void SetParent(CExtendedComboBox *);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxInsideComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListBoxInsideComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListBoxInsideComboBox)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CExtendedComboBox window

class CExtendedComboBox : public CComboBox
{
// Construction
public:
	CExtendedComboBox();

// Attributes
public:
	// default implementation uses LSB of item data; override to get other behaviour
	virtual BOOL IsItemEnabled(UINT) const;

protected:
	CString m_strSavedText;	// saves text between OnSelendok and OnRealSelEndOK calls
	CListBoxInsideComboBox m_ListBox;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtendedComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtendedComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtendedComboBox)
	afx_msg int OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex);
	afx_msg void OnSelendok();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	LRESULT OnCtlColor(WPARAM,LPARAM lParam);
	afx_msg LRESULT OnRealSelEndOK(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
