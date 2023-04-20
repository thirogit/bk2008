// drawvw.cpp : implementation of the CDrawView class
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


#include "stdafx.h"
#include <afxpriv.h>

#ifndef FOR_CATTLEBASE
#include "drawcli.h"
#include "drawtool.h"
#include "resource.h"     
#include "GroupMgrDlg.h"
#include "GroupPos.h"
#include "ObjGroup.h"
#include "StyleMgr.h"
#include "Mainfrm.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>


#else
#include "printdocs/MyPrintPreview.h"
#include "../../src/global_arrays.h"
#endif

#include "cntritem.h"
#include "drawvw.h"
#include "drawobj.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifndef FOR_CATTLEBASE
// private clipboard format (list of Draw objects)
CLIPFORMAT CDrawView::m_cfDraw = (CLIPFORMAT)
	::RegisterClipboardFormat(_T("MFC Draw Sample"));
CLIPFORMAT CDrawView::m_cfObjectDescriptor = NULL;
#endif

#define TOP_OFFSET 100 // 10mm

/////////////////////////////////////////////////////////////////////////////
// CDrawView

IMPLEMENT_DYNCREATE(CDrawView, CScrollView)

BEGIN_MESSAGE_MAP(CDrawView, CScrollView)

#ifndef FOR_CATTLEBASE
	//{{AFX_MSG_MAP(CDrawView)
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
	ON_COMMAND(ID_SAVEPAGETOXML, OnSavePageToXml)	

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
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawView construction/destruction

CDrawView::CDrawView()
{

#ifndef FOR_CATTLEBASE
	
	m_bGrid = TRUE;
	m_bBackDoc = FALSE;
	m_gridColor = RGB(0, 0, 128);
	m_bActive = FALSE;

	if( m_cfObjectDescriptor == NULL )
		m_cfObjectDescriptor = (CLIPFORMAT)::RegisterClipboardFormat(_T("Object Descriptor") );
	m_prevDropEffect = DROPEFFECT_NONE;
	
	nZoom = 100;
#endif


#ifdef FOR_CATTLEBASE
	m_pOldFrame = NULL;
	m_pCtrlDlg = NULL;
#endif

}

CDrawView::~CDrawView()
{

}


BOOL CDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	ASSERT(cs.style & WS_CHILD);
	if (cs.lpszClass == NULL)
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);

	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE|WS_EX_STATICEDGE);
	return TRUE;
}


#ifndef FOR_CATTLEBASE
	void CDrawView::OnActivateView(BOOL bActivate, CView* pActiveView,
		CView* pDeactiveView)
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
/////////////////////////////////////////////////////////////////////////////
// CDrawView drawing
	void CDrawView::InvalObj(CDrawObj* pObj)
	{
		CRect rect = pObj->m_position;
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


	void CDrawView::OnUpdate(CView* , LPARAM lHint, CObject* pHint)
	{
		switch (lHint)
		{
		/*case HINT_UPDATE_TEXT_OBJECTS:
		{
				CDrawDoc* pDoc = GetDocument();
				POSITION pos = pDoc->GetObjects()->GetHeadPosition();
				while (pos != NULL)
				{
					CDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
					if (pObj->IsKindOf(RUNTIME_CLASS(CDrawText)))
					{
						CDrawText *pText = (CDrawText*)pObj;
						if(pText->m_font)
						{
							DeleteObject(pText->m_font);
							pText->m_font = NULL;
						}
					}
				}
				Invalidate(0);
				break;
		}*/
		case HINT_UPDATE_WINDOW:    // redraw entire window
			Invalidate(FALSE);
			break;

		case HINT_UPDATE_DRAWOBJ:   // a single object has changed
			InvalObj((CDrawObj*)pHint);
			break;

		case HINT_UPDATE_SELECTION: // an entire selection has changed
			{
				CDrawObjList* pList = pHint != NULL ?
					(CDrawObjList*)pHint : &m_selection;
				POSITION pos = pList->GetHeadPosition();
				while (pos != NULL)
					InvalObj(pList->GetNext(pos));
			}
			break;

		case HINT_DELETE_SELECTION: // an entire selection has been removed
			if (pHint != &m_selection)
			{
				CDrawObjList* pList = (CDrawObjList*)pHint;
				POSITION pos = pList->GetHeadPosition();
				while (pos != NULL)
				{
					CDrawObj* pObj = pList->GetNext(pos);
					InvalObj(pObj);
					Remove(pObj);   // remove it from this view's selection
				}
			}
			break;

		case HINT_UPDATE_OLE_ITEMS:
			{
				CDrawDoc* pDoc = GetDocument();
				POSITION pos = pDoc->GetObjects()->GetHeadPosition();
				while (pos != NULL)
				{
					CDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
					if (pObj->IsKindOf(RUNTIME_CLASS(CDrawOleObj)))
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


	BOOL CDrawView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
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
#endif


void CDrawView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);

	SetUpDC(pDC);



	if(pInfo != NULL)
	{
			pInfo->m_rectDraw.SetRect(0, 0,	pDC->GetDeviceCaps(HORZRES),pDC->GetDeviceCaps(VERTRES));
			pDC->DPtoLP(&pInfo->m_rectDraw);


		/*TRACE("left = %d, top = %d, right = %d,bottom = %d\n",	pInfo->m_rectDraw.left,
			pInfo->m_rectDraw.top,	pInfo->m_rectDraw.right,	pInfo->m_rectDraw.bottom);*/

		CDrawDoc* pDoc = GetDocument();
		CSize l_PrintMarg = pDoc->GetPaperSize()-pDoc->GetPageSize();
		CSize l_phyMarg = pDoc->GetPhyMarginsSize();
		CSize l_margOff = pDoc->GetMarginOffset();
				
		/*pDC->SetWindowOrg(
			
			-(pInfo->m_rectDraw.Width()+l_PrintMarg.cx-2*l_phyMarg.cx)/2,
			(-pInfo->m_rectDraw.Height() + l_PrintMarg.cy +2*l_phyMarg.cy)/2);
*/

		pDC->SetWindowOrg(
			
			-(pInfo->m_rectDraw.Width()+l_PrintMarg.cx)/2 + l_phyMarg.cx,
			(-pInfo->m_rectDraw.Height() + l_PrintMarg.cy )/2  + l_phyMarg.cy);


#ifdef FOR_CATTLEBASE 
		pDC->OffsetWindowOrg(-l_margOff.cx,l_margOff.cy);
#endif

	}
	else
	{



#ifndef FOR_CATTLEBASE
			SetUpDCZoom(pDC,nZoom);
#endif	

	
	CSize size = GetDocument()->GetPaperSize();

	// ptOrg is in logical coordinates

	pDC->SetWindowOrg(-size.cx/2,size.cy/2 + TOP_OFFSET);

		

	}



}

#ifndef FOR_CATTLEBASE

void CDrawView::SetZoom(int Zoom)
{
	nZoom = Zoom;
	Invalidate();

SetPageSize(GetDocument()->GetPaperSize());

}

void CDrawView::SetUpDCZoom(CDC *pDC,int Zoom)
{
	CSize vport = pDC->GetViewportExt();
	pDC->SetViewportExt(MulDiv(vport.cx,Zoom,100),
		                MulDiv(vport.cy,Zoom,100));

}
#endif

void CDrawView::SetUpDC(CDC *pDC)
{

	// mapping mode is MM_ANISOTROPIC
	// these extents setup a mode similar to MM_LOENGLISH
	// MM_LOENGLISH is in .01 physical inches
	// these extents provide .01 logical inches

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(254, -254);
}


void CDrawView::OnDraw(CDC* pDC)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = NULL;

	

	// only paint the rect that needs repainting
	CRect client;

	

	
	pDC->GetClipBox(client);
	CRect rect = client;
	DocToClient(rect);


	if (!pDC->IsPrinting())
	{
		// draw to offscreen bitmap for fast looking repaints
		if (dc.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;

				// offset origin more because bitmap is just piece of the whole drawing
				dc.OffsetViewportOrg(-rect.left, -rect.top);
				pOldBitmap = dc.SelectObject(&bitmap);
				dc.SetBrushOrg(rect.left % 8, rect.top % 8);

				// might as well clip to the same rectangle
				dc.IntersectClipRect(client);
			}
		}
	}

	// paint background
	CBrush brush;
	if (!brush.CreateSolidBrush(pDoc->GetPaperColor()))
		return;

	brush.UnrealizeObject();
	pDrawDC->FillRect(client, &brush);

	//pDrawDC->MoveTo(client.left,client.top);
	//pDrawDC->LineTo(client.right,client.bottom);



#ifndef FOR_CATTLEBASE
	if (!pDC->IsPrinting()) DrawHelp(pDrawDC);
#endif

	pDoc->Draw(pDrawDC, this);

	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBitmap);
	}
}


#ifndef FOR_CATTLEBASE

	void CDrawView::Remove(CDrawObj* pObj)
	{
		POSITION pos = m_selection.Find(pObj);
		if (pos != NULL)
			m_selection.RemoveAt(pos);
	}

	void CDrawView::PasteNative(COleDataObject& dataObject)
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

	void CDrawView::PasteEmbedded(COleDataObject& dataObject, CPoint point )
	{
		BeginWaitCursor();

		// paste embedded
		CDrawOleObj* pObj = new CDrawOleObj(GetInitialPosition());
		ASSERT_VALID(pObj);
		CDrawItem* pItem = new CDrawItem(GetDocument(), pObj);
		ASSERT_VALID(pItem);
		pObj->m_pClientItem = pItem;

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
			pObj->MoveTo( CRect( point, pObj->m_extent ), this );

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


	void CDrawView::DrawHelp(CDC* pDC)
	{
		CDrawDoc* pDoc = GetDocument();

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

		/*CDC *hdcMem = new CDC;
		hdcMem->CreateCompatibleDC(pDC);
		SetUpDC(hdcMem);
		SetUpDCZoom(hdcMem,nZoom);

		CRect bmprect(0,0,m_BackBmpWidth,m_BackBmpHeight);
		//hdcMem->SetWindowOrg(0,0);
		
		CPoint org(bmprect.right/2,bmprect.bottom/2);
		hdcMem->DPtoLP(&org);

		org.x *= -1;
		org.y *= -1;
		hdcMem->SetWindowOrg(org);
		
		hdcMem->SelectObject(m_BackDoc);
		
		hdcMem->DPtoLP(&bmprect);

		int BmpWidth = bmprect.Width();
		int BmpHeight= bmprect.Height();
			

	//	TRACE("bmp left = %d, top = %d\n",bmprect.left,bmprect.top);
		pDC->SetStretchBltMode(COLORONCOLOR);
		///pDC->BitBlt(rect.left,rect.top,BmpWidth,BmpHeight,hdcMem,bmprect.left,bmprect.top,SRCCOPY);
		pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),
						hdcMem,bmprect.left,bmprect.top,BmpWidth,BmpHeight,SRCCOPY);
		delete hdcMem;*/
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
		
		// Outlines
		//CPen penSolid;
		//penSolid.CreatePen(PS_SOLID, 1, m_gridColor);
	//	pDC->SelectObject(&penSolid);
	//	pDC->MoveTo(rect.left, rect.top);
	//	pDC->LineTo(rect.right, rect.top);
	//	pDC->LineTo(rect.right, rect.bottom);
	//	pDC->LineTo(rect.left, rect.bottom);
	//	pDC->LineTo(rect.left, rect.top);
		pDC->SelectObject(pOldPen);
	}
		//pDC->SelectObject(oldBrush);
		
		pDC->SetBkColor(oldBkColor);
	}

#endif

void CDrawView::OnInitialUpdate()
{
	CSize size = GetDocument()->GetPaperSize();
	//CClientDC dc(NULL);
	//size.cx = MulDiv(size.cx, dc.GetDeviceCaps(LOGPIXELSX), 100);
	//size.cy = MulDiv(size.cy, dc.GetDeviceCaps(LOGPIXELSY), 100);
//	OnPrepareDC(&dc,NULL);
/*
	size.cy += 2* TOP_OFFSET;
	size.cy = MulDiv(size.cy, dc.GetDeviceCaps(LOGPIXELSY), 254);
	size.cx = MulDiv(size.cx, dc.GetDeviceCaps(LOGPIXELSX), 254);
	SetScrollSizes(MM_TEXT, size);*/
	SetPageSize(size);
}

void CDrawView::SetPageSize(CSize size)
{
	CClientDC dc(NULL);
	SetUpDC(&dc);

#ifndef FOR_CATTLEBASE
	SetUpDCZoom(&dc,nZoom);
#endif
	CSize vport = dc.GetViewportExt();
	size.cy += (2* TOP_OFFSET);

#if  !defined(FOR_CATTLEBASE)
	int scale = MulDiv(254,100,nZoom);
#else
	int scale = 254;
#endif

	size.cy = MulDiv(size.cy, vport.cy, scale);
	size.cx = MulDiv(size.cx, vport.cx, scale);

	
	SetScrollSizes(MM_TEXT, size);
//	GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_WINDOW, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CDrawView printing

BOOL CDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation

#ifdef FOR_CATTLEBASE

		pInfo->SetMinPage(1);
		pInfo->SetMaxPage(GetDocument()->GetPrintPages());
	
#endif
	
	//if(pInfo->m_bPreview) return TRUE;


	return DoPreparePrinting(pInfo);
}

void CDrawView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CDrawDoc *pDoc = GetDocument();
	CScrollView::OnBeginPrinting(pDC,pInfo);

	// check page size -- user could have gone into print setup
	// from print dialog and changed paper or orientation
	pDoc->ComputePageSize();

#ifdef FOR_CATTLEBASE
	nCurPrintPage = -1;
#endif
}

void CDrawView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{


}

/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

#ifndef FOR_CATTLEBASE

	BOOL CDrawView::IsSelected(const CObject* pDocItem) const
	{
		CDrawObj* pDrawObj = (CDrawObj*)pDocItem;
		if (pDocItem->IsKindOf(RUNTIME_CLASS(CDrawItem)))
			pDrawObj = ((CDrawItem*)pDocItem)->m_pDrawObj;
		return m_selection.Find(pDrawObj) != NULL;
	}

	void CDrawView::OnInsertObject()
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
	void CDrawView::OnCancelEdit()
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

	void CDrawView::OnSetFocus(CWnd* pOldWnd)
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

	CRect CDrawView::GetInitialPosition()
	{
		CRect rect(10, 10, 10, 10);
		ClientToDoc(rect);
		return rect;
	}
#endif

void CDrawView::ClientToDoc(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);
}

void CDrawView::ClientToDoc(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rect);
	ASSERT(rect.left <= rect.right);
	ASSERT(rect.bottom <= rect.top);
}

void CDrawView::DocToClient(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
}

void CDrawView::DocToClient(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}

#ifndef FOR_CATTLEBASE
	void CDrawView::Select(CDrawObj* pObj, BOOL bAdd)
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
	void CDrawView::SelectWithinRect(CRect rect, BOOL bAdd)
	{
		if (!bAdd)
			Select(NULL);

		ClientToDoc(rect);

		CDrawObjList* pObList = GetDocument()->GetObjects();
		POSITION posObj = pObList->GetHeadPosition();
		while (posObj != NULL)
		{
			CDrawObj* pObj = pObList->GetNext(posObj);
			if (pObj->Intersects(rect))
				if(pObj->m_group)
				{
					if(pObj->m_group->m_PrintVis)
						Select(pObj, TRUE);
				}
				else Select(pObj, TRUE);
		}
	}

	void CDrawView::Deselect(CDrawObj* pObj)
	{
		POSITION pos = m_selection.Find(pObj);
		if (pos != NULL)
		{
			InvalObj(pObj);
			m_selection.RemoveAt(pos);
		}
	}

	void CDrawView::CloneSelection()
	{
		POSITION pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			CDrawObj* pObj = m_selection.GetNext(pos);
			pObj->Clone(pObj->m_pDocument);
				// copies object and adds it to the document
		}
	}

	void CDrawView::UpdateActiveItem()
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
	// CDrawView message handlers

	void CDrawView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (!m_bActive)
			return;
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnLButtonDown(this, nFlags, point);
		SyncToolbarObjButtons();
	}

	void CDrawView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		if (!m_bActive)
			return;
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnLButtonUp(this, nFlags, point);
		SyncToolbarObjButtons();
	}

	void CDrawView::OnMouseMove(UINT nFlags, CPoint point)
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

	void CDrawView::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		if (!m_bActive)
			return;
		CDrawTool* pTool = CDrawTool::FindTool(CDrawTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnLButtonDblClk(this, nFlags, point);
	}

	void CDrawView::OnDestroy()
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


	void CDrawView::OnNewPage()
	{


		GetDocument()->NewPage();
			Invalidate();

		
	}


	void CDrawView::OnNextPage()
	{
		GetDocument()->NextPage();
			Invalidate();
		
	}


	void CDrawView::OnSavePageToXml()
	{

		CFileDialog saveDlg(FALSE,NULL,NULL,OFN_OVERWRITEPROMPT, "XML|*.xml||");
	
		if(saveDlg.DoModal() == IDOK)
		{

			CString xmlFilePath = saveDlg.GetPathName();
	

			try 
			{
				xercesc::XMLPlatformUtils::Initialize();
			}
			catch (const xercesc::XMLException& toCatch) 
			{
				// Do your failure processing here
				return;
			}


			xercesc::DOMImplementation    *pImplement = NULL;
			xercesc::DOMLSOutput         *pOutput    = NULL;
			xercesc::XMLFormatTarget        *pTarget    = NULL;

			/*
			Return the first registered implementation that has
			the desired features. In this case, we are after
			a DOM implementation that has the LS feature... or Load/Save.
			*/
			pImplement = xercesc::DOMImplementationRegistry::getDOMImplementation(L"LS");

			

			/*
			From the DOMImplementation, create a DOMWriter.
			DOMWriters are used to serialize a DOM tree [back] into an XML document.
			*/
			pOutput = ((xercesc::DOMImplementationLS*)pImplement)->createLSOutput();
			
			
			pTarget = new xercesc::LocalFileFormatTarget((LPCSTR)xmlFilePath);
			
		
			pOutput->setByteStream(pTarget);
			
			//get a LSSerializer object
			xercesc::DOMLSSerializer* pSerializer = pImplement->createLSSerializer();

			 if (pSerializer->getDomConfig()->canSetParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true))
				  pSerializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);



			xercesc::DOMDocument * pDOMDocument = NULL;
			pDOMDocument = pImplement->createDocument(0, L"document", 0);

			xercesc::DOMElement * pRootElement = NULL;
			pRootElement = pDOMDocument->getDocumentElement();

			GetDocument()->SerializeXml(pRootElement);
									
			bool bSuccess = pSerializer->write(pDOMDocument,pOutput);

			pTarget->flush();
			
			//pOutput->release();

			//pDOMDocument->release();

			//pSerializer->release();

			xercesc::XMLPlatformUtils::Terminate();
		}
	}

	void CDrawView::OnOpenBackImg()
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

	void CDrawView::OnPrevPage()
	{
		GetDocument()->PrevPage();
			Invalidate();
	}


	void CDrawView::OnDelPage()
	{

		GetDocument()->DelPage();
			Invalidate();
		
	}


	void CDrawView::OnDrawSelect()
	{
		CDrawTool::c_drawShape = selection;
	}

	void CDrawView::OnDrawRoundRect()
	{
		CDrawTool::c_drawShape = roundRect;
	}

	void CDrawView::OnDrawRect()
	{
		CDrawTool::c_drawShape = rect;
	}

	void CDrawView::OnDrawLine()
	{
		CDrawTool::c_drawShape = line;
	}

	void CDrawView::OnDrawEllipse()
	{
		CDrawTool::c_drawShape = ellipse;
	}

	void CDrawView::OnDrawPolygon()
	{
		CDrawTool::c_drawShape = poly;
	}

	void CDrawView::OnUpdateDrawEllipse(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == ellipse);
	}

	void CDrawView::OnUpdateDrawLine(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == line);
	}

	void CDrawView::OnUpdateDrawRect(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == rect);
	}

	void CDrawView::OnUpdateDrawRoundRect(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == roundRect);
	}

	void CDrawView::OnUpdateDrawSelect(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == selection);
	}

	void CDrawView::OnUpdateSingleSelect(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(m_selection.GetCount() == 1);
	}


	void CDrawView::OnDrawText()
	{
		CDrawTool::c_drawShape = text;
	}

	void CDrawView::OnDrawImage()
	{
		CDrawTool::c_drawShape = image;
	}

	void CDrawView::OnUpdateDrawText(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == text);
	}

	void CDrawView::OnUpdateDrawImage(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == image);
	}

	void CDrawView::OnEditSelectAll()
	{
		CDrawObjList* pObList = GetDocument()->GetObjects();
		POSITION pos = pObList->GetHeadPosition();
		while (pos != NULL)
			Select(pObList->GetNext(pos), TRUE);
	}

	void CDrawView::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(GetDocument()->GetObjects()->GetCount() != 0);
	}

	void CDrawView::OnEditClear()
	{
		// update all the views before the selection goes away
		GetDocument()->UpdateAllViews(NULL, HINT_DELETE_SELECTION, &m_selection);
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);

		// now remove the selection from the document
		POSITION pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			CDrawObj* pObj = m_selection.GetNext(pos);
			GetDocument()->Remove(pObj);
			pObj->Remove();
		}
		m_selection.RemoveAll();
	}

	void CDrawView::OnUpdateAnySelect(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(!m_selection.IsEmpty());
	}

	void CDrawView::OnUpdateDrawPolygon(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(CDrawTool::c_drawShape == poly);
	}

	void CDrawView::OnSize(UINT nType, int cx, int cy)
	{
		CScrollView::OnSize(nType, cx, cy);
		UpdateActiveItem();
	}

	void CDrawView::OnViewGrid()
	{
		m_bGrid = !m_bGrid;
		Invalidate(FALSE);
	}

	void CDrawView::OnUpdateViewGrid(CCmdUI* pCmdUI)
	{
		pCmdUI->SetCheck(m_bGrid);

	}

	void CDrawView::OnViewBackDoc()
	{
		m_bBackDoc = !m_bBackDoc;
		Invalidate(FALSE);
	}

	void CDrawView::OnUpdateViewBackDoc(CCmdUI* pCmdUI)
	{
		pCmdUI->SetCheck(m_bBackDoc);

	}

	BOOL CDrawView::OnEraseBkgnd(CDC*)
	{
		return TRUE;
	}


	void CDrawView::OnObjectFillColor()
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
	CDrawObj * pObj = m_selection.GetNext( pos );
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
			CDrawObj* pObj = m_selection.GetNext(pos);
			pObj->SetNoFill( FALSE );
			pObj->SetFillColor(color);
		}

	#endif

		SyncToolbarObjButtons();
	}

	void CDrawView::OnObjectLineColor()
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
	CDrawObj * pObj = m_selection.GetNext( pos );
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
			CDrawObj* pObj = m_selection.GetNext(pos);
			pObj->SetNoOutline( FALSE );
			pObj->SetLineColor(color);
		}

	#endif

		SyncToolbarObjButtons();
	}

	void CDrawView::OnObjectMoveBack()
	{
		CDrawDoc* pDoc = GetDocument();
		CDrawObj* pObj = m_selection.GetHead();
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

	void CDrawView::OnObjectMoveForward()
	{
		CDrawDoc* pDoc = GetDocument();
		CDrawObj* pObj = m_selection.GetHead();
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

	void CDrawView::OnObjectMoveToBack()
	{
		CDrawDoc* pDoc = GetDocument();
		CDrawObj* pObj = m_selection.GetHead();
		CDrawObjList* pObjects = pDoc->GetObjects();
		POSITION pos = pObjects->Find(pObj);
		ASSERT(pos != NULL);
		pObjects->RemoveAt(pos);
		pObjects->AddHead(pObj);
		InvalObj(pObj);
	}

	void CDrawView::OnObjectMoveToFront()
	{
		CDrawDoc* pDoc = GetDocument();
		CDrawObj* pObj = m_selection.GetHead();
		CDrawObjList* pObjects = pDoc->GetObjects();
		POSITION pos = pObjects->Find(pObj);
		ASSERT(pos != NULL);
		pObjects->RemoveAt(pos);
		pObjects->AddTail(pObj);
		InvalObj(pObj);
	}

	void CDrawView::OnEditCopy()
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
			CDrawObj* pDrawObj = m_selection.GetHead();
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

	void CDrawView::OnUpdateEditCopy(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(!m_selection.IsEmpty());
	}

	void CDrawView::OnEditCut()
	{
		OnEditCopy();
		OnEditClear();
	}

	void CDrawView::OnUpdateEditCut(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(!m_selection.IsEmpty());
	}

	void CDrawView::OnEditPaste()
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

	void CDrawView::OnUpdateEditPaste(CCmdUI* pCmdUI)
	{
		// determine if private or standard OLE formats are on the clipboard
		COleDataObject dataObject;
		BOOL bEnable = dataObject.AttachClipboard() &&
			(dataObject.IsDataAvailable(m_cfDraw) ||
			 COleClientItem::CanCreateFromData(&dataObject));

		// enable command based on availability
		pCmdUI->Enable(bEnable);
	}

	void CDrawView::OnViewShowObjects()
	{
		CDrawOleObj::c_bShowItems = !CDrawOleObj::c_bShowItems;
		GetDocument()->UpdateAllViews(NULL, HINT_UPDATE_OLE_ITEMS, NULL);
	}

	void CDrawView::OnUpdateViewShowObjects(CCmdUI* pCmdUI)
	{
		pCmdUI->SetCheck(CDrawOleObj::c_bShowItems);
	}

	void CDrawView::OnEditProperties()
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

	void CDrawView::OnUpdateGroupProp(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(m_selection.GetCount() > 1 && CDrawTool::c_drawShape == selection);
		

	}

	void CDrawView::OnUpdateEditProperties(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(m_selection.GetCount() == 1 &&
					   CDrawTool::c_drawShape == selection);
	}

#endif
	
void CDrawView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	BOOL lpreview = FALSE;
	
	CDrawDoc *pDoc = GetDocument();
	pDoc;//unused variable warning

	
#ifdef FOR_CATTLEBASE
	pDoc->SetPrintDC(pDC);

	if(pInfo->m_nCurPage != nCurPrintPage)
	{
		pDoc->PreparePage(pInfo->m_nCurPage);
		nCurPrintPage = pInfo->m_nCurPage;
	}
#endif

	if(pInfo) lpreview = pInfo->m_bPreview;

#ifdef FOR_CATTLEBASE
	if(!pDoc->InForm() || lpreview)
		OnDraw(pDC);
	 pDoc->PrintPage(pInfo->m_nCurPage);
	pDoc->SetPrintDC(NULL);
#else
	OnDraw(pDC);
#endif
}

void CDrawView::OnFilePrint()
{	
	CScrollView::OnFilePrint();	
	//GetDocument()->ComputePageSize();
}

#ifdef FOR_CATTLEBASE

void CDrawView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
	// Show the original frame
	m_pOldFrame->ShowWindow(SW_SHOW);
	// Restore main frame pointer
	AfxGetApp()->m_pMainWnd = m_pOldFrame;
    
	m_pCtrlDlg->EnableWindow(TRUE);
}

#endif


/////////////////////////////////////////////////////////////////////////////
// CDrawView diagnostics

#ifdef _DEBUG
void CDrawView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDrawView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// new
// support for drag/drop

#ifndef FOR_CATTLEBASE
	int CDrawView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CScrollView::OnCreate(lpCreateStruct) == -1)
			return -1;

		
		// register drop target
		if( m_dropTarget.Register( this ) )
			return 0;
		else
			return -1;
		
	}
	BOOL CDrawView::GetObjectInfo(COleDataObject* pDataObject,CSize* pSize, CSize* pOffset)
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

	DROPEFFECT CDrawView::OnDragEnter(COleDataObject* pDataObject,
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

	DROPEFFECT CDrawView::OnDragOver(COleDataObject*,
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

	BOOL CDrawView::OnDrop(COleDataObject* pDataObject,
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

	void CDrawView::OnDragLeave()
	{
		CClientDC dc(this);
		if (m_prevDropEffect != DROPEFFECT_NONE)
		{
			dc.DrawFocusRect(CRect(m_dragPoint,m_dragSize)); // erase previous focus rect
			m_prevDropEffect = DROPEFFECT_NONE;
		}
	}


	void CDrawView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
	{
	CDrawDoc * pDoc = GetDocument();
		ASSERT_VALID( pDoc );

		// make sure window is active
		GetParentFrame()->ActivateFrame();

	CPoint ptLocal = point;
		ScreenToClient( &ptLocal );
		ClientToDoc( ptLocal );

	bool bTryProcessItemFocus = !( (point.x < 0) || (point.y < 0) );
	CDrawObj * pObj = NULL;
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



	LRESULT CDrawView::OnExtMenuPrepare(WPARAM wParam, LPARAM lParam)
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

	CDrawObj * pObj = NULL;
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

	LRESULT CDrawView::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
	{
	POSITION pos = m_selection.GetHeadPosition();
		ASSERT( pos != NULL );
		while( pos != NULL )
		{
			CDrawObj * pObj = m_selection.GetNext( pos );
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

	LRESULT CDrawView::OnColorSelectCustom(WPARAM wParam, LPARAM lParam)
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

	void CDrawView::OnUpdateSelectionSingle(CCmdUI* pCmdUI)
	{
	int nSelCount = m_selection.GetCount();
		pCmdUI->Enable( nSelCount == 1 );
	}

	void CDrawView::OnUpdateSelectionNonEmpty(CCmdUI* pCmdUI)
	{
	int nSelCount = m_selection.GetCount();
		pCmdUI->Enable( nSelCount > 0 );
	}

	void CDrawView::OnLineWidth( UINT nCmdID )
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
			CDrawObj * pObj = m_selection.GetNext( pos );
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

	void CDrawView::OnUpdateLineWidth( CCmdUI * pCmdUI )
	{
		ASSERT( pCmdUI != NULL );
	UINT nCmdID = pCmdUI->m_nID;
		ASSERT( ID_LINE_WIDTH_0 <= nCmdID && nCmdID <= ID_LINE_WIDTH_OTHER );
	int nSelCount = m_selection.GetCount();

	CDrawObj * pObj = NULL;
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

	LRESULT CDrawView::OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam)
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

	void CDrawView::OnUpdateDocLineWidth(CCmdUI* pCmdUI) 
	{
		pCmdUI->Enable( m_selection.GetCount() > 0 );
	}


	void CDrawView::SyncToolbarObjButtons()
	{
	CFrameWnd * pFrame = GetParentFrame();
		ASSERT_VALID( pFrame );
		ASSERT_KINDOF( CMDIChildWnd, pFrame );
	CMainFrame * pMainFrame = (CMainFrame *)pFrame->GetParentFrame();
		ASSERT_VALID( pMainFrame );
		ASSERT_KINDOF( CMainFrame, pMainFrame );

	#if (defined __EXT_MFC_NO_CUSTOMIZE)
	int nSelCount = m_selection.GetCount();

	CDrawObj * pObj = NULL;
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


#endif

void CDrawView::OnFilePrintPreview()
{

	ASSERT_VALID( this );
	CPrintPreviewState * pState = new CPrintPreviewState;

	if( !DoPrintPreview(
			AFX_IDD_PREVIEW_TOOLBAR,
			this,
#ifndef FOR_CATTLEBASE
			RUNTIME_CLASS(CDrawPreviewView),
#else
			RUNTIME_CLASS(CMyPreviewView),
#endif
			pState
			)
		)
	{
		TRACE0("Error: DoPrintPreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
#ifdef FOR_CATTLEBASE
	else
		m_pCtrlDlg->EnableWindow(FALSE);
#endif


}
#ifndef FOR_CATTLEBASE
bool CDrawView::LoadDocImage(LPCTSTR szFileName)
{
	return m_BackDoc.Load(szFileName);
}

void CDrawView::OnAlignTops()
{
	POSITION pos = m_selection.GetHeadPosition();
    ASSERT( pos != NULL );
	CDrawObj * pObjBase = m_selection.GetNext( pos );
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
			CDrawObj * pObj = m_selection.GetNext( pos );
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

void CDrawView::OnAlignLefts()
{
	POSITION pos = m_selection.GetHeadPosition();
    ASSERT( pos != NULL );
	CDrawObj * pObjBase = m_selection.GetNext( pos );
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
			CDrawObj * pObj = m_selection.GetNext( pos );
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

void CDrawView::OnGroupPos()
{

CGroupPos dlg;
if(dlg.DoModal() == IDOK)
{
	CSize XY = dlg.GetPos();
	POSITION pos = m_selection.GetHeadPosition();
    ASSERT( pos != NULL );
	
		while( pos != NULL )
		{
			CDrawObj * pObj = m_selection.GetNext( pos );
			ASSERT( pObj != NULL );
			
			pObj->m_position.left += XY.cx;
			pObj->m_position.right += XY.cx;

			pObj->m_position.top -= XY.cy;
			pObj->m_position.bottom -= XY.cy;

			
		} // while( pos != NULL )

		Invalidate();

}


}

void CDrawView::OnShowGroupMgr()
{
	CGroupMgrDlg dlg(this);
	dlg.DoModal();
	OnUpdate(NULL, HINT_UPDATE_WINDOW, NULL);


}

void CDrawView::OnAddToGroup()
{
	CDrawObj * pObj = m_selection.GetHead();
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

void CDrawView::OnSelGroup()
{
	
	CDrawObjGroup *pObjGrp = m_selection.GetHead()->m_group;
	CDrawObj *pObj;
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
void CDrawView::OnShowStyleMgr()
{
CStyleMgr mgrDlg(this);
mgrDlg.DoModal();
Invalidate(FALSE);
//OnUpdate(NULL, HINT_UPDATE_TEXT_OBJECTS, NULL);
}


BOOL CDrawView::OnMakeSameSize(UINT nId)
{
	POSITION pos = m_selection.GetHeadPosition();
    ASSERT( pos != NULL );
	CDrawObj * pObjBase = m_selection.GetNext( pos );

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
		CDrawObj * pObj = m_selection.GetNext( pos );
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


void CDrawView::NormalizePosition(CRect &rect)
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



void CDrawView::OnTopBottom()
{
	CRect newPos;
	CDrawObj *pObj1 = m_selection.GetAt(m_selection.FindIndex(0));
	CDrawObj *pObj2 = m_selection.GetAt(m_selection.FindIndex(1));
	NormalizePosition(pObj1->m_position);
	NormalizePosition(pObj2->m_position);

	newPos.SetRect(pObj2->m_position.left,pObj1->m_position.bottom,pObj2->m_position.right,pObj1->m_position.bottom-abs(pObj2->m_position.Height()));
    pObj2->MoveTo(newPos);

}
void CDrawView::OnLeftRight()
{
	CRect newPos;
	CDrawObj *pObj1 = m_selection.GetAt(m_selection.FindIndex(0));
	CDrawObj *pObj2 = m_selection.GetAt(m_selection.FindIndex(1));
	NormalizePosition(pObj1->m_position);
	NormalizePosition(pObj2->m_position);

	newPos.SetRect(pObj1->m_position.right,pObj2->m_position.top,pObj1->m_position.right+pObj2->m_position.Width(),pObj2->m_position.bottom);
    pObj2->MoveTo(newPos);


}

void CDrawView::OnUpdateNeighbours(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_selection.GetCount() == 2);
}


#endif



/////////////////////////////////////////////////////////////////////////////
// CDrawPreviewView
#ifndef FOR_CATTLEBASE
IMPLEMENT_DYNCREATE(CDrawPreviewView, CPreviewView)

CDrawPreviewView::CDrawPreviewView()
{
}

LRESULT CDrawPreviewView::WindowProc(
									 UINT message,
									 WPARAM wParam,
									 LPARAM lParam
									 )
{
	LRESULT lResult;

	switch( message )
	{


	case WM_CREATE:
		lResult = CPreviewView::WindowProc(message,wParam,lParam);
		SetTimer( 789, 50, NULL );
		return lResult;

	case WM_TIMER:
		{
			if( wParam == 789 )
			{

				if(m_pToolBar->GetSafeHwnd() == NULL ||	(!::IsWindow(m_pToolBar->GetSafeHwnd())))
					return 0;

				VERIFY(SetupHookWndSink(m_pToolBar->GetSafeHwnd()));

				KillTimer( 789 );

				HINSTANCE hInstResource =
					AfxFindResourceHandle(MAKEINTRESOURCE( IDR_DRAWCLTYPE ),RT_GROUP_ICON);

				ASSERT( hInstResource != NULL );

				HWND hWndChild = ::GetWindow(m_pToolBar->GetSafeHwnd(),GW_CHILD);

				int nOffset = 0;
				for(;hWndChild != NULL; hWndChild = ::GetWindow(hWndChild,GW_HWNDNEXT))
				{
					CSubclassBtn * pBtn = new CSubclassBtn( hWndChild );
					ASSERT_VALID( pBtn );
					if( nOffset != 0 )
					{
						CRect rcWnd;
						pBtn->GetWindowRect( &rcWnd );
						m_pToolBar->ScreenToClient( &rcWnd );
						rcWnd.OffsetRect( nOffset, 0 );
						pBtn->MoveWindow( &rcWnd );
					}
					int nBtnID = pBtn->GetDlgCtrlID();
					UINT nResourceID = 0;
					bool bFlushText = false;
					bool bNcSeparator = false;
					switch( nBtnID )
					{
					case AFX_ID_PREVIEW_PRINT:
						nResourceID = ID_SCPPV_PRINT;
						break;
					case AFX_ID_PREVIEW_PREV:
						bFlushText = true;
						nResourceID = ID_SCPPV_NEXT;
						pBtn->SetDlgCtrlID( AFX_ID_PREVIEW_NEXT );
						break;
					case AFX_ID_PREVIEW_NEXT:
						bNcSeparator = true;
						bFlushText = true;
						nResourceID = ID_SCPPV_PREV;
						pBtn->SetDlgCtrlID( AFX_ID_PREVIEW_PREV );
						break;
					case AFX_ID_PREVIEW_NUMPAGE:
						bNcSeparator = true;
						nResourceID = ID_SCPPV_NUMPAGE;
						break;
					case AFX_ID_PREVIEW_ZOOMIN:
						bNcSeparator = true;
						bFlushText = true;
						nResourceID = ID_SCPPV_ZOOMIN;
						break;
					case AFX_ID_PREVIEW_ZOOMOUT:
						bFlushText = true;
						nResourceID = ID_SCPPV_ZOOMOUT;
						break;
					case AFX_ID_PREVIEW_CLOSE:
						bNcSeparator = true;
						nResourceID = ID_SCPPV_CLOSE;
						break;
					} // switch( nBtnID )
					if( nResourceID == 0 ) 	continue;

					CExtSafeString sTip;
					if( sTip.LoadString(nResourceID)) pBtn->SetTooltipText( sTip );

					HICON hIcon = (HICON)
						::LoadImage(hInstResource,MAKEINTRESOURCE( nResourceID ),
						IMAGE_ICON,16,16,0);

					ASSERT( hIcon != NULL );
					if( hIcon == NULL ) continue;

					pBtn->SetIcon( hIcon );

					int nAddWidth = 22;
					CRect rcWnd;
					pBtn->GetWindowRect( &rcWnd );
					m_pToolBar->ScreenToClient( &rcWnd );
					UINT nSwpFlags = SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOACTIVATE;
					if( bNcSeparator )
					{
						nAddWidth += CSubclassBtn::g_nNcSeparatorWidth;
						nSwpFlags |= SWP_FRAMECHANGED;
						pBtn->m_bNcSeparator = true;
					}
					nOffset += nAddWidth;

					if( bFlushText )
					{
						int nOldWidth = rcWnd.Width();
						rcWnd.right = rcWnd.left + nAddWidth;
						pBtn->SetWindowText( _T("") );
						nOffset -= nOldWidth;
					}
					else
						rcWnd.right += nAddWidth;
					pBtn->SetWindowPos(
						NULL,
						rcWnd.left, rcWnd.top,
						rcWnd.Width(), rcWnd.Height(),
						nSwpFlags
						);
				} // for( ; hWndChild != NULL; hWndChild = ::GetWindow(hWndChild,GW_HWNDNEXT) )
				//&&&&&*****######
				m_pToolBar->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);

				m_pToolBar->RedrawWindow(
					NULL,
					NULL,
					RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW
					|RDW_FRAME|RDW_ALLCHILDREN
					);
				return 0;
			} // if( wParam == 789 )
		}
		break; // case WM_TIMER
	default:
		if( message == CExtPopupMenuWnd::g_nMsgPrepareMenu )
		{
			CExtPopupMenuWnd::MsgPrepareMenuData_t * pData =
				reinterpret_cast
				< CExtPopupMenuWnd::MsgPrepareMenuData_t * >
				( wParam );
			ASSERT( pData != NULL );
			CExtPopupMenuWnd * pPopup = pData->m_pPopup;
			ASSERT( pPopup != NULL );
			INT nReplacePos =
				pPopup->ItemFindPosForCmdID(ID_OLE_VERB_FIRST);
			for( ;	nReplacePos >= 0;
				nReplacePos =
				pPopup->ItemFindPosForCmdID(ID_OLE_VERB_FIRST)
				/////nReplacePos = -1
				)
			{
				// delete OLE verb entry
				VERIFY( pPopup->ItemRemove(nReplacePos) );
				if( nReplacePos > 0
					&& pPopup->ItemGetCmdID(nReplacePos-1) == ID_SEPARATOR
					)
				{	// delete separator befire OLE verb entry
					VERIFY( pPopup->ItemRemove(nReplacePos-1) );
				}	// delete separator befire OLE verb entry
			}
			return TRUE;
		} // if( message == CExtPopupMenuWnd::g_nMsgPrepareMenu )
		break; // default
	} // switch( message )

	lResult = CPreviewView::WindowProc(message,wParam,lParam);
	return lResult;
}

bool CDrawPreviewView::OnHookWndMsg(
									LRESULT & lResult,
									HWND hWndHooked,
									UINT nMessage,
									WPARAM & wParam,
									LPARAM & lParam
									)
{
	if(		m_pToolBar->GetSafeHwnd() != NULL
		&&	::IsWindow(m_pToolBar->GetSafeHwnd())
		&&	CWnd::FromHandlePermanent(m_pToolBar->GetSafeHwnd()) != NULL
		&&	hWndHooked == m_pToolBar->GetSafeHwnd()
		)
	{ // if this is dialog bar message
		switch( nMessage )
		{
		case WM_ERASEBKGND:
			{
				HDC hDcErase = (HDC)wParam;
				ASSERT( hDcErase != NULL );
				CDC dc;
				dc.Attach( hDcErase );
				CRect rcClient;
				m_pToolBar->GetClientRect( &rcClient );
				dc.FillSolidRect(
					rcClient,
					g_PaintManager->GetColor(
					CExtPaintManager::CLR_3DFACE_OUT,
					this
					)
					);
				dc.Detach();
			}
			return (!0); // case WM_ERASEBKGND
		case WM_PAINT:
			{
				CPaintDC dc( m_pToolBar );
				CRect rcClient;
				m_pToolBar->GetClientRect( &rcClient );
				dc.FillSolidRect(
					rcClient,
					g_PaintManager->GetColor(
					CExtPaintManager::CLR_3DFACE_OUT,
					this
					)
					);
			}
			return 0; // case WM_PAINT
		case WM_NCCALCSIZE:
			return 0;
		} // switch( nMessage )
	} // if this is dialog bar message

	bool bRetVal =
		CExtHookSink::OnHookWndMsg(
		lResult,
		hWndHooked,
		nMessage,
		wParam,
		lParam
		);
	return bRetVal;
}

const int CDrawPreviewView::CSubclassBtn::g_nNcSeparatorWidth = 16;

CDrawPreviewView::CSubclassBtn::CSubclassBtn( HWND hWnd )
: m_bNcSeparator( false )
{
	ASSERT( hWnd != NULL );
	ASSERT( ::IsWindow(hWnd) );
	VERIFY( SubclassWindow( hWnd ) );
	SetFlat( TRUE );
	SetDrawBorder( TRUE );
}

void CDrawPreviewView::CSubclassBtn::PostNcDestroy()
{
	delete this;
}
LRESULT CDrawPreviewView::CSubclassBtn::WindowProc(
	UINT message,
	WPARAM wParam,
	LPARAM lParam
	)
{
	LRESULT lResult;

	switch( message )
	{

	case WM_NCCALCSIZE:
		{
			if( m_bNcSeparator )
			{
				NCCALCSIZE_PARAMS * pNCCSP =
					reinterpret_cast < NCCALCSIZE_PARAMS * > ( lParam );
				ASSERT( pNCCSP != NULL );

				CRect rcInBarWnd( pNCCSP->rgrc[0] );
				rcInBarWnd.DeflateRect(
					g_nNcSeparatorWidth, 0, 0, 0
					);
				::CopyRect( &(pNCCSP->rgrc[0]), rcInBarWnd );
			} // if( m_bNcSeparator )
		}
		return 0; // case WM_NCCALCSIZE

	case WM_NCPAINT:
		{
			if( m_bNcSeparator )
			{
				CRect rcInBarWnd, rcInBarClient;
				GetWindowRect( &rcInBarWnd );
				GetClientRect( &rcInBarClient );
				ClientToScreen( &rcInBarClient );
				if( rcInBarWnd == rcInBarClient )
					return 0;
				CPoint ptDevOffset = -rcInBarWnd.TopLeft();
				rcInBarWnd.OffsetRect( ptDevOffset );
				rcInBarClient.OffsetRect( ptDevOffset );

				CWindowDC dc( this );
				ASSERT( dc.GetSafeHdc() != NULL );
				dc.ExcludeClipRect( &rcInBarClient );

				CRect rcGrip( rcInBarWnd );
				rcGrip.right = rcGrip.left + g_nNcSeparatorWidth;
				dc.FillSolidRect(
					&rcGrip,
					g_PaintManager->GetColor(
					CExtPaintManager::CLR_3DFACE_OUT,
					this
					)
					);
				rcGrip.left += rcGrip.Width()/2;
				rcGrip.right = rcGrip.left + 2;
				g_PaintManager->PaintSeparator(
					dc,
					rcGrip,
					true,
					false,
					NULL,
					0L
					);
			} // if( m_bNcSeparator )
		}
		return 0; // case WM_NCPAINT

	} // switch( message )

	lResult = CExtButton::WindowProc(message,wParam,lParam);
	return lResult;
}
#endif
