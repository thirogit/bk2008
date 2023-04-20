#ifndef __DRWDOCVIEW_H__
#define __DRWDOCVIEW_H__

#ifndef __AFXPRIV_H__
	#include <AfxPriv.h>
#endif
#if (!defined __EXT_HOOK_H)
	#include <../Src/ExtHook.h>
#endif

#include "../../src/printing/CowDrwDocView.h"
#include "../../src/printing/Picture.h"
#include "../../src/printing/DocObjList.h"
#include "DrwDoc.h"

// Hints for UpdateAllViews/OnUpdate
#define HINT_UPDATE_WINDOW					0
#define HINT_UPDATE_DRAWOBJ					1
#define HINT_UPDATE_SELECTION				2
#define HINT_DELETE_SELECTION				3
#define HINT_UPDATE_OLE_ITEMS				4
#define HINT_UPDATE_TOOLBAR_COLOR_BUTTONS	5
//#define HINT_UPDATE_TEXT_OBJECTS            6

class CDrwDocView : public CCowDrwDocView
{
	CDrwDocView();
	DECLARE_DYNCREATE(CDrwDocView)
	CDrwDoc* GetDocument();

	CRect GetInitialPosition();
	void Select(CDocObj* pObj, BOOL bAdd = FALSE);
	void SelectWithinRect(CRect rect, BOOL bAdd = FALSE);
	void Deselect(CDocObj* pObj);
	void CloneSelection();
	void UpdateActiveItem();
	void InvalObj(CDocObj* pObj);
	void Remove(CDocObj* pObj);
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

public:
	virtual ~CDrwDocView();


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

	CDocObjList m_selection;
	BOOL m_bGrid;
	BOOL m_bBackDoc;
	COLORREF m_gridColor;
	BOOL m_bActive; // is the view active?

	int nZoom;
	void SetZoom(int Zoom);
	inline int GetZoom() { return nZoom; };
	void SetUpDCZoom(CDC *pDC,int Zoom);
	void NormalizePosition(CRect &rect);
	

#ifdef FOR_CATTLEBASE
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
#endif

	virtual void OnInitialUpdate(); // called first time after construct

public:
	virtual BOOL IsSelected(const CObject* pDocItem) const;

	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
protected:
	
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

	DECLARE_MESSAGE_MAP()
}; 

#endif 