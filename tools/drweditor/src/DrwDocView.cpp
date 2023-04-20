#include "stdafx.h"
#include "DrwDocView.h"
#include "DrwDocOleObj.h"
//#include <afxpriv.h>

//#include "drawcli.h"
//#include "drawtool.h"
//#include "resource.h"     
//#include "GroupMgrDlg.h"
//#include "GroupPos.h"
//#include "ObjGroup.h"
//#include "StyleMgr.h"
//#include "Mainfrm.h"
//#include "cntritem.h"
//#include "drawvw.h"
//#include "drawobj.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

CLIPFORMAT CDrwDocView::m_cfDraw = (CLIPFORMAT)::RegisterClipboardFormat(_T("DRWDOCOBJECT"));
CLIPFORMAT CDrwDocView::m_cfObjectDescriptor = NULL;

IMPLEMENT_DYNCREATE(CDrwDocView, CScrollView)

BEGIN_MESSAGE_MAP(CDrwDocView, CScrollView)

	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT, OnCancelEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_DRAW_SELECT, OnDrawSelect)
	ON_COMMAND(ID_DRAW_ROUNDRECT, OnDrawRoundRect)
	ON_COMMAND(ID_DRAW_RECT, OnDrawRect)
	ON_COMMAND(ID_DRAW_LINE, OnDrawLine)
	ON_COMMAND(ID_DRAW_ELLIPSE, OnDrawEllipse)
	ON_COMMAND(ID_DRAW_TEXT, OnDrawText)
	ON_COMMAND(ID_DRAW_IMAGE, OnDrawImage)

	ON_COMMAND(ID_ALIGNTOPS, OnAlignTops)
	ON_COMMAND(ID_ALIGNLEFTS, OnAlignLefts)
	ON_COMMAND(ID_GROUPPOS, OnGroupPos)	
	ON_COMMAND(ID_ADDTOGROUP,OnAddToGroup)
	ON_COMMAND(ID_NEW_PAGE, OnNewPage)
	ON_COMMAND(ID_NEXT_PAGE, OnNextPage)
	ON_COMMAND(ID_PREV_PAGE, OnPrevPage)
	ON_COMMAND(ID_DEL_PAGE, OnDelPage)
	ON_COMMAND(ID_SHOWGROUPMGR,OnShowGroupMgr)
	ON_COMMAND(ID_SELECTGROUP,OnSelGroup)
	ON_COMMAND(ID_SHOW_STYLEMANAGER,OnShowStyleMgr)
	ON_COMMAND(ID_TOPBOTTOM,OnTopBottom)
	ON_COMMAND(ID_LEFTRIGHT,OnLeftRight)

	ON_COMMAND(ID_GRID, OnViewGrid)
	ON_UPDATE_COMMAND_UI(ID_GRID, OnUpdateViewGrid)

	
	ON_COMMAND(ID_BACKDOC, OnViewBackDoc)
	ON_UPDATE_COMMAND_UI(ID_BACKDOC, OnUpdateViewBackDoc)

	ON_UPDATE_COMMAND_UI(ID_DRAW_ELLIPSE, OnUpdateDrawEllipse)
	ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, OnUpdateDrawLine)
	ON_UPDATE_COMMAND_UI(ID_DRAW_RECT, OnUpdateDrawRect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_ROUNDRECT, OnUpdateDrawRoundRect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_SELECT, OnUpdateDrawSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEBACK, OnUpdateSingleSelect)
	ON_UPDATE_COMMAND_UI(ID_DRAW_TEXT, OnUpdateDrawText)
	ON_UPDATE_COMMAND_UI(ID_DRAW_IMAGE, OnUpdateDrawImage)
	ON_UPDATE_COMMAND_UI(ID_SELECTGROUP,OnUpdateSingleSelect)

	ON_UPDATE_COMMAND_UI(ID_ALIGNTOPS, OnUpdateGroupProp)
	ON_UPDATE_COMMAND_UI(ID_ALIGNLEFTS, OnUpdateGroupProp)
	ON_UPDATE_COMMAND_UI(ID_GROUPPOS, OnUpdateGroupProp)

	ON_UPDATE_COMMAND_UI(ID_MAKESAMESIZE_HEIGHT,OnUpdateGroupProp)
	ON_UPDATE_COMMAND_UI(ID_MAKESAMESIZE_WIDTH,OnUpdateGroupProp)
	ON_UPDATE_COMMAND_UI(ID_MAKESAMESIZE_BOTH,OnUpdateGroupProp)
	ON_UPDATE_COMMAND_UI(ID_TOPBOTTOM,OnUpdateNeighbours)
	ON_UPDATE_COMMAND_UI(ID_LEFTRIGHT,OnUpdateNeighbours)

	ON_COMMAND_EX(ID_MAKESAMESIZE_HEIGHT,OnMakeSameSize)
	ON_COMMAND_EX(ID_MAKESAMESIZE_WIDTH,OnMakeSameSize)
	ON_COMMAND_EX(ID_MAKESAMESIZE_BOTH,OnMakeSameSize)

	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateAnySelect)
	ON_COMMAND(ID_DRAW_POLYGON, OnDrawPolygon)
	ON_UPDATE_COMMAND_UI(ID_DRAW_POLYGON, OnUpdateDrawPolygon)
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)

	ON_COMMAND(ID_OPENBACKIMG, OnOpenBackImg)	

	//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND( ID_FILE_PRINT_PREVIEW, OnFilePrintPreview )
	
	ON_COMMAND(ID_OBJECT_FILLCOLOR, OnObjectFillColor)
	ON_COMMAND(ID_OBJECT_LINECOLOR, OnObjectLineColor)
	ON_COMMAND(ID_OBJECT_MOVEBACK, OnObjectMoveBack)
	ON_COMMAND(ID_OBJECT_MOVEFORWARD, OnObjectMoveForward)
	ON_COMMAND(ID_OBJECT_MOVETOBACK, OnObjectMoveToBack)
	ON_COMMAND(ID_OBJECT_MOVETOFRONT, OnObjectMoveToFront)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_SHOWOBJECTS, OnViewShowObjects)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWOBJECTS, OnUpdateViewShowObjects)
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PROPERTIES, OnUpdateEditProperties)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	
	ON_WM_CONTEXTMENU()
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEFORWARD, OnUpdateSingleSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOBACK, OnUpdateSingleSelect)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOFRONT, OnUpdateSingleSelect)
	ON_UPDATE_COMMAND_UI(ID_DOC_LINE_WIDTH, OnUpdateDocLineWidth)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)


	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPrepareMenu,
		OnExtMenuPrepare
		)

	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEBACK, OnUpdateSelectionSingle)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEFORWARD, OnUpdateSelectionSingle)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOBACK, OnUpdateSelectionSingle)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVETOFRONT, OnUpdateSelectionSingle)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_FILLCOLOR, OnUpdateSelectionNonEmpty)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_LINECOLOR, OnUpdateSelectionNonEmpty)

	// color popup menu
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyCustColor,
		OnColorSelectCustom
		)

	ON_COMMAND_RANGE(
		ID_LINE_WIDTH_0,
		ID_LINE_WIDTH_OTHER,
		OnLineWidth
		)
	ON_UPDATE_COMMAND_UI_RANGE(
		ID_LINE_WIDTH_0,
		ID_LINE_WIDTH_OTHER,
		OnUpdateLineWidth
		)

	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPopupDrawItem,
		OnDrawPopupMenuItem
		)
END_MESSAGE_MAP()

CDrwDocView::CDrwDocView()
{
	m_bGrid = TRUE;
	m_bBackDoc = FALSE;
	m_gridColor = RGB(0, 0, 128);
	m_bActive = FALSE;

	if( m_cfObjectDescriptor == NULL ) 
		m_cfObjectDescriptor = (CLIPFORMAT)::RegisterClipboardFormat(_T("Object Descriptor") );
	m_prevDropEffect = DROPEFFECT_NONE;
	
	nZoom = 100;

}

CDrwDoc* CDrwDocView::GetDocument()
{ 
	return (CDrwDoc*)m_pDocument; 
}

CDrwDocView::~CDrwDocView()
{

}

void CDrwDocView::OnActivateView(BOOL bActivate, CView* pActiveView,CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActiveView, pDeactiveView);

	// invalidate selections when active status changes
	if (m_bActive != bActivate)
	{
		if( bActivate )  // if becoming active update as if active
		{
			m_bActive = bActivate;
			SyncToolbarObjButtons();
		}
		if (!m_selection.IsEmpty())
			OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_bActive = bActivate;
	}

	
}
void CDrwDocView::InvalObj(CDocObj* pObj)
{
	CRect rect = pObj->GetPosition();
	DocToClient(rect);
	if (m_bActive && IsSelected(pObj))
	{
		rect.left -= 4;
		rect.top -= 5;
		rect.right += 5;
		rect.bottom += 4;
	}
	rect.InflateRect(1, 1); // handles CDrawOleObj objects

	InvalidateRect(rect, FALSE);
}


void CDrwDocView::OnUpdate(CView* , LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	
	case HINT_UPDATE_WINDOW:    // redraw entire window
		Invalidate(FALSE);
		break;

	case HINT_UPDATE_DRAWOBJ:   // a single object has changed
		InvalObj((CDocObj*)pHint);
		break;

	case HINT_UPDATE_SELECTION: // an entire selection has changed
		{
			CDocObjList* pList = pHint != NULL ? (CDocObjList*)pHint : &m_selection;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
				InvalObj(pList->GetNext(pos));
		}
		break;

	case HINT_DELETE_SELECTION: // an entire selection has been removed
		if (pHint != &m_selection)
		{
			CDocObjList* pList = (CDocObjList*)pHint;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
			{
				CDocObj* pObj = pList->GetNext(pos);
				InvalObj(pObj);
				Remove(pObj);   // remove it from this view's selection
			}
		}
		break;

	case HINT_UPDATE_OLE_ITEMS:
		{
			CDrwDoc* pDoc = GetDocument();
			POSITION pos = pDoc->GetObjects()->GetHeadPosition();
			while (pos != NULL)
			{
				CDocObj* pObj = pDoc->GetObjects()->GetNext(pos);
				if (pObj->IsKindOf(RUNTIME_CLASS(CDrwDocOleObj)))
					InvalObj(pObj);
			}
		}
		break;

	case HINT_UPDATE_TOOLBAR_COLOR_BUTTONS:
		{
			CWnd * pWndFocus = GetFocus();
			if( pWndFocus != NULL
				&& (pWndFocus == this || IsChild(pWndFocus) )
				)
				SyncToolbarObjButtons();
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}


BOOL CDrwDocView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	// do the scroll
	if (!CScrollView::OnScrollBy(sizeScroll, bDoScroll))
		return FALSE;

	// update the position of any in-place active item
	if (bDoScroll)
	{
		UpdateActiveItem();
		UpdateWindow();
	}
	return TRUE;
}



void CDrwDocView::SetZoom(int Zoom)
{
	nZoom = Zoom;
	Invalidate();
	SetPageSize(GetDocument()->GetPaperSize());
}

void CDrwDocView::SetUpDCZoom(CDC *pDC,int Zoom)
{
	CSize vport = pDC->GetViewportExt();
	pDC->SetViewportExt(MulDiv(vport.cx,Zoom,100),
		                MulDiv(vport.cy,Zoom,100));

}

	void CDrwDocView::Remove(CDocObj* pObj)
	{
		POSITION pos = m_selection.Find(pObj);
		if (pos != NULL)
			m_selection.RemoveAt(pos);
	}

	void CDrwDocView::PasteNative(COleDataObject& dataObject)
	{
		// get file refering to clipboard data
		CFile* pFile = dataObject.GetFileData(m_cfDraw);
		if (pFile == NULL)
			return;

		// connect the file to the archive
		CArchive ar(pFile, CArchive::load);
		TRY
		{
			ar.m_pDocument = GetDocument(); // set back-pointer in archive

			// read the selection
			m_selection.Serialize(ar);
		}
		CATCH_ALL(e)
		{
			ar.Close();
			delete pFile;
			THROW_LAST();
		}
		END_CATCH_ALL

		ar.Close();
		delete pFile;
	}

	void CDrwDocView::PasteEmbedded(COleDataObject& dataObject, CPoint point )
	{
		BeginWaitCursor();

		// paste embedded
		CDrwDocOleObj* pObj = new CDrwDocOleObj(GetInitialPosition());
		ASSERT_VALID(pObj);
		CDocOleItem* pItem = new CDocOleItem(GetDocument(), pObj);
		ASSERT_VALID(pItem);
		pObj->SetOleItem(pItem);

		TRY
		{
			if (!pItem->CreateFromData(&dataObject) &&
				!pItem->CreateStaticFromData(&dataObject))
			{
				AfxThrowMemoryException();      // any exception will do
			}

			// add the object to the document
			GetDocument()->Add(pObj);
			m_selection.AddTail(pObj);
			ClientToDoc( point );
			pObj->MoveTo( CRect( point, pObj->GetExtent() ), this );

			// try to get initial presentation data
			pItem->UpdateLink();
			pItem->UpdateExtent();
		}
		CATCH_ALL(e)
		{
			// clean up item
			pItem->Delete();
			pObj->m_pClientItem = NULL;
			GetDocument()->Remove(pObj);
			pObj->Remove();

			AfxMessageBox(IDP_FAILED_TO_CREATE);
		}
		END_CATCH_ALL

		EndWaitCursor();
	}


	void CDrwDocView::DrawHelp(CDC* pDC)
	{
		CDrwDoc* pDoc = GetDocument();

		COLORREF oldBkColor = pDC->SetBkColor(pDoc->GetPaperColor());

		CRect rect;
		CBrush *oldBrush = NULL;

		rect.left = -pDoc->GetPaperSize().cx / 2;
		rect.top = pDoc->GetPaperSize().cy / 2;
		rect.right = rect.left + pDoc->GetPaperSize().cx;
		rect.bottom = rect.top - pDoc->GetPaperSize().cy;

	if(m_bBackDoc)
	{
		CSize paperSize = pDoc->GetPaperSize();		
		m_BackDoc.Draw(pDC,CPoint(-paperSize.cx / 2,paperSize.cy / 2),CSize(paperSize.cx,-paperSize.cy));		
	}
		oldBrush = (CBrush*)pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&rect);

		


		rect.left = -pDoc->GetPageSize().cx / 2;
		rect.top = -pDoc->GetPageSize().cy / 2;
		rect.right = rect.left + pDoc->GetPageSize().cx;
		rect.bottom = rect.top + pDoc->GetPageSize().cy;


	if(m_bGrid)
	{
		// Center lines
		CPen penDash;
		penDash.CreatePen(PS_DASH, 1, m_gridColor);
		CPen* pOldPen = pDC->SelectObject(&penDash);



		pDC->MoveTo(0, rect.top);
		pDC->LineTo(0, rect.bottom);
		pDC->MoveTo(rect.left, 0);
		pDC->LineTo(rect.right, 0);

		// Major unit lines
		CPen penDot;
		penDot.CreatePen(PS_DOT, 1, m_gridColor);
		pDC->SelectObject(&penDot);

		for (int x = rect.left / 100 * 100; x < rect.right; x += 100)
		{
			if (x != 0)
			{
				pDC->MoveTo(x, rect.top);
				pDC->LineTo(x, rect.bottom);
			}
		}

		for (int y = rect.top / 100 * 100; y < rect.bottom; y += 100)
		{
			if (y != 0)
			{
				pDC->MoveTo(rect.left, y);
				pDC->LineTo(rect.right, y);
			}
		}
		
		
		pDC->SelectObject(pOldPen);
	}
		
		pDC->SetBkColor(oldBkColor);
	}


	BOOL CDrwDocView::IsSelected(const CObject* pDocItem) const
	{
		CDocObj* pDrawObj = (CDocObj*)pDocItem;
		if (pDocItem->IsKindOf(RUNTIME_CLASS(CDrawItem)))
			pDrawObj = ((CDrawItem*)pDocItem)->m_pDrawObj;
		return m_selection.Find(pDrawObj) != NULL;
	}

	void CDrwDocView::OnInsertObject()
	{
		// Invoke the standard Insert Object dialog box to obtain information
		//  for new CDrawItem object.
		COleInsertDialog dlg;
		if (dlg.DoModal() != IDOK)
			return;

		BeginWaitCursor();

		// First create the C++ object
		CDrawOleObj* pObj = new CDrawOleObj(GetInitialPosition());
		ASSERT_VALID(pObj);
		CDrawItem* pItem = new CDrawItem(GetDocument(), pObj);
		ASSERT_VALID(pItem);
		pObj->m_pClientItem = pItem;

		// Now create the OLE object/item
		TRY
		{
			if (!dlg.CreateItem(pObj->m_pClientItem))
				AfxThrowMemoryException();

			// add the object to the document
			GetDocument()->Add(pObj);

			// try to get initial presentation data
			pItem->UpdateLink();
			pItem->UpdateExtent();

			// if insert new object -- initially show the object
			if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
				pItem->DoVerb(OLEIVERB_SHOW, this);
		}
		CATCH_ALL(e)
		{
			// clean up item
			pItem->Delete();
			pObj->m_pClientItem = NULL;
			GetDocument()->Remove(pObj);
			pObj->Remove();

			AfxMessageBox(IDP_FAILED_TO_CREATE);
		}
		END_CATCH_ALL

		EndWaitCursor();
	}

	// The following command handler provides the standard keyboard
	//  user interface to cancel an in-place editing session.
	void CDrwDocView::OnCancelEdit()
	{
		// deactivate any in-place active item on this view!
		COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
		if (pActiveItem != NULL)
		{
			// if we found one, deactivate it
			pActiveItem->Close();
		}
		ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);

		// escape also brings us back into select mode
		ReleaseCapture();

		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnCancel();

		CDrawTool::c_drawShape = selection;
	}

	void CDrwDocView::OnSetFocus(CWnd* pOldWnd)
	{
		COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
		if (pActiveItem != NULL &&
			pActiveItem->GetItemState() == COleClientItem::activeUIState)
		{
			// need to set focus to this item if it is in the same view
			CWnd* pWnd = pActiveItem->GetInPlaceWindow();
			if (pWnd != NULL)
			{
				pWnd->SetFocus();
				return;
			}
		}

		CScrollView::OnSetFocus(pOldWnd);
	}

	CRect CDrwDocView::GetInitialPosition()
	{
		CRect rect(10, 10, 10, 10);
		ClientToDoc(rect);
		return rect;
	}

	void CDrwDocView::Select(CDocObj* pObj, BOOL bAdd)
	{
		if (!bAdd)
		{
			OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
			m_selection.RemoveAll();
		}

		if (pObj == NULL || IsSelected(pObj))
			return;

		m_selection.AddTail(pObj);
		InvalObj(pObj);
	}

	// rect is in device coordinates
	void CDrwDocView::SelectWithinRect(CRect rect, BOOL bAdd)
	{
		if (!bAdd)
			Select(NULL);

		ClientToDoc(rect);

		CDrawObjList* pObList = GetDocument()->GetObjects();
		POSITION posObj = pObList->GetHeadPosition();
		while (posObj != NULL)
		{
			CDocObj* pObj = pObList->GetNext(posObj);
			if (pObj->Intersects(rect))
				if(pObj->m_group)
				{
					if(pObj->m_group->m_PrintVis)
						Select(pObj, TRUE);
				}
				else Select(pObj, TRUE);
		}
	}

	void CDrwDocView::Deselect(CDocObj* pObj)
	{
		POSITION pos = m_selection.Find(pObj);
		if (pos != NULL)
		{
			InvalObj(pObj);
			m_selection.RemoveAt(pos);
		}
	}

	void CDrwDocView::CloneSelection()
	{
		POSITION pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			CDocObj* pObj = m_selection.GetNext(pos);
			pObj->Clone(pObj->m_pDocument);
				// copies object and adds it to the document
		}
	}

	void CDrwDocView::UpdateActiveItem()
	{
		COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
		if (pActiveItem != NULL &&
			pActiveItem->GetItemState() == COleClientItem::activeUIState)
		{
			// this will update the item rectangles by calling
			//  OnGetPosRect & OnGetClipRect.
			pActiveItem->SetItemRects();
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// CDrwDocView message handlers

	void CDrwDocView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (!m_bActive)
			return;
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnLButtonDown(this, nFlags, point);
		SyncToolbarObjButtons();
	}

	void CDrwDocView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		if (!m_bActive)
			return;
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnLButtonUp(this, nFlags, point);
		SyncToolbarObjButtons();
	}

	void CDrwDocView::OnMouseMove(UINT nFlags, CPoint point)
	{
		if (!m_bActive)
			return;

		CMainFrame* pFrame = (CMainFrame*)GetMainFrame();

		CPoint pos = point;
		ClientToDoc(pos);
		char buffPos[50];
		sprintf(buffPos,"x:%d , y:%d",pos.x,pos.y);
		pFrame->m_wndStatusBar.SetPaneText(2,buffPos);


		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnMouseMove(this, nFlags, point);
	}

	void CDrwDocView::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		if (!m_bActive)
			return;
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnLButtonDblClk(this, nFlags, point);
	}

	void CDrwDocView::OnDestroy()
	{
		CScrollView::OnDestroy();

		// deactivate the inplace active item on this view
		COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
		if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
		{
			pActiveItem->Deactivate();
			ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
		}
	}


	void CDrwDocView::OnNewPage()
	{


		GetDocument()->NewPage();
			Invalidate();

		
	}


	void CDrwDocView::OnNextPage()
	{
		GetDocument()->NextPage();
			Invalidate();
		
	}


	void CDrwDocView::OnOpenBackImg()
	{

		CFileDialog l_openimg(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,
		  "Windows Bitmap (*.bmp)|*.bmp|JPEG Compressed Image (*.jpg)|*.jpg|TrueVision Targa (*.tga)|*.tga|TIFF (*.tif)|*.tga||");
	
		if(l_openimg.DoModal() == IDOK)
		{

		CString imgfile = l_openimg.GetPathName();
		if(!LoadDocImage((LPCSTR)imgfile))
			MessageBox("B³¹d , nie poprawny format pliku","B³¹d");
		}	
	
		Invalidate();

	}

	void CDrwDocView::OnPrevPage()
	{
		GetDocument()->PrevPage();
			Invalidate();
	}


	void CDrwDocView::OnDelPage()
	{

		GetDocument()->DelPage();
			Invalidate();
		
	}


	void CDrwDocView::OnDrawSelect()
	{
		CDrawTool::c_drawShape = selection;
	}

	void CDrwDocView::OnDrawRoundRect()
	{
		CDrawTool::c_drawShape = roundRect;
	}

	void CDrwDocView::OnDrawRect()
	{
		CDrawTool::c_drawShape = rect;
	}

	void CDrwDocView::OnDrawLine()
	{
		CDrawTool::c_drawShape = line;
	}

	void CDrwDocView::OnDrawEllipse()
	{
		CDrawTool::c_drawShape = ellipse;
	}

	void CDrwDocView::OnDrawPolygon()
	{
		CDrawTool::c_drawShape = poly;
	}

	void CDrwDocView::OnUpdateDrawEllipse(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == ellipse);
	}

	void CDrwDocView::OnUpdateDrawLine(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == line);
	}

	void CDrwDocView::OnUpdateDrawRect(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == rect);
	}

	void CDrwDocView::OnUpdateDrawRoundRect(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == roundRect);
	}

	void CDrwDocView::OnUpdateDrawSelect(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == selection);
	}

	void CDrwDocView::OnUpdateSingleSelect(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(m_selection.GetCount() == 1);
	}


	void CDrwDocView::OnDrawText()
	{
		CDrawTool::c_drawShape = text;
	}

	void CDrwDocView::OnDrawImage()
	{
		CDrawTool::c_drawShape = image;
	}

	void CDrwDocView::OnUpdateDrawText(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == text);
	}

	void CDrwDocView::OnUpdateDrawImage(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == image);
	}

	void CDrwDocView::OnEditSelectAll()
	{
		CDrawObjList* pObList = GetDocument()->GetObjects();
		POSITION pos = pObList->GetHeadPosition();
		while (pos != NULL)
			Select(pObList->GetNext(pos), TRUE);
	}

	void CDrwDocView::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(GetDocument()->GetObjects()->GetCount() != 0);
	}

	void CDrwDocView::OnEditClear()
	{
		// update all the views before the selection goes away
		GetDocument()->UpdateAllViews(NULL, HINT_DELETE_SELECTION, &m_selection);
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);

		// now remove the selection from the document
		POSITION pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			CDocObj* pObj = m_selection.GetNext(pos);
			GetDocument()->Remove(pObj);
			pObj->Remove();
		}
		m_selection.RemoveAll();
	}

	void CDrwDocView::OnUpdateAnySelect(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(!m_selection.IsEmpty());
	}

	void CDrwDocView::OnUpdateDrawPolygon(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == poly);
	}

	void CDrwDocView::OnSize(UINT nType, int cx, int cy)
	{
		CScrollView::OnSize(nType, cx, cy);
		UpdateActiveItem();
	}

	void CDrwDocView::OnViewGrid()
	{
		m_bGrid = !m_bGrid;
		Invalidate(FALSE);
	}

	void CDrwDocView::OnUpdateViewGrid(CCmdUI* pCmdUI)
	{
		pCmdUI->SetCheck(m_bGrid);

	}

	void CDrwDocView::OnViewBackDoc()
	{
		m_bBackDoc = !m_bBackDoc;
		Invalidate(FALSE);
	}

	void CDrwDocView::OnUpdateViewBackDoc(CCmdUI* pCmdUI)
	{
		pCmdUI->SetCheck(m_bBackDoc);

	}

	BOOL CDrwDocView::OnEraseBkgnd(CDC*)
	{
		return TRUE;
	}


	void CDrwDocView::OnObjectFillColor()
	{
	#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

	int nSelCount = m_selection.GetCount();
		if( nSelCount == 0 )
		{
			ASSERT( FALSE ); // should be disabled
			return;
		}
	POSITION pos = m_selection.GetHeadPosition();
		ASSERT( pos != NULL );
	CDocObj * pObj = m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
	CExtColorDlg dlg;
		dlg.m_clrInit
			= dlg.m_clrNew
			= pObj->GetFillColor();
		VERIFY(
			dlg.m_strCaption.LoadString( ID_OBJECT_FILLCOLOR )
			);
		if( dlg.DoModal() != IDOK )
			return;
		pos = m_selection.GetHeadPosition();
		ASSERT( pos != NULL );
		while( pos != NULL )
		{
			pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
			pObj->SetNoFill( FALSE );
			pObj->SetFillColor( dlg.m_clrNew );
		}

	#else

		CColorDialog dlg;
		if (dlg.DoModal() != IDOK)
			return;

		COLORREF color = dlg.GetColor();

		POSITION pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			CDocObj* pObj = m_selection.GetNext(pos);
			pObj->SetNoFill( FALSE );
			pObj->SetFillColor(color);
		}

	#endif

		SyncToolbarObjButtons();
	}

	void CDrwDocView::OnObjectLineColor()
	{
	#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

	int nSelCount = m_selection.GetCount();
		if( nSelCount == 0 )
		{
			ASSERT( FALSE ); // should be disabled
			return;
		}
	POSITION pos = m_selection.GetHeadPosition();
		ASSERT( pos != NULL );
	CDocObj * pObj = m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
	CExtColorDlg dlg;
		dlg.m_clrInit
			= dlg.m_clrNew
			= pObj->GetLineColor();
		VERIFY(
			dlg.m_strCaption.LoadString( ID_OBJECT_LINECOLOR )
			);
		if( dlg.DoModal() != IDOK )
			return;
		pos = m_selection.GetHeadPosition();
		ASSERT( pos != NULL );
		while( pos != NULL )
		{
			pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
			pObj->SetNoOutline( FALSE );
			pObj->SetLineColor( dlg.m_clrNew );
		}

	#else

		CColorDialog dlg;
		if (dlg.DoModal() != IDOK)
			return;

		COLORREF color = dlg.GetColor();

		POSITION pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			CDocObj* pObj = m_selection.GetNext(pos);
			pObj->SetNoOutline( FALSE );
			pObj->SetLineColor(color);
		}

	#endif

		SyncToolbarObjButtons();
	}

	void CDrwDocView::OnObjectMoveBack()
	{
		CDrawDoc* pDoc = GetDocument();
		CDocObj* pObj = m_selection.GetHead();
		CDrawObjList* pObjects = pDoc->GetObjects();
		POSITION pos = pObjects->Find(pObj);
		ASSERT(pos != NULL);
		if (pos != pObjects->GetHeadPosition())
		{
			POSITION posPrev = pos;
			pObjects->GetPrev(posPrev);
			pObjects->RemoveAt(pos);
			pObjects->InsertBefore(posPrev, pObj);
			InvalObj(pObj);
		}
	}

	void CDrwDocView::OnObjectMoveForward()
	{
		CDrawDoc* pDoc = GetDocument();
		CDocObj* pObj = m_selection.GetHead();
		CDrawObjList* pObjects = pDoc->GetObjects();
		POSITION pos = pObjects->Find(pObj);
		ASSERT(pos != NULL);
		if (pos != pObjects->GetTailPosition())
		{
			POSITION posNext = pos;
			pObjects->GetNext(posNext);
			pObjects->RemoveAt(pos);
			pObjects->InsertAfter(posNext, pObj);
			InvalObj(pObj);
		}
	}

	void CDrwDocView::OnObjectMoveToBack()
	{
		CDrawDoc* pDoc = GetDocument();
		CDocObj* pObj = m_selection.GetHead();
		CDrawObjList* pObjects = pDoc->GetObjects();
		POSITION pos = pObjects->Find(pObj);
		ASSERT(pos != NULL);
		pObjects->RemoveAt(pos);
		pObjects->AddHead(pObj);
		InvalObj(pObj);
	}

	void CDrwDocView::OnObjectMoveToFront()
	{
		CDrawDoc* pDoc = GetDocument();
		CDocObj* pObj = m_selection.GetHead();
		CDrawObjList* pObjects = pDoc->GetObjects();
		POSITION pos = pObjects->Find(pObj);
		ASSERT(pos != NULL);
		pObjects->RemoveAt(pos);
		pObjects->AddTail(pObj);
		InvalObj(pObj);
	}

	void CDrwDocView::OnEditCopy()
	{
		ASSERT_VALID(this);
		ASSERT(m_cfDraw != NULL);

		// Create a shared file and associate a CArchive with it
		CSharedFile file;
		CArchive ar(&file, CArchive::store);

		// Serialize selected objects to the archive
		m_selection.Serialize(ar);
		ar.Close();
		
	//	CFile clipboard("clip.dat",CFile::modeCreate|CFile::modeWrite);
	

		COleDataSource* pDataSource = NULL;
		TRY
		{
			pDataSource = new COleDataSource;
			// put on local format instead of or in addation to
			pDataSource->CacheGlobalData(m_cfDraw, file.Detach());

			// if only one item and it is a COleClientItem then also
			// paste in that format
			CDocObj* pDrawObj = m_selection.GetHead();
			if (m_selection.GetCount() == 1 &&
				pDrawObj->IsKindOf(RUNTIME_CLASS(CDrawOleObj)))
			{
				CDrawOleObj* pDrawOle = (CDrawOleObj*)pDrawObj;
				pDrawOle->m_pClientItem->GetClipboardData(pDataSource, FALSE);
			}
			pDataSource->SetClipboard();
		}
		CATCH_ALL(e)
		{
			delete pDataSource;
			THROW_LAST();
		}
		END_CATCH_ALL
	}

	void CDrwDocView::OnUpdateEditCopy(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(!m_selection.IsEmpty());
	}

	void CDrwDocView::OnEditCut()
	{
		OnEditCopy();
		OnEditClear();
	}

	void CDrwDocView::OnUpdateEditCut(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(!m_selection.IsEmpty());
	}

	void CDrwDocView::OnEditPaste()
	{
		COleDataObject dataObject;
		dataObject.AttachClipboard();

		// invalidate current selection since it will be deselected
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_selection.RemoveAll();
		if (dataObject.IsDataAvailable(m_cfDraw))
		{
			PasteNative(dataObject);

			// now add all items in m_selection to document
			POSITION pos = m_selection.GetHeadPosition();
			while (pos != NULL)
				GetDocument()->Add(m_selection.GetNext(pos));
		}
		else
			PasteEmbedded(dataObject, GetInitialPosition().TopLeft() );

		GetDocument()->SetModifiedFlag();

		// invalidate new pasted stuff
		GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_SELECTION, &m_selection);
	}

	void CDrwDocView::OnUpdateEditPaste(CCmdUI* pCmdUI)
	{
		// determine if private or standard OLE formats are on the clipboard
		COleDataObject dataObject;
		BOOL bEnable = dataObject.AttachClipboard() &&
			(dataObject.IsDataAvailable(m_cfDraw) ||
			 COleClientItem::CanCreateFromData(&dataObject));

		// enable command based on availability
		pCmdUI->Enable(bEnable);
	}

	void CDrwDocView::OnViewShowObjects()
	{
		CDrawOleObj::c_bShowItems = !CDrawOleObj::c_bShowItems;
		GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_OLE_ITEMS, NULL);
	}

	void CDrwDocView::OnUpdateViewShowObjects(CCmdUI* pCmdUI)
	{
		pCmdUI->SetCheck(CDrawOleObj::c_bShowItems);
	}

	void CDrwDocView::OnEditProperties()
	{
		if (m_selection.GetCount() == 1 && CDrawTool::c_drawShape == selection)
		{
			CDrawTool* pTool =
				CDrawTool::FindTool(
					CDrawTool::c_drawShape
					);
			ASSERT(pTool != NULL);
			pTool->OnEditProperties(this);
		}
	}

	void CDrwDocView::OnUpdateGroupProp(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(m_selection.GetCount() > 1 && CDrawTool::c_drawShape == selection);
		

	}

	void CDrwDocView::OnUpdateEditProperties(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(m_selection.GetCount() == 1 &&
					   CDrawTool::c_drawShape == selection);
	}
	


void CDrwDocView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CScrollView::OnEndPrintPreview(pDC, pInfo, point, pView);	
}


	int CDrwDocView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CScrollView::OnCreate(lpCreateStruct) == -1)
			return -1;

		
		// register drop target
		if( m_dropTarget.Register( this ) )
			return 0;
		else
			return -1;
		
	}
	BOOL CDrwDocView::GetObjectInfo(COleDataObject* pDataObject,CSize* pSize, CSize* pOffset)
	{
		ASSERT(pSize != NULL);

		// get object descriptor data
		HGLOBAL hObjDesc = pDataObject->GetGlobalData(m_cfObjectDescriptor);
		if (hObjDesc == NULL)
		{
			if (pOffset != NULL)
				*pOffset = CSize(0, 0); // fill in defaults instead
			*pSize = CSize(0, 0);
			return FALSE;
		}
		ASSERT(hObjDesc != NULL);

		// otherwise, got CF_OBJECTDESCRIPTOR ok.  Lock it down and extract size.
		LPOBJECTDESCRIPTOR pObjDesc = (LPOBJECTDESCRIPTOR)GlobalLock(hObjDesc);
		ASSERT(pObjDesc != NULL);
		pSize->cx = (int)pObjDesc->sizel.cx;
		pSize->cy = (int)pObjDesc->sizel.cy;
		if (pOffset != NULL)
		{
			pOffset->cx = (int)pObjDesc->pointl.x;
			pOffset->cy = (int)pObjDesc->pointl.y;
		}
		GlobalUnlock(hObjDesc);
		GlobalFree(hObjDesc);

		// successfully retrieved pSize & pOffset info
		return TRUE;
	}

	DROPEFFECT CDrwDocView::OnDragEnter(COleDataObject* pDataObject,
		DWORD grfKeyState, CPoint point)
	{
		ASSERT(m_prevDropEffect == DROPEFFECT_NONE);
		m_bDragDataAcceptable = FALSE;
		if (!COleClientItem::CanCreateFromData(pDataObject))
			return DROPEFFECT_NONE;

		m_bDragDataAcceptable = TRUE;
		GetObjectInfo(pDataObject, &m_dragSize, &m_dragOffset);
		CClientDC dc(NULL);
		dc.HIMETRICtoDP(&m_dragSize);
		dc.HIMETRICtoDP(&m_dragOffset);

		return OnDragOver(pDataObject, grfKeyState, point);
	}

	DROPEFFECT CDrwDocView::OnDragOver(COleDataObject*,
		DWORD grfKeyState, CPoint point)
	{
		if(m_bDragDataAcceptable == FALSE)
			return DROPEFFECT_NONE;

		point -= m_dragOffset;  // adjust target rect by original cursor offset

		// check for point outside logical area -- i.e. in hatched region
		// GetTotalSize() returns the size passed to SetScrollSizes
		CRect rectScroll(CPoint(0, 0), GetTotalSize());

		CRect rectItem(point,m_dragSize);
		rectItem.OffsetRect(GetDeviceScrollPosition());

		DROPEFFECT de = DROPEFFECT_NONE;
		CRect rectTemp;
		if (rectTemp.IntersectRect(rectScroll, rectItem))
		{
			// check for force link
			if ((grfKeyState & (MK_CONTROL|MK_SHIFT)) == (MK_CONTROL|MK_SHIFT))
				de = DROPEFFECT_NONE; // DRAWCLI isn't a linking container
			// check for force copy
			else if ((grfKeyState & MK_CONTROL) == MK_CONTROL)
				de = DROPEFFECT_COPY;
			// check for force move
			else if ((grfKeyState & MK_ALT) == MK_ALT)
				de = DROPEFFECT_MOVE;
			// default -- recommended action is move
			else
				de = DROPEFFECT_MOVE;
		}

		if (point == m_dragPoint)
			return de;

		// otherwise, cursor has moved -- need to update the drag feedback
		CClientDC dc(this);
		if (m_prevDropEffect != DROPEFFECT_NONE)
		{
			// erase previous focus rect
			dc.DrawFocusRect(CRect(m_dragPoint, m_dragSize));
		}
		m_prevDropEffect = de;
		if (m_prevDropEffect != DROPEFFECT_NONE)
		{
			m_dragPoint = point;
			dc.DrawFocusRect(CRect(point, m_dragSize));
		}
		return de;
	}

	BOOL CDrwDocView::OnDrop(COleDataObject* pDataObject,
		DROPEFFECT /*dropEffect*/, CPoint point)
	{
		ASSERT_VALID(this);

		// clean up focus rect
		OnDragLeave();

		// offset point as appropriate for dragging
		GetObjectInfo(pDataObject, &m_dragSize, &m_dragOffset);
		CClientDC dc(NULL);
		dc.HIMETRICtoDP(&m_dragSize);
		dc.HIMETRICtoDP(&m_dragOffset);
		point -= m_dragOffset;

		// invalidate current selection since it will be deselected
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_selection.RemoveAll();
		if (m_bDragDataAcceptable)
			PasteEmbedded(*pDataObject, point);

		// update the document and views
		GetDocument()->SetModifiedFlag();
		GetDocument()->UpdateAllViews(NULL, 0, NULL);      // including this view

		return TRUE;
	}

	void CDrwDocView::OnDragLeave()
	{
		CClientDC dc(this);
		if (m_prevDropEffect != DROPEFFECT_NONE)
		{
			dc.DrawFocusRect(CRect(m_dragPoint,m_dragSize)); // erase previous focus rect
			m_prevDropEffect = DROPEFFECT_NONE;
		}
	}


	void CDrwDocView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
	{
	CDrawDoc * pDoc = GetDocument();
		ASSERT_VALID( pDoc );

		// make sure window is active
		GetParentFrame()->ActivateFrame();

	CPoint ptLocal = point;
		ScreenToClient( &ptLocal );
		ClientToDoc( ptLocal );

	bool bTryProcessItemFocus = !( (point.x < 0) || (point.y < 0) );
	CDocObj * pObj = NULL;
		if( bTryProcessItemFocus )
		{
			pObj = GetDocument()->ObjectAt( ptLocal );
			if( pObj == NULL )
				return;
			if( !IsSelected(pObj) )
				Select( pObj, FALSE ); // reselect item if appropriate
			Invalidate();
			UpdateWindow();
		} // if( bTryProcessItemFocus )
		else
		{
			POSITION pos = m_selection.GetHeadPosition();
			if( pos == NULL )
				return;
			pObj = m_selection.GetNext( pos );
		} // else from if( bTryProcessItemFocus )

		ASSERT( pObj != NULL );

		SyncToolbarObjButtons();

	CMenu menu;
		if( !menu.LoadMenu(ID_POPUP_MENU) )
		{
			ASSERT( FALSE );
			return;
		}

	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();
		ASSERT( hWnd != NULL );
		ASSERT( ::IsWindow( hWnd ) );

	CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
		VERIFY( pPopup->UpdateFromMenu( hWnd, &menu ) );

		VERIFY(
			pPopup->TrackPopupMenu( TPMX_OWNERDRAW_FIXED, point.x, point.y )
			);
	}



	LRESULT CDrwDocView::OnExtMenuPrepare(WPARAM wParam, LPARAM lParam)
	{
		lParam;
	CExtPopupMenuWnd::MsgPrepareMenuData_t * pData =
			reinterpret_cast
				< CExtPopupMenuWnd::MsgPrepareMenuData_t * >
				( wParam );
		ASSERT( pData != NULL );
	CExtPopupMenuWnd * pPopup = pData->m_pPopup;
		ASSERT( pPopup != NULL );

	CDrawDoc * pDoc = GetDocument();
		ASSERT_VALID( pDoc );

	CDocObj * pObj = NULL;
	POSITION pos = m_selection.GetHeadPosition();
		if( pos != NULL )
		{
			pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
		}

	INT nReplacePos =
			pPopup->ItemFindPosForCmdID(ID_OLE_VERB_FIRST);
		for( ;	nReplacePos >= 0;
				nReplacePos =
					pPopup->ItemFindPosForCmdID(ID_OLE_VERB_FIRST)
	/////nReplacePos = -1
			)
		{ // for all ole verb start's ?!?!
			CDrawOleObj * pDrawOleObj =
				DYNAMIC_DOWNCAST(CDrawOleObj,pObj);
			if( pDrawOleObj != NULL )
			{ // if menu for OLE object
				// delete OLE verb entry
				VERIFY( pPopup->ItemRemove(nReplacePos) );
				CMenu menu;
				VERIFY( menu.CreatePopupMenu() );
				ASSERT( menu.GetSafeHmenu() != NULL );
				CMenu * pMenuPopup = &menu; // menu.GetSubMenu(0);
				//ASSERT( pMenuPopup->GetSafeHmenu() != NULL );
				VERIFY(pMenuPopup->AppendMenu( 0, ID_OLE_VERB_FIRST ));
				
				UINT nConvertID = ID_OLE_EDIT_CONVERT;
				AFX_CMDHANDLERINFO info;
				if( !pDoc->OnCmdMsg(ID_OLE_EDIT_CONVERT, CN_COMMAND, NULL, &info) )
					nConvertID = 0;
				ASSERT( pDrawOleObj->m_pClientItem != NULL );
				ASSERT( ::IsMenu( pMenuPopup->GetSafeHmenu() ) );
				AfxOleSetEditMenu(
					pDrawOleObj->m_pClientItem,
					pMenuPopup, // &menu,
					pMenuPopup->GetMenuItemCount() - 1, // First OLE cmd
					ID_OLE_VERB_FIRST,
					ID_OLE_VERB_LAST,
					nConvertID
					);
				
				const CExtCmdIcon & _iconOLE =
				g_CmdManager->CmdGetIcon(
					g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
					ID_OLE_INSERT_NEW
					);

				
				CString sOlePopupText;
				pMenuPopup->GetMenuString(0,sOlePopupText,MF_BYPOSITION);
				ASSERT( !sOlePopupText.IsEmpty() );
				VERIFY(
					pPopup->ItemInsert(
						(UINT)CExtPopupMenuWnd::TYPE_POPUP,
						nReplacePos,
						(LPCTSTR)sOlePopupText,
						_iconOLE
						));
				pPopup->ItemSetDisplayed( nReplacePos, true );
				CExtPopupMenuWnd * pOlePopup =
					pPopup->ItemGetPopup(
						nReplacePos
						);
				ASSERT( pOlePopup != NULL );
				if( pOlePopup != NULL )
				{
					VERIFY(
						pOlePopup->UpdateFromMenu(
							AfxGetMainWnd()->GetSafeHwnd(),
							&menu,
							true,
							false
							)
						);
				} // if( pOlePopup != NULL )
			} // if menu for OLE object
			else
			{ // if menu for internal object
				// delete OLE verb entry
				VERIFY( pPopup->ItemRemove(nReplacePos) );
				if( nReplacePos > 0
					&& pPopup->ItemGetCmdID(nReplacePos-1) == ID_SEPARATOR
					)
				{	// delete separator befire OLE verb entry
					VERIFY( pPopup->ItemRemove(nReplacePos-1) );
				}	// delete separator befire OLE verb entry
			} // if menu for internal object
		} // for all ole verb start's ?!?!

		if( pObj != NULL )
		{
			CExtPopupColorMenuWnd * pColorPopup;
			CString sNoColor;

			nReplacePos =
				pPopup->ItemFindPosForCmdID(ID_OBJECT_FILLCOLOR);
			if( nReplacePos >= 0 )
			{
				VERIFY( sNoColor.LoadString(IDS_STRING_NO_COLOR_FILL) );
				ASSERT( !sNoColor.IsEmpty() );
				pColorPopup = new CExtPopupColorMenuWnd;
				pColorPopup->m_sBtnTextColorDefault = sNoColor;
				pColorPopup->m_lParamCookie = LPARAM(ID_OBJECT_FILLCOLOR);
				pColorPopup->m_hWndNotifyColorChanged = GetSafeHwnd();
				pColorPopup->m_clrDefault = COLORREF(-1);
				pColorPopup->m_clrInitial =
					pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor();
				VERIFY(
					pPopup->ItemInsertSpecPopup(
						pColorPopup,
						nReplacePos + 1,
						pPopup->ItemGetText(nReplacePos),
						pPopup->ItemGetIcon(nReplacePos)
						)
					);
				pPopup->ItemSetDisplayed( nReplacePos + 1, true );
				VERIFY( pPopup->ItemRemove(nReplacePos) );
			} // if( nReplacePos >= 0 )

			nReplacePos =
				pPopup->ItemFindPosForCmdID(ID_OBJECT_LINECOLOR);
			if( nReplacePos >= 0 )
			{
				VERIFY( sNoColor.LoadString(IDS_STRING_NO_COLOR_OUTLINE) );
				ASSERT( !sNoColor.IsEmpty() );
				pColorPopup = new CExtPopupColorMenuWnd;
				pColorPopup->m_sBtnTextColorDefault = sNoColor;
				//pColorPopup->m_bEnableBtnColorDefault = false;
				pColorPopup->m_lParamCookie = LPARAM(ID_OBJECT_LINECOLOR);
				pColorPopup->m_hWndNotifyColorChanged = GetSafeHwnd();
				pColorPopup->m_clrDefault = COLORREF(-1);
				pColorPopup->m_clrInitial =
					pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor();
				VERIFY(
					pPopup->ItemInsertSpecPopup(
						pColorPopup,
						nReplacePos + 1,
						pPopup->ItemGetText(nReplacePos),
						pPopup->ItemGetIcon(nReplacePos)
						)
					);
				pPopup->ItemSetDisplayed( nReplacePos + 1, true );
				VERIFY( pPopup->ItemRemove(nReplacePos) );
			} // if( nReplacePos >= 0 )

		} // if( pObj != NULL )

		nReplacePos = pPopup->ItemFindByText( _T("Line &Width...") );
		if( nReplacePos >= 0 )
		{
				const CExtCmdIcon & _icon =
			g_CmdManager->CmdGetIcon(
				g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() ),
				ID_DOC_LINE_WIDTH
				);
			pPopup->ItemSetPopupIcon(nReplacePos, _icon );
		}
		

		return TRUE;
	}

	LRESULT CDrwDocView::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
	{
	POSITION pos = m_selection.GetHeadPosition();
		ASSERT( pos != NULL );
		while( pos != NULL )
		{
			CDocObj * pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
			switch( lParam )
			{
			case ID_OBJECT_FILLCOLOR:
				if( COLORREF(wParam) == COLORREF(-1) )
					pObj->SetNoFill();
				else
				{
					pObj->SetNoFill( FALSE );
					pObj->SetFillColor( COLORREF(wParam) );
				}
			break;
			case ID_OBJECT_LINECOLOR:
				if( COLORREF(wParam) == COLORREF(-1) )
					pObj->SetNoOutline();
				else
				{
					pObj->SetNoOutline( FALSE );
					pObj->SetLineColor( COLORREF(wParam) );
				}
			break;
			} // switch( lParam )
		}
		Invalidate();
		UpdateWindow();
		SyncToolbarObjButtons();
		return 0;
	}

	LRESULT CDrwDocView::OnColorSelectCustom(WPARAM wParam, LPARAM lParam)
	{
		wParam;
		switch( lParam )
		{
		case ID_OBJECT_FILLCOLOR:
		case ID_OBJECT_LINECOLOR:
			PostMessage(WM_COMMAND,lParam,0);
		break;
		} // switch( lParam )
		return 0;
	}

	void CDrwDocView::OnUpdateSelectionSingle(CCmdUI* pCmdUI)
	{
	int nSelCount = m_selection.GetCount();
		pCmdUI->Enable( nSelCount == 1 );
	}

	void CDrwDocView::OnUpdateSelectionNonEmpty(CCmdUI* pCmdUI)
	{
	int nSelCount = m_selection.GetCount();
		pCmdUI->Enable( nSelCount > 0 );
	}

	void CDrwDocView::OnLineWidth( UINT nCmdID )
	{
		ASSERT( ID_LINE_WIDTH_0 <= nCmdID && nCmdID <= ID_LINE_WIDTH_OTHER );
	#ifdef _DEBUG
	int nSelCount = m_selection.GetCount();
		ASSERT( nSelCount > 0 );
	#endif // _DEBUG

	POSITION pos = m_selection.GetHeadPosition();
		ASSERT( pos != NULL );

		while( pos != NULL )
		{
			CDocObj * pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
		
			switch( nCmdID )
			{
			case ID_LINE_WIDTH_OTHER:
					ASSERT( nSelCount == 1 );
					if( CDrawTool::c_drawShape == selection )
						PostMessage( WM_COMMAND, ID_EDIT_PROPERTIES );
				return;
			case ID_LINE_WIDTH_0:
					pObj->SetNoOutline( TRUE );
				break;
			default:
				{
					ASSERT( ID_LINE_WIDTH_1 <= nCmdID && nCmdID <= ID_LINE_WIDTH_10 );
					UINT nLineWidth = nCmdID - ID_LINE_WIDTH_0;
					ASSERT( 1 <= nLineWidth && nLineWidth <= 10 );
					pObj->SetNoOutline( FALSE );
					pObj->m_logpen.lopnWidth.x = nLineWidth;
					pObj->m_logpen.lopnWidth.y = nLineWidth;
				}
				break;
			} // switch( nCmdID )

		} // while( pos != NULL )

		Invalidate();
	}

	void CDrwDocView::OnUpdateLineWidth( CCmdUI * pCmdUI )
	{
		ASSERT( pCmdUI != NULL );
	UINT nCmdID = pCmdUI->m_nID;
		ASSERT( ID_LINE_WIDTH_0 <= nCmdID && nCmdID <= ID_LINE_WIDTH_OTHER );
	int nSelCount = m_selection.GetCount();

	CDocObj * pObj = NULL;
		if( nSelCount > 0 )
		{
			POSITION pos = m_selection.GetHeadPosition();
			ASSERT( pos != NULL );
			pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
		}
		else
		{
			pCmdUI->Enable( FALSE );
			pCmdUI->SetRadio( FALSE );
		}

		if( nCmdID == ID_LINE_WIDTH_OTHER )
		{
			pCmdUI->Enable( nSelCount == 1 && m_selection.GetCount() == 1 && CDrawTool::c_drawShape == selection );
			if( pObj != NULL )
				pCmdUI->SetRadio( pObj->m_logpen.lopnWidth.x > 10 );
			return;
		}

		pCmdUI->Enable( nSelCount > 0 );
		
		if( nCmdID == ID_LINE_WIDTH_0 )
		{
			if( pObj != NULL )
				pCmdUI->SetRadio( pObj->IsNoOutline() );
			return;
		}

		ASSERT( ID_LINE_WIDTH_1 <= nCmdID && nCmdID <= ID_LINE_WIDTH_10 );
	UINT nLineWidth = nCmdID - ID_LINE_WIDTH_0;
		ASSERT( 1 <= nLineWidth && nLineWidth <= 10 );
		pCmdUI->Enable( nSelCount > 0 );
		if( pObj != NULL )
			pCmdUI->SetRadio( UINT(pObj->m_logpen.lopnWidth.x) == nLineWidth );

	}

	LRESULT CDrwDocView::OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam)
	{
		wParam;

	CExtPopupMenuWnd::DRAWITEMDATA * pDrawItemData =
			reinterpret_cast < CExtPopupMenuWnd::DRAWITEMDATA * > ( lParam );
		ASSERT( pDrawItemData != NULL );
	UINT nCmdID = pDrawItemData->GetCmdID();
		if( ! ( ID_LINE_WIDTH_1 <= nCmdID && nCmdID <= ID_LINE_WIDTH_10 ) )
			return FALSE; // default painting

	#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	const CExtCustomizeCmdTreeNode * pNode =
			pDrawItemData->m_pItemData->GetCmdNode();
		if( pNode != NULL )
		{
			ASSERT_VALID( pNode );
			if( (pNode->GetFlags() & __ECTN_TBB_SEPARATED_DROPDOWN) != 0 )
				return FALSE;
		} // if( pNode != NULL )
	#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)


		// paint default menu item background
		pDrawItemData->PaintDefault(
			false,
			true,
			false,
			false,
			false
			);


	UINT nLineWidth = nCmdID - ID_LINE_WIDTH_0;
		ASSERT( 1 <= nLineWidth && nLineWidth <= 10 );

	CRect rcItem = LPCRECT(*pDrawItemData);
		rcItem.DeflateRect( 26, 0, 2, 0 );
		rcItem.top += rcItem.Height()/2;
		rcItem.top -= nLineWidth/2;
		rcItem.bottom = rcItem.top + nLineWidth;
		
		// get draw DC
	CDC & dc = *( (CDC *) *pDrawItemData );

		dc.FillSolidRect(
			&rcItem,
			g_PaintManager->GetColor(
				(m_selection.GetCount() > 0)
					? COLOR_3DDKSHADOW
					: COLOR_3DSHADOW
					,
				this
				)
			);

		return TRUE;
	}

	void CDrwDocView::OnUpdateDocLineWidth(CCmdUI* pCmdUI) 
	{
		pCmdUI->Enable( m_selection.GetCount() > 0 );
	}


	void CDrwDocView::SyncToolbarObjButtons()
	{
	CFrameWnd * pFrame = GetParentFrame();
		ASSERT_VALID( pFrame );
		ASSERT_KINDOF( CMDIChildWnd, pFrame );
	CMainFrame * pMainFrame = (CMainFrame *)pFrame->GetParentFrame();
		ASSERT_VALID( pMainFrame );
		ASSERT_KINDOF( CMainFrame, pMainFrame );

	#if (defined __EXT_MFC_NO_CUSTOMIZE)
	int nSelCount = m_selection.GetCount();

	CDocObj * pObj = NULL;
		if( nSelCount > 0 )
		{
			POSITION pos = m_selection.GetHeadPosition();
			ASSERT( pos != NULL );
			pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
		}

		if( pObj != NULL )
		{
			if(		pMainFrame->m_pBtnColorFill != NULL
				&&	pMainFrame->m_pBtnColorFill->ColorGet(true)
						!= pObj->GetFillColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorFill );
				pMainFrame->m_pBtnColorFill->ColorSet(
					pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor(),
					false
					);
				pMainFrame->m_pBtnColorFill->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorFill->RedrawButton();
			}
			if(		pMainFrame->m_pBtnColorOutline != NULL
				&&	pMainFrame->m_pBtnColorOutline->ColorGet(false)
						!= pObj->GetLineColor()
				)
			{
				ASSERT_VALID( pMainFrame->m_pBtnColorOutline );
				pMainFrame->m_pBtnColorOutline->ColorSet(
					pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor(),
					false
					);
				pMainFrame->m_pBtnColorOutline->ColorSet(
					COLORREF(-1),
					true
					);
				pMainFrame->m_pBtnColorOutline->RedrawButton();
			}

			CExtBarButton * pTBB =
				pMainFrame->m_wndToolBarStandard.GetButton(
					pMainFrame->m_wndToolBarStandard.CommandToIndex(
						ID_DOC_LINE_WIDTH
						)
					);
			ASSERT_VALID( pTBB );
			UINT nCmdID = pTBB->GetCmdID( true );
			ASSERT( ID_LINE_WIDTH_0 <= nCmdID && nCmdID <= ID_LINE_WIDTH_OTHER );

			UINT nAdjustCmdID = ID_LINE_WIDTH_0;
			if( !pObj->IsNoOutline() )
			{
				UINT xw = (UINT)pObj->m_logpen.lopnWidth.x;
				if( xw <= 10 )
				{
					ASSERT( xw > 0 );
					nAdjustCmdID = ID_LINE_WIDTH_0 + xw;
					ASSERT( ID_LINE_WIDTH_1 <= nAdjustCmdID && nAdjustCmdID <= ID_LINE_WIDTH_10 );
				}
				else
					nAdjustCmdID = ID_LINE_WIDTH_OTHER;
			}

			if( nCmdID != nAdjustCmdID )
				pTBB->SetCmdID( nAdjustCmdID, true, true );
		}
	#else // (defined __EXT_MFC_NO_CUSTOMIZE)

		pMainFrame->RedrawCommandItems( ID_OBJECT_FILLCOLOR ); 
		pMainFrame->RedrawCommandItems( ID_OBJECT_LINECOLOR ); 
		pMainFrame->RedrawCommandItems( ID_LINE_WIDTH_OTHER ); 

	#endif // not - (defined __EXT_MFC_NO_CUSTOMIZE)
	}



#ifndef FOR_CATTLEBASE
bool CDrwDocView::LoadDocImage(LPCTSTR szFileName)
{
	return m_BackDoc.Load(szFileName);
}

void CDrwDocView::OnAlignTops()
{
	POSITION pos = m_selection.GetHeadPosition();
    ASSERT( pos != NULL );
	CDocObj * pObjBase = m_selection.GetNext( pos );
	LONG *basetop = &pObjBase->m_position.top,*basebottom = &pObjBase->m_position.bottom
		,*top,*bottom,*temp,height;

	if(*basebottom > *basetop)
	{
		temp = basetop;
		basetop = basebottom;
		basebottom = temp;
	}

		while( pos != NULL )
		{
			CDocObj * pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
			
			top = &pObj->m_position.top;
			bottom = &pObj->m_position.bottom;

			if(*top < *bottom)
			{
				temp = top;
				top = bottom;
				bottom = temp;
			}

			height = *top-*bottom;

            *top = *basetop;
			*bottom = *top - height;

		} // while( pos != NULL )

		Invalidate();

}

void CDrwDocView::OnAlignLefts()
{
	POSITION pos = m_selection.GetHeadPosition();
    ASSERT( pos != NULL );
	CDocObj * pObjBase = m_selection.GetNext( pos );
	LONG *baseleft = &pObjBase->m_position.left,*baseright = &pObjBase->m_position.right
		,*left,*right,*temp,width;

	if(*baseleft > *baseright)
	{
		temp = baseleft;
		baseleft = baseright;
		baseright = temp;
	}

		while( pos != NULL )
		{
			CDocObj * pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
			
			left = &pObj->m_position.left;
			right = &pObj->m_position.right;

			if(*left > *right)
			{
				temp = left;
				left = right;
				right = temp;
			}

			width = *right-*left;

            *left = *baseleft;
			*right = *left + width;

		} // while( pos != NULL )

		Invalidate();
}

void CDrwDocView::OnGroupPos()
{

CGroupPos dlg;
if(dlg.DoModal() == IDOK)
{
	CSize XY = dlg.GetPos();
	POSITION pos = m_selection.GetHeadPosition();
    ASSERT( pos != NULL );
	
		while( pos != NULL )
		{
			CDocObj * pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
			
			pObj->m_position.left += XY.cx;
			pObj->m_position.right += XY.cx;

			pObj->m_position.top -= XY.cy;
			pObj->m_position.bottom -= XY.cy;

			
		} // while( pos != NULL )

		Invalidate();

}


}

void CDrwDocView::OnShowGroupMgr()
{
	CGroupMgrDlg dlg(this);
	dlg.DoModal();
	OnUpdate(NULL, HINT_UPDATE_WINDOW, NULL);


}

void CDrwDocView::OnAddToGroup()
{
	CDocObj * pObj = m_selection.GetHead();
	CDrawObjGroup *pGroup;
	POSITION pos;

	CObjGroup objdlg(this,pObj->m_group);
	if(objdlg.DoModal() == IDOK)
	{
		pGroup = objdlg.GetSelectedGroup();
		pos = m_selection.GetHeadPosition();
		
		while(pos)	
		{
			pObj = m_selection.GetNext(pos);
			pObj->m_group = pGroup;			

		}
		
		
	}

}

