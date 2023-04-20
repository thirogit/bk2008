// splitfrm.cpp : implementation file
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
#include "drawcli.h"
#include "splitfrm.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitFrame

IMPLEMENT_DYNCREATE(CSplitFrame, CMDIChildWnd)

CSplitFrame::CSplitFrame()
{

	m_hChildFrameIcon = NULL;
HINSTANCE hInstResource =
		AfxFindResourceHandle(
			MAKEINTRESOURCE(IDR_DRAWCLTYPE),
			RT_GROUP_ICON
			);
	ASSERT( hInstResource != NULL );
	if( hInstResource != NULL )
	{
		m_hChildFrameIcon =
			(HICON)::LoadImage(
				hInstResource,
				MAKEINTRESOURCE(IDR_DRAWCLTYPE),
				IMAGE_ICON,
				16,
				16,
				0
				);
		ASSERT( m_hChildFrameIcon != NULL );
	}

}

CSplitFrame::~CSplitFrame()
{
	if( m_hChildFrameIcon != NULL )
		::DestroyIcon( m_hChildFrameIcon );
}

BOOL CSplitFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	return m_wndSplitter.Create(this,
		2, 2,       // TODO: adjust the number of rows, columns
		CSize(10, 10),  // TODO: adjust the minimum pane size
		pContext);
}

BEGIN_MESSAGE_MAP(CSplitFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSplitFrame)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_NCPAINT()
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitFrame message handlers

BOOL CSplitFrame::PreTranslateMessage(MSG* pMsg) 
{
CMainFrame * pMainFrame =
		DYNAMIC_DOWNCAST(
			CMainFrame,
			GetParentFrame()
			);
	ASSERT_VALID( pMainFrame );
	if( pMainFrame->m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

void CSplitFrame::ActivateFrame(int nCmdShow) 
{
	nCmdShow = SW_SHOWMAXIMIZED;

	CMDIChildWnd::ActivateFrame(nCmdShow);
}

int CSplitFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ASSERT( m_hChildFrameIcon != NULL );
	SetIcon( m_hChildFrameIcon, FALSE );
	SetIcon( m_hChildFrameIcon, TRUE );

	return 0;
}

BOOL CSplitFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return false;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE);
	return TRUE;
}

void CSplitFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	// OnWindowPosChanged() and OnNcPaint() methods
	// implemented to fix WinXP MDI interface flicker bug
CMDIFrameWnd * pMdiFrame =
		STATIC_DOWNCAST( CMDIFrameWnd, GetParentFrame() );
	ASSERT_VALID( pMdiFrame );
BOOL bMax = FALSE;
CMDIChildWnd * pActive = pMdiFrame->MDIGetActive( &bMax );
	pActive;
	if( ! bMax )
	{
		CMDIChildWnd::OnWindowPosChanged(lpwndpos);
		return;
	} // if( ! bMax )
	SetRedraw( FALSE );
	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	SetRedraw( TRUE );
	RedrawWindow(
		NULL, NULL,
		RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_FRAME
		);
}

void CSplitFrame::OnNcPaint() 
{
	// OnWindowPosChanged() and OnNcPaint() methods
	// implemented to fix WinXP MDI interface flicker bug
CMDIFrameWnd * pMdiFrame =
		STATIC_DOWNCAST( CMDIFrameWnd, GetParentFrame() );
	ASSERT_VALID( pMdiFrame );
BOOL bMax = FALSE;
CMDIChildWnd * pActive = pMdiFrame->MDIGetActive( &bMax );
	pActive;
	if( bMax )
		return;
	CMDIChildWnd::OnNcPaint();
}


void CSplitFrame::OnMDIActivate(BOOL bActivate,CWnd* pActivateWnd, CWnd* pDeactivateWnd )
{
	CMDIChildWnd::OnMDIActivate(bActivate,pActivateWnd,  pDeactivateWnd );
if(bActivate)
{
	CDrawView *dView = (CDrawView*)GetActiveView();

	CMainFrame *Main = GetMainFrame();
	CExtToolControlBar *zoomBar = &Main->m_wndToolBarZoom;


	CMainFrame::CZoomBarSliderButton * pZoomSliderTBB = (CMainFrame::CZoomBarSliderButton *)
			zoomBar->GetButton(zoomBar->CommandToIndex(ID_MYEXTBTN_SLIDER));
	

	pZoomSliderTBB->SetZoomPos(dView->GetZoom());
}

	
}