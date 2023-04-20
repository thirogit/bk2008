// cmTreeTooltip.h : header file
//
#if !defined(_CMTREETOOLTIP_H_)
#define _CMTREETOOLTIP_H_

/////////////////////////////////////////////////////////////////////////////
// cmTreeTooltip window

class cmTreeNodeDescriptor;

class cmTreeTooltip : public CToolTipCtrl
{
// Construction
public:
	cmTreeTooltip();

	void SetTreeNodeDescriptor(cmTreeNodeDescriptor* desc);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmTreeTooltip)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~cmTreeTooltip();

	// Generated message map functions
protected:
	//{{AFX_MSG(cmTreeTooltip)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnShow(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();

	void UpdateFont();

	cmTreeNodeDescriptor* nodeDescriptor_;
	CFont font_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CMTREETOOLTIP_H_)