void CDrwDocView::OnSelGroup()
{
	
	CDrawObjGroup *pObjGrp = m_selection.GetHead()->m_group;
	CDocObj *pObj;
	if(pObjGrp)
	{

		CDrawDoc* pDoc = GetDocument();
		POSITION pos = pDoc->GetObjects()->GetHeadPosition();
		while (pos != NULL)
		{
			pObj = pDoc->GetObjects()->GetNext(pos);
			if(pObj->m_group == pObjGrp)
				if (!IsSelected(pObj))
			  	  Select(pObj, true);
		
		
		}
				
			
	}

}
void CDrwDocView::OnShowStyleMgr()
{
CStyleMgr mgrDlg(this);
mgrDlg.DoModal();
Invalidate(FALSE);
//OnUpdate(NULL, HINT_UPDATE_TEXT_OBJECTS, NULL);
}


BOOL CDrwDocView::OnMakeSameSize(UINT nId)
{
	POSITION pos = m_selection.GetHeadPosition();
    ASSERT( pos != NULL );
	CDocObj * pObjBase = m_selection.GetNext( pos );

	LONG *baseleft = &pObjBase->m_position.left,*baseright = &pObjBase->m_position.right
		,*left,*right,*temp,width;

	LONG *basetop = &pObjBase->m_position.top,*basebottom = &pObjBase->m_position.bottom
		,*top,*bottom,height;

	if(*basebottom > *basetop)
	{
		temp = basetop;
		basetop = basebottom;
		basebottom = temp;
	}		

	if(*baseleft > *baseright)
	{
		temp = baseleft;
		baseleft = baseright;
		baseright = temp;
	}

	height = *basebottom -*basetop;
	width = *baseright-*baseleft;
    while( pos != NULL )
	{
		CDocObj * pObj = m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
			
		left = &pObj->m_position.left;
		right = &pObj->m_position.right;
		top = &pObj->m_position.top;
		bottom = &pObj->m_position.bottom;


		if(*left > *right)
		{
			temp = left;
			left = right;
			right = temp;
		}
			

		if(*top < *bottom)
		{
			temp = top;
			top = bottom;
			bottom = temp;
		}

			switch(nId)
			{
				case ID_MAKESAMESIZE_BOTH:
					*bottom = *top+height;
					//no break;
				case ID_MAKESAMESIZE_WIDTH:
					*right = *left+width;
				break;
				case ID_MAKESAMESIZE_HEIGHT:
					*bottom = *top+height;
				break;
			}

	}

		Invalidate();
		return TRUE;
}


void CDrwDocView::NormalizePosition(CRect &rect)
{

	LONG temp;
	if(rect.bottom > rect.top)
	{
		temp = rect.top;
		rect.top = rect.bottom;
		rect.bottom = temp;
	}		

	if(rect.left > rect.right)
	{
		temp = rect.left;
		rect.left = rect.right;
		rect.right = temp;
	}
}



void CDrwDocView::OnTopBottom()
{
	CRect newPos;
	CDocObj *pObj1 = m_selection.GetAt(m_selection.FindIndex(0));
	CDocObj *pObj2 = m_selection.GetAt(m_selection.FindIndex(1));
	NormalizePosition(pObj1->m_position);
	NormalizePosition(pObj2->m_position);

	newPos.SetRect(pObj2->m_position.left,pObj1->m_position.bottom,pObj2->m_position.right,pObj1->m_position.bottom-abs(pObj2->m_position.Height()));
    pObj2->MoveTo(newPos);

}
void CDrwDocView::OnLeftRight()
{
	CRect newPos;
	CDocObj *pObj1 = m_selection.GetAt(m_selection.FindIndex(0));
	CDocObj *pObj2 = m_selection.GetAt(m_selection.FindIndex(1));
	NormalizePosition(pObj1->m_position);
	NormalizePosition(pObj2->m_position);

	newPos.SetRect(pObj1->m_position.right,pObj2->m_position.top,pObj1->m_position.right+pObj2->m_position.Width(),pObj2->m_position.bottom);
    pObj2->MoveTo(newPos);


}

void CDrwDocView::OnUpdateNeighbours(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_selection.GetCount() == 2);
}


#endif
