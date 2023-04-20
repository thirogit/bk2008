// cmTreeCtrl.h : header file
//
#if !defined(_CMTREECTRL_H_)
#define _CMTREECTRL_H_

/////////////////////////////////////////////////////////////////////////////
// cmTreeCtrl window
#include <map>
#include "cmTreeNodeDescriptor.h"
#include "cmTreeTooltip.h"

using namespace std;


typedef map<HTREEITEM, cmTreeNodeDescriptor*> cmTreeNodes;

class cmTreeCtrl : public CTreeCtrl
{
// Construction
public:
	cmTreeCtrl();
	virtual ~cmTreeCtrl();

// Attributes
public:
	void SetBkColor(COLORREF bkColor);
	COLORREF GetBkColor() const;

	void SetTextColor(COLORREF txtColor);
	COLORREF GetTextColor() const;

	cmTreeNodeDescriptor* GetNodeDescriptor(HTREEITEM hNode);
	
	void CleanUp();

// Operations
public:
	cmTreeNodeDescriptor* InsertNode(const cmString& text, 
									 HTREEITEM hParent = TVI_ROOT, 
									 HTREEITEM hInsertAfter = TVI_LAST, 
									 UINT iconSelExpanded = 0, 
									 UINT iconSelCollapsed = 0,
									 UINT iconNotSelExpanded = 0,
									 UINT iconNotSelCollapsed = 0);
	void DeleteNode(HTREEITEM node);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cmTreeCtrl)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	

	// Generated message map functions
protected:
	//{{AFX_MSG(cmTreeCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();

	void DrawBackground(CDC* pDC);
	void DrawItems(CDC* pDC);
	void DrawNodeIcon(CDC* pDC, HTREEITEM item, CRect itemRect);

	void UpdateDefaultDescriptor();

	cmTreeNodeDescriptor* AddItemDescriptor(HTREEITEM hNode);
	cmTreeNodeDescriptor* GetItemDescriptor2(HTREEITEM hNode);

	void InitializeTooltip();
	void InitializeImageList();

	

	COLORREF bkColor_;
	COLORREF txtColor_;

	CRect m_rect;						// The client rect when drawing
	int   m_h_offset;					// 0... -x (scroll offset)
	int   m_h_size;						// width of unclipped window
	int   m_v_offset;					// 0... -y (scroll offset)
	int   m_v_size;						// height of unclipped window

	cmTreeNodes nodes_;
	cmTreeNodeDescriptor defaultDescriptor_;
	cmTreeTooltip tooltip_;

	CImageList imageList_;
	HTREEITEM hoverItem_;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CMTREECTRL_H_)
