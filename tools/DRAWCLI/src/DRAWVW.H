// drawvw.h : interface of the CDrawView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __DRAWVW_H__
#define __DRAWVW_H__


#ifndef __AFXPRIV_H__
	#include <AfxPriv.h>
#endif
#if (!defined __EXT_HOOK_H)
	#include <../Src/ExtHook.h>
#endif

#include "picture.h"
#include "drawdoc.h"

// Hints for UpdateAllViews/OnUpdate
#define HINT_UPDATE_WINDOW					0
#define HINT_UPDATE_DRAWOBJ					1
#define HINT_UPDATE_SELECTION				2
#define HINT_DELETE_SELECTION				3
#define HINT_UPDATE_OLE_ITEMS				4
#define HINT_UPDATE_TOOLBAR_COLOR_BUTTONS	5
//#define HINT_UPDATE_TEXT_OBJECTS            6


#ifndef FOR_CATTLEBASE
class CDrawObj;
#endif

class CATTLEPRINT_DLLAPI CDrawView : public CScrollView
{
#if !defined(FOR_CATTLEBASE)
protected: // create from serialization only
#else
public: // create dynamicly when printing
#endif
	CDrawView();
	DECLARE_DYNCREATE(CDrawView)

public:
	CDrawDoc* GetDocument()  { return (CDrawDoc*)m_pDocument; }
	void DocToClient(CRect& rect);
	void DocToClient(CPoint& point);
	void ClientToDoc(CPoint& point);
	void ClientToDoc(CRect& rect);
	static void SetUpDC(CDC *pDC);
	void SetPageSize(CSize size);

#ifdef FOR_CATTLEBASE
	CFrameWnd *m_pOldFrame;
	int nCurPrintPage;
	CDialog *m_pCtrlDlg;

#endif



#ifndef FOR_CATTLEBASE
	CRect GetInitialPosition();
	void Select(CDrawObj* pObj, BOOL bAdd = FALSE);
	void SelectWithinRect(CRect rect, BOOL bAdd = FALSE);
	void Deselect(CDrawObj* pObj);
	void CloneSelection();
	void UpdateActiveItem();
	void InvalObj(CDrawObj* pObj);
	void Remove(CDrawObj* pObj);
	void PasteNative(COleDataObject& dataObject);
	void PasteEmbedded(COleDataObject& dataObject, CPoint point );
	bool LoadDocImage(LPCTSTR szFileName);
	

protected:
	// added for drop-target functionality
	COleDropTarget m_dropTarget;        // for drop target functionality
	CPoint m_dragPoint;                 // current position
	CSize m_dragSize;                   // size of dragged object
	CSize m_dragOffset;                 // offset between pt and drag object corner
	DROPEFFECT m_prevDropEffect;
	BOOL m_bDragDataAcceptable;

	CPicture m_BackDoc;


	BOOL GetObjectInfo(COleDataObject* pDataObject,	CSize* pSize, CSize* pOffset);

#endif	// end of drop-target additions

public:
	virtual ~CDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);  // overriden to record time/date

	
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


#ifndef FOR_CATTLEBASE
	void DrawHelp(CDC* pDC);

	// added for drop-target functionality

	virtual void OnActivateView(BOOL bActivate, CView* pActiveView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll);
	virtual BOOL OnDrop(COleDataObject* pDataObject,DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject,DWORD grfKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject,DWORD grfKeyState, CPoint point);
	virtual void OnDragLeave();
	void SyncToolbarObjButtons();
	static CLIPFORMAT m_cfObjectDescriptor;
	// end of drop-target additions
	static CLIPFORMAT m_cfDraw; // custom clipboard format

	CDrawObjList m_selection;
	BOOL m_bGrid;
	BOOL m_bBackDoc;
	COLORREF m_gridColor;
	BOOL m_bActive; // is the view active?

	int nZoom;
	void SetZoom(int Zoom);
	inline int GetZoom() { return nZoom; };
	void SetUpDCZoom(CDC *pDC,int Zoom);
	void NormalizePosition(CRect &rect);
#endif
	

protected:
	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
#ifdef FOR_CATTLEBASE
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
#endif


	virtual void OnInitialUpdate(); // called first time after construct

#ifndef FOR_CATTLEBASE
public:
	virtual BOOL IsSelected(const CObject* pDocItem) const;
#endif

#if !defined(FOR_CATTLEBASE)
protected: 
#else
public: //need this public
#endif
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
protected:
#ifndef FOR_CATTLEBASE
	//{{AFX_MSG(CDrawView)

	
	afx_msg  void OnNewPage();
	afx_msg  void OnNextPage();
	afx_msg  void OnPrevPage();	
	afx_msg  void OnDelPage();

	afx_msg void OnViewBackDoc();
	afx_msg void OnUpdateViewBackDoc(CCmdUI* pCmdUI);


	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEdit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDrawSelect();
	afx_msg void OnDrawRoundRect();
	afx_msg void OnDrawRect();
	afx_msg void OnDrawLine();
	afx_msg void OnDrawEllipse();
	afx_msg void OnUpdateDrawEllipse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawRect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawRoundRect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSingleSelect(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditClear();
	afx_msg void OnUpdateAnySelect(CCmdUI* pCmdUI);
	afx_msg void OnDrawPolygon();
	afx_msg void OnUpdateDrawPolygon(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewGrid();
	afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
	afx_msg void OnOpenBackImg();
	afx_msg void OnSavePageToXml();

	afx_msg void OnObjectFillColor();
	afx_msg void OnObjectLineColor();
	afx_msg void OnObjectMoveBack();
	afx_msg void OnObjectMoveForward();
	afx_msg void OnObjectMoveToBack();
	afx_msg void OnObjectMoveToFront();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnViewShowObjects();
	afx_msg void OnUpdateViewShowObjects(CCmdUI* pCmdUI);
	afx_msg void OnEditProperties();
	afx_msg void OnUpdateEditProperties(CCmdUI* pCmdUI);

	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdateDocLineWidth(CCmdUI* pCmdUI);

	afx_msg void OnDrawText();
	afx_msg void OnUpdateDrawText(CCmdUI* pCmdUI);

	afx_msg void OnDrawImage();
	afx_msg void OnUpdateDrawImage(CCmdUI* pCmdUI);

	//}}AFX_MSG
	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorSelectCustom(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateSelectionSingle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectionNonEmpty(CCmdUI* pCmdUI);
	afx_msg void OnLineWidth( UINT nCmdID );
	afx_msg void OnUpdateLineWidth( CCmdUI * pCmdUI );
	afx_msg LRESULT OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateGroupProp(CCmdUI* pCmdUI);
	afx_msg void OnAlignLefts();
	afx_msg void OnAlignTops();
	afx_msg void OnGroupPos();
	afx_msg void OnShowGroupMgr();
	afx_msg void OnAddToGroup();
	afx_msg void OnSelGroup();
	afx_msg void OnShowStyleMgr();
	afx_msg BOOL OnMakeSameSize(UINT nId);
	afx_msg void OnTopBottom();
	afx_msg void OnLeftRight();
	afx_msg void OnUpdateNeighbours(CCmdUI* pCmdUI);

	
#endif

	DECLARE_MESSAGE_MAP()
}; // class CDrawView

#ifndef FOR_CATTLEBASE
class CATTLEPRINT_DLLAPI CDrawPreviewView
	: public CPreviewView
	, public CExtHookSink
{

	class CSubclassBtn : public CExtButton
	{
	public:
		static const int g_nNcSeparatorWidth;
		bool m_bNcSeparator:1;
		CSubclassBtn( HWND hWnd );
		virtual void PostNcDestroy();
		virtual LRESULT WindowProc(
			UINT message,
			WPARAM wParam,
			LPARAM lParam
			);
	}; // class CSubclassBtn

protected: // create from serialization only
	CDrawPreviewView();
	DECLARE_DYNCREATE(CDrawPreviewView)

	// Attributes
public:

	// Operations
public:

	// Implementation
protected:
	virtual LRESULT WindowProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam
		);
	virtual bool OnHookWndMsg(
		LRESULT & lResult,
		HWND hWndHooked,
		UINT nMessage,
		WPARAM & wParam,
		LPARAM & lParam
		);


}; // class CDrawPreviewView
#endif


#endif //__DRAWVW_H__
/////////////////////////////////////////////////////////////////////////////
