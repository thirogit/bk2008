// cmSplitDialog.h : header file
//
#if !defined(_CMSPLITDIALOG_H_)
#define _CMSPLITDIALOG_H_

/////////////////////////////////////////////////////////////////////////////
// cmSplitDialog dialog
#include "cmTreeCtrl.h"
#include <vector>

//#include "resource.h"

using namespace std;

class cmSplitDialog : public CDialog
{
// Construction
public:
	cmSplitDialog(CWnd* pParent = NULL);   // standard constructor

	int AppendChild(CWnd* childWindow);
	void SelectChild(unsigned int index);
	CWnd* GetChild(unsigned int index);
	CWnd* GetActiveChild();

	void SetBkColor(COLORREF bkColor);
	COLORREF GetBkColor() const;

// Dialog Data
	//{{AFX_DATA(cmSplitDialog)
	//enum { IDD = IDD_SPLITTERDLG };
	CButton	cmdOk_;
	CButton	cmdCancel_;
	cmTreeCtrl	tree_;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmSplitDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cmSplitDialog)
	afx_msg void OnSelChangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnItemExpandedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message );
    afx_msg void OnMouseMove( UINT, CPoint );
	afx_msg void OnMouseUp( UINT, CPoint );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

	void DrawBackground(CDC* pDC);
	void DrawSplitter(CDC* pDC);
	void UpdateWindowSize();
	virtual void OnTreeNodeSelection(HTREEITEM oldItem, HTREEITEM newItem);
	virtual void OnTreeNodeExpansion(HTREEITEM item);

	bool initialized_;

	CRect treeRect_;
	CRect childRect_;
	int splitterPos_;
	int selectedChildIndex_;
	bool draggingSplitter_;
	vector<CWnd*> rightPaneChildren_;
	bool HitSplitter(const CPoint& pt);
	bool embedded_;

	COLORREF bkColor_;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CMSPLITDIALOG_H_)
