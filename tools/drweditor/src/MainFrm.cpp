#include "stdafx.h"
#include "drawcli.h"

#include "mainfrm.h"
#include "drawdoc.h"
#include "drawvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_MDITABS_TOP, OnMditabsTop)
	ON_UPDATE_COMMAND_UI(ID_MDITABS_TOP, OnUpdateMditabsTop)
	ON_COMMAND(ID_MDITABS_BOTTOM, OnMditabsBottom)
	ON_UPDATE_COMMAND_UI(ID_MDITABS_BOTTOM, OnUpdateMditabsBottom)
	ON_COMMAND(ID_MDITABS_LEFT, OnMditabsLeft)
	ON_UPDATE_COMMAND_UI(ID_MDITABS_LEFT, OnUpdateMditabsLeft)
	ON_COMMAND(ID_MDITABS_RIGHT, OnMditabsRight)
	ON_UPDATE_COMMAND_UI(ID_MDITABS_RIGHT, OnUpdateMditabsRight)
	ON_COMMAND(ID_MDITABS_SHOW_CLOSE, OnMditabsShowClose)
	ON_UPDATE_COMMAND_UI(ID_MDITABS_SHOW_CLOSE, OnUpdateMditabsShowClose)
	ON_COMMAND(ID_MDITABS_SHOW_CTX_HELP, OnMditabsShowCtxHelp)
	ON_UPDATE_COMMAND_UI(ID_MDITABS_SHOW_CTX_HELP, OnUpdateMditabsShowCtxHelp)
	ON_COMMAND(ID_UISTYLE_2000, OnUistyle2000)
	ON_UPDATE_COMMAND_UI(ID_UISTYLE_2000, OnUpdateUistyle2000)
	ON_COMMAND(ID_UISTYLE_XP, OnUistyleXp)
	ON_UPDATE_COMMAND_UI(ID_UISTYLE_XP, OnUpdateUistyleXp)
	ON_COMMAND(ID_VIEW_LIKE_OFFICE_2003, OnViewLikeOffice2003)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIKE_OFFICE_2003, OnUpdateViewLikeOffice2003)
	ON_COMMAND(ID_VIEW_LIKE_STUDIO_2005, OnViewLikeStudio2005)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIKE_STUDIO_2005, OnUpdateViewLikeStudio2005)
	ON_COMMAND(ID_CONTEXT_HELP, OnContextHelp)
	ON_UPDATE_COMMAND_UI(ID_CONTEXT_HELP, OnUpdateContextHelp)
	ON_COMMAND(ID_CUSTOMIZE, OnCustomize)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE, OnUpdateCustomize)
	//}}AFX_MSG_MAP

	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_TOOLBAR2, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR2, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_PALETTE, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_PALETTE, OnUpdateControlBarMenu)

	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPrepareMenu,
		OnExtMenuPrepare
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPopupDrawItem,
		OnDrawPopupMenuItem
		)

	// color popup menu
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyCustColor,
		OnColorSelectCustom
		)


	ON_COMMAND_EX( IDR_TOOLBAR_ZOOM, OnBarCheck )
	ON_UPDATE_COMMAND_UI( IDR_TOOLBAR_ZOOM, OnUpdateControlBarMenu )

	

//	ON_WM_MDIACTIVATE()
	
END_MESSAGE_MAP()

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
#if (defined __EXT_MFC_NO_CUSTOMIZE)
	m_pBtnColorFill = m_pBtnColorOutline = NULL;
#endif // (defined __EXT_MFC_NO_CUSTOMIZE)

	g_PaintManager.InstallPaintManager(
		RUNTIME_CLASS(CExtPaintManagerOffice2003)
		);

	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;

}

CMainFrame::~CMainFrame()
{
}

static UINT g_statBasicCommands[] =
{
	ID_APP_ABOUT,
	ID_APP_EXIT,
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
	ID_EDIT_SELECT_ALL,
	ID_OLE_INSERT_NEW,
	ID_OLE_EDIT_LINKS,
	ID_DRAW_SELECT,
	ID_DRAW_LINE,
	ID_DRAW_RECT,
	ID_DRAW_ROUNDRECT,
	ID_DRAW_ELLIPSE,
	ID_DRAW_POLYGON,
	ID_OBJECT_LINECOLOR,
	ID_OBJECT_FILLCOLOR,
	ID_VIEW_MENUBAR,
	ID_VIEW_TOOLBAR,
	ID_VIEW_TOOLBAR2,
	ID_VIEW_PALETTE,
	ID_VIEW_STATUS_BAR,
	ID_MDITABS_TOP,
	ID_MDITABS_BOTTOM,
	ID_MDITABS_LEFT,
	ID_MDITABS_RIGHT,
	ID_MDITABS_SHOW_CLOSE,
	ID_MDITABS_SHOW_CTX_HELP,
	ID_UISTYLE_2000,
	ID_UISTYLE_XP,
	ID_VIEW_LIKE_OFFICE_2003,
	ID_VIEW_LIKE_STUDIO_2005,
	ID_EDIT_COPY,
	ID_EDIT_CUT,
	ID_EDIT_PASTE,
	ID_WINDOW_NEW,
	ID_WINDOW_CASCADE,
	ID_WINDOW_TILE_HORZ,
	ID_WINDOW_ARRANGE,
	ID_LINE_WIDTH_OTHER,
	ID_LINE_WIDTH_0,
	ID_LINE_WIDTH_1,
	ID_LINE_WIDTH_2,
	ID_LINE_WIDTH_3,
	ID_LINE_WIDTH_4,
	ID_LINE_WIDTH_5,
	ID_LINE_WIDTH_6,
	ID_LINE_WIDTH_7,
	ID_LINE_WIDTH_8,
	ID_LINE_WIDTH_9,
	ID_LINE_WIDTH_10,
	ID_DRAW_TEXT,
	ID_DRAW_IMAGE,
	ID_NEW_PAGE,
	ID_NEXT_PAGE,
	ID_PREV_PAGE,
	ID_DEL_PAGE,
	0, // end of list
};

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

HINSTANCE hInstResource =
		AfxFindResourceHandle(
			MAKEINTRESOURCE( IDR_MAINFRAME ),
			RT_GROUP_ICON
			);
	ASSERT( hInstResource != NULL );
HICON hIcon = (HICON)
		::LoadImage(
			hInstResource,
			MAKEINTRESOURCE( IDR_MAINFRAME ),
			IMAGE_ICON,
			16,
			16,
			0
			);
	ASSERT( hIcon != NULL );
	SetIcon( hIcon, FALSE );

CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	hIcon = pApp->LoadIcon(IDR_MAINFRAME);
	ASSERT( hIcon != NULL );
	SetIcon( hIcon, TRUE );

	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	ASSERT( pApp->m_pszProfileName != NULL );
	g_CmdManager->ProfileSetup(
		pApp->m_pszProfileName,
		GetSafeHwnd()
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_MAINFRAME
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_DRAWCLTYPE
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromToolBar(
			pApp->m_pszProfileName,
			IDR_TOOLBAR_HELPER_ICONS
			)
		);

	m_wndMenuBar.SetMdiWindowPopupName( _T("Window") );
	if(	!m_wndMenuBar.Create(
			NULL,
			this,
			ID_VIEW_MENUBAR
			)
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }

	if( !m_wndToolBarStandard.Create(
			NULL,
			this,
			AFX_IDW_TOOLBAR
			)
		||
		!m_wndToolBarStandard.LoadToolBar(IDR_MAINFRAME)
		)
	{
		TRACE0("Failed to create standard toolbar\n");
		return -1;      // fail to create
	}

LPCTSTR strProfileName =
		g_CmdManager->ProfileNameFromWnd( GetSafeHwnd() );
	ASSERT( strProfileName != NULL );
CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr( strProfileName, ID_OBJECT_FILLCOLOR );
	ASSERT( pCmdItem != NULL );
	pCmdItem->StateSetPersistentIcon();
	pCmdItem->m_nLParamUserData = (LPARAM)pCmdItem->m_nCmdID;
	pCmdItem->StateSetColor();
	pCmdItem->StateSetColorBtnDefault();
	pCmdItem->StateSetColorBtnCustom();
	pCmdItem->StateSetNoRotateVL();
	pCmdItem->StateSetSeparatedDD();

	pCmdItem =
		g_CmdManager->CmdGetPtr( strProfileName, ID_OBJECT_LINECOLOR );
	ASSERT( pCmdItem != NULL );
	pCmdItem->StateSetPersistentIcon();
	pCmdItem->m_nLParamUserData = (LPARAM)pCmdItem->m_nCmdID;
	pCmdItem->StateSetColor();
	pCmdItem->StateSetColorBtnDefault();
	pCmdItem->StateSetColorBtnCustom();
	pCmdItem->StateSetNoRotateVL();
	
	pCmdItem =
		g_CmdManager->CmdGetPtr( strProfileName, ID_DOC_LINE_WIDTH );
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sMenuText = _T("Line &Width");
	pCmdItem->StateSetPersistentIcon();


INT nBtnIdx = m_wndToolBarStandard.CommandToIndex(ID_DOC_LINE_WIDTH);
	ASSERT( nBtnIdx >= 0 );
CMenu _line_menu;
	VERIFY( _line_menu.LoadMenu(IDR_MENU_LW) );
	VERIFY( m_wndToolBarStandard.SetButtonMenu( nBtnIdx, _line_menu.Detach() ) );
CExtBarButton * pTBB = m_wndToolBarStandard.GetButton(nBtnIdx);
	ASSERT_VALID( pTBB );
	pTBB->SetNoRotateVerticalLayout();
	pTBB->SetSeparatedDropDown();
	pTBB->SetAutoChangeID();
	pTBB->SetCmdID(ID_LINE_WIDTH_1,true); // set default effective command

#if (defined __EXT_MFC_NO_CUSTOMIZE)
	nBtnIdx = m_wndToolBarStandard.CommandToIndex(ID_OBJECT_FILLCOLOR);
	VERIFY(
		m_wndToolBarStandard.RemoveButton(
			nBtnIdx,
			FALSE
			)
		);
CString sNoFill;
	VERIFY( sNoFill.LoadString( IDS_STRING_NO_COLOR_FILL ) );
	m_pBtnColorFill =
		new CExtBarColorButton(
			&m_wndToolBarStandard,
			ID_OBJECT_FILLCOLOR, 0, COLORREF(-1), RGB(0,0,0),
			ID_OBJECT_FILLCOLOR, true, true,
			(LPCTSTR)sNoFill, NULL,
			CExtBarColorButton::__DIT_CHAR
			);
	m_pBtnColorFill->SetSeparatedDropDown();
	VERIFY(
		m_wndToolBarStandard.InsertSpecButton(
			nBtnIdx,
			m_pBtnColorFill,
			FALSE
			)
		);
	
	nBtnIdx = m_wndToolBarStandard.CommandToIndex(ID_OBJECT_LINECOLOR);
	m_wndToolBarStandard.SetButtonCtrlVisibleVertically( nBtnIdx, true );
	VERIFY(
		m_wndToolBarStandard.RemoveButton(
			nBtnIdx,
			FALSE
			)
		);
CString sNoOutline;
	VERIFY( sNoOutline.LoadString( IDS_STRING_NO_COLOR_OUTLINE ) );
	m_pBtnColorOutline =
		new CExtBarColorButton(
			&m_wndToolBarStandard,
			ID_OBJECT_LINECOLOR, 0, COLORREF(-1), RGB(0,0,0),
			ID_OBJECT_LINECOLOR, true, true,
			(LPCTSTR)sNoOutline, NULL,
			CExtBarColorButton::__DIT_FRAME
			);
	VERIFY(
		m_wndToolBarStandard.InsertSpecButton(
			nBtnIdx,
			m_pBtnColorOutline,
			FALSE
			)
		);

#endif // (defined __EXT_MFC_NO_CUSTOMIZE)

	if( !m_wndToolBarCustom.Create(
			NULL,
			this,
			ID_VIEW_TOOLBAR2
			)
		|| !m_wndToolBarCustom.InitContentExpandButton()
		)
	{
		TRACE0("Failed to create custom toolbar\n");
		return -1;      // fail to create
	}
static struct
{
	UINT m_nCmdID;
	LPCTSTR m_sToolbarText;
}
custom_toolbar_cmds[] =
{
	{ ID_OBJECT_MOVETOFRONT,	_T("Move to front") },
	{ ID_OBJECT_MOVETOBACK,		_T("Move to back") },
	{ ID_SEPARATOR,				NULL },
	{ ID_OBJECT_MOVEFORWARD,	NULL },
	{ ID_OBJECT_MOVEBACK,		NULL },
};
	for(	INT nCustBarIdx = 0;
			nCustBarIdx < sizeof(custom_toolbar_cmds)/sizeof(custom_toolbar_cmds[0]);
			nCustBarIdx++
			)
	{ // add buttons to custom toolbar
		UINT nCmdID = custom_toolbar_cmds[nCustBarIdx].m_nCmdID;
		if( nCmdID != ID_SEPARATOR )
		{
			LPCTSTR sToolBarBtnText =
				custom_toolbar_cmds[nCustBarIdx].m_sToolbarText;
			if( sToolBarBtnText != NULL )
			{
				CExtCmdItem * pCmdItem =
					g_CmdManager->CmdGetPtr(
						strProfileName,
						nCmdID
						);
				ASSERT( pCmdItem != NULL );
				pCmdItem->m_sToolbarText = sToolBarBtnText;
			} // if( sToolBarBtnText != NULL )
			HICON hIcon = (HICON)
				::LoadImage(
					hInstResource,
					MAKEINTRESOURCE( nCmdID ),
					IMAGE_ICON,
					16,
					16,
					0
					);
			if( hIcon != NULL )
			{
				VERIFY(
					g_CmdManager->CmdSetIcon(
						g_CmdManager->ProfileNameFromWnd(GetSafeHwnd()),
						nCmdID,
						hIcon,
						false
						)
					);
			} // if( hIcon != NULL )
		} // if( nCmdID != ID_SEPARATOR )
		m_wndToolBarCustom.InsertButton( -1, nCmdID, FALSE );

	} // add buttons to custom toolbar

	m_wndPalette.m_bPaletteMode = true;
	if( !m_wndPalette.Create(
			NULL,
			this,
			ID_VIEW_PALETTE
			)
		||
		!m_wndPalette.LoadToolBar(IDR_TOOLBAR_HELPER_ICONS)
		)
	{
		TRACE0("Failed to create m_wndPalette toolbar\n");
		return -1;      // fail to create
	}

	m_wndPalette.GetButton(3)->SetWrap( CExtBarButton::__EVT_HORZ );
	m_wndPalette.GetButton(8)->SetWrap( CExtBarButton::__EVT_HORZ );

	m_wndPalette.GetButton(3)->SetWrap( CExtBarButton::__EVT_VERT );
	m_wndPalette.GetButton(6)->SetWrap( CExtBarButton::__EVT_VERT );
	m_wndPalette.GetButton(9)->SetWrap( CExtBarButton::__EVT_VERT );
	m_wndPalette.GetButton(12)->SetWrap( CExtBarButton::__EVT_VERT );

	m_wndPalette.GetButton(3)->SetWrap( CExtBarButton::__EVT_FLOAT );
	m_wndPalette.GetButton(5)->SetWrap( CExtBarButton::__EVT_FLOAT );
	m_wndPalette.GetButton(7)->SetWrap( CExtBarButton::__EVT_FLOAT );
	m_wndPalette.GetButton(9)->SetWrap( CExtBarButton::__EVT_FLOAT );
	m_wndPalette.GetButton(11)->SetWrap( CExtBarButton::__EVT_FLOAT );

	m_wndStatusBar.m_bDrawPaneSeparatorsInsteadOfBorders = true;
	if(	!m_wndStatusBar.Create(
			this,
			WS_CHILD|WS_VISIBLE|CBRS_BOTTOM|CBRS_HIDE_INPLACE
			)
		||	!m_wndStatusBar.SetIndicators(
				indicators,
				sizeof(indicators) / sizeof(UINT)
				)
			)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.SetPaneWidth(0, 90);
	m_wndStatusBar.AddPane(IDS_PANE_ZOOM, 1);
	m_wndStatusBar.SetPaneWidth(1, 40);
	m_wndStatusBar.AddPane(IDS_PANE_POS, 2);
	m_wndStatusBar.SetPaneWidth(2, 120);
	m_wndStatusBar.AddPane(IDS_PANE_SIZE, 3);
	m_wndStatusBar.SetPaneWidth(3, 120);
		
	


//****ZOOM

   	if( !m_wndToolBarZoom.Create(
		_T("Zoom Slider"),
		this,
		IDR_TOOLBAR_ZOOM)
		)
	{
		TRACE0("Failed to create toolbar m_wndToolBarZoom \n");
		return -1;      // fail to create
	}
	m_wndToolBarZoom.InitContentExpandButton();
	
	CExtCmdItem * pCmdItemSlider =	g_CmdManager->CmdAllocPtr(pApp->m_pszProfileName,ID_MYEXTBTN_SLIDER	);

	ASSERT( pCmdItemSlider != NULL );
	pCmdItemSlider->m_sToolbarText = _T("Zoom:");
	pCmdItemSlider->m_sMenuText = _T("Zoom slider");
	pCmdItemSlider->m_sTipTool 	= pCmdItemSlider->m_sTipStatus = _T("Zoom slider");

	CZoomBarSliderButton *pZoomSliderTBB = 	new CZoomBarSliderButton(
		/////////////////
		// standard CExtBarButton parms
		/////////////////
		&m_wndToolBarZoom,
		ID_MYEXTBTN_SLIDER,
		0,
		/////////////////
		// CExtBarSliderButton specific parms
		/////////////////
		//
		// scroll total/pos/page
		2, 0, 0,
		// button extent horz(left/right)/vert(up/down) in pixels
		// (if zeros - use slider-like layout instead of scrollbar-like)
		0, 0,
		// total slider control extent horz/vert in pixels
		100, 100
		);
	VERIFY(	m_wndToolBarZoom.InsertSpecButton(-1,pZoomSliderTBB,FALSE));

	m_wndToolBarZoom.EnableDocking(CBRS_ALIGN_ANY);
//****ZOOM END

	
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarStandard.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarCustom.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPalette.EnableDocking(CBRS_ALIGN_ANY);

	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
	VERIFY(
		g_CmdManager->SetBasicCommands(
			pApp->m_pszProfileName,
			g_statBasicCommands
			)
		);

//	g_CmdManager->CmdGetPtr(
//		pApp->m_pszProfileName,
//		ID_FILE_NEW
//		)->StateSetCannotHide();

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	VERIFY(
		CExtCustomizeSite::MenuInfoAdd(
			this,
			_T("Default"),
			IDR_MAINFRAME,
			true,
			false,
			RUNTIME_CLASS( CMainFrame )
			)
		);
	VERIFY(
		CExtCustomizeSite::MenuInfoAdd(
			this,
			_T("Document"),
			IDR_DRAWCLTYPE,
			false,
			false,
			RUNTIME_CLASS( CMDIChildWnd ),
			RUNTIME_CLASS( CView ),
			RUNTIME_CLASS( CDocument )
			)
		);
	VERIFY(
		CExtCustomizeSite::MenuInfoLoadAccelTable(
			_T("Default"),
			IDR_MAINFRAME
			)
		);
	VERIFY(
		CExtCustomizeSite::MenuInfoLoadAccelTable(
			_T("Document"),
			IDR_MAINFRAME
			)
		);
	CExtCustomizeSite::MenuInfoGetByName( _T("Document") )
		-> GetNode( true )
			-> SearchNodeElement( _T("&Window"), 1 )
				-> ModifyFlags( __ECTN_TBB_APPEND_MDI_MENU );
	CExtCustomizeSite::MenuInfoGetByName( _T("Document") )
		-> GetNode( false )
			-> SearchNodeElement( _T("&Window"), 1 )
				-> ModifyFlags( __ECTN_TBB_APPEND_MDI_MENU );
	if( !CExtCustomizeSite::EnableCustomization( this ) )
	{
		ASSERT( FALSE );
		return -1;
	}
	CExtCustomizeSite::CategoryUpdate( IDR_DRAWCLTYPE );
	CExtCustomizeSite::CategoryUpdate( IDR_MENU_HELPER );
	CExtCustomizeSite::CategoryUpdate( IDR_MAINFRAME );

CExtCustomizeCmdTreeNode * pObjectCategory = 
		CExtCustomizeSite::CategoryGetTreeByName( _T("Object") );
	ASSERT_VALID( pObjectCategory );

CExtCustomizeCmdTreeNode * pMenu = 
		pObjectCategory->SearchNodeElement( ID_DOC_LINE_WIDTH );
	ASSERT_VALID( pMenu );
	VERIFY( pMenu->LoadMenuTree( this, this, IDR_MENU_LW ) );
	pMenu->ModifyFlags(
		__ECTN_TBB_SEPARATED_DROPDOWN
		|__ECTN_TBB_AUTOCHANGE_ID
		|__ECTN_TBB_NO_ROTATE_VL
		);
	pMenu->SetCmdID(ID_LINE_WIDTH_1,true); // set default effective command

/*
	pMenu = 
		pObjectCategory->SearchNodeElement( ID_OBJECT_FILLCOLOR );
	ASSERT_VALID( pMenu );
	pMenu->ModifyFlags( __ECTN_AUTO_POPUP );

	pMenu = 
		pObjectCategory->SearchNodeElement( ID_OBJECT_LINECOLOR );
	ASSERT_VALID( pMenu );
	pMenu->ModifyFlags( __ECTN_AUTO_POPUP );
*/

	CExtCustomizeSite::CategoryMakeAllCmdsUnique();
	CExtCustomizeSite::CategoryAppendAllCommands();
	CExtCustomizeSite::CategoryAppendNewMenu();

	CExtCustomizeSite::CustomizeStateLoad(
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		pApp->m_pszProfileName
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	if(	!CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		)
	{
		DockControlBar( &m_wndMenuBar );
		DockControlBar( &m_wndToolBarStandard );
		DockControlBar( &m_wndToolBarCustom );
		DockControlBar( &m_wndPalette );
		DockControlBar( &m_wndToolBarZoom );
		
		CRect wrAlredyDockedBar;

		m_wndToolBarZoom.GetWindowRect( &wrAlredyDockedBar );
		wrAlredyDockedBar.OffsetRect( 1, 0 );
	}
	
	/*g_CmdManager->SerializeState(
		pApp->m_pszProfileName,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		false
		);*/

#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	if( !m_wndMdiTabs.Create( this ) )
	{
		ASSERT( FALSE );
		return -1;
	}
	m_wndMdiTabs.ModifyTabWndStyle(
		0,
		__ETWS_ONENOTE_DEFAULT
			| __ETWS_SHOW_BTN_TAB_LIST
			| __ETWS_ENABLED_BTN_TAB_LIST
			| __ETWS_ITEM_DRAGGING
			| __ETWS_SHOW_BTN_CLOSE
		,
		true
		);

#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;

	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::DestroyWindow() 
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );

	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	VERIFY(
		CExtControlBar::ProfileBarStateSave(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		);

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	VERIFY(
		CExtCustomizeSite::CustomizeStateSave(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

	VERIFY(
		g_CmdManager->SerializeState(
			pApp->m_pszProfileName,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			true
			)
		);
	g_CmdManager->ProfileWndRemove( GetSafeHwnd() );
	
	return CMDIFrameWnd::DestroyWindow();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CFrameWnd::ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	
	CMDIFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::RecalcLayout(BOOL bNotify)
{
	CMDIFrameWnd::RecalcLayout(bNotify);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.lpszClass = ::AfxRegisterWndClass(CS_DBLCLKS);
	ASSERT( cs.lpszClass != NULL );
	cs.style =
		WS_OVERLAPPED|WS_CAPTION|WS_THICKFRAME|WS_SYSMENU
		|WS_MINIMIZEBOX|WS_MAXIMIZEBOX
		|FWS_ADDTOTITLE
		;
	cs.dwExStyle &= ~ (WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
	return TRUE;
}

LRESULT CMainFrame::OnExtMenuPrepare(WPARAM wParam, LPARAM lParam)
{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

	//////////////////////////////////////////////////////////////////////////
	// Add "Windows..." command
	//////////////////////////////////////////////////////////////////////////
	
	CExtPopupMenuWnd::MsgPrepareMenuData_t * pData =
		reinterpret_cast
		< CExtPopupMenuWnd::MsgPrepareMenuData_t * >
		( wParam );
	ASSERT( pData != NULL );
	CExtPopupMenuWnd * pPopup = pData->m_pPopup;
	ASSERT( pPopup != NULL );
	
	INT nItemPos;
	INT nNewPos = -1;
	
	nItemPos = pPopup->ItemFindPosForCmdID( __ID_MDIWND_DLGWINDOWS );
	if( nItemPos > 0 )
	{
		// "More Windows..." command found
		pPopup->ItemRemove( nItemPos );
		nNewPos = nItemPos;
	}
	else
	{
		int nMaxCmdID = 0;
		for( int nCmdID = __ID_MDIWNDLIST_FIRST; nCmdID <= __ID_MDIWNDLIST_LAST; nCmdID++ ){
			nItemPos = pPopup->ItemFindPosForCmdID( nCmdID );
			if( nItemPos > 0 ){
				if( nCmdID > nMaxCmdID ){
					nMaxCmdID = nCmdID;
					nNewPos = nItemPos;
				}
			}
		}
		if( nNewPos > 0 ){
			pPopup->ItemInsert(
				(UINT)CExtPopupMenuWnd::TYPE_SEPARATOR,
				++nNewPos
				);
			nNewPos++;
		}
	}
	if( nNewPos > 0 )
	{
		UINT nCmdID = ID_WINDOWS_LIST;
		CExtCmdItem * pCmdItem =
			g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( this->GetSafeHwnd() ),
			nCmdID
			);
		if( pCmdItem == NULL ){
			pCmdItem = 
				g_CmdManager->CmdAllocPtr( 
				g_CmdManager->ProfileNameFromWnd( this->GetSafeHwnd() ), 
				nCmdID 
				);
		}
		ASSERT( pCmdItem != NULL );
		if( pCmdItem != NULL )
		{
			CString sMoreWindows(_T("Windows..."));
			CString sManageWindows(_T("Manages the currently open windows"));
			pCmdItem->m_sMenuText = sMoreWindows;
			pCmdItem->m_sToolbarText = pCmdItem->m_sMenuText;
			pCmdItem->m_sTipTool = sManageWindows;
			pCmdItem->m_sTipStatus = pCmdItem->m_sTipTool;
			pCmdItem->StateSetBasic( true );
			
			pPopup->ItemInsert(
				nCmdID,
				nNewPos,
				sMoreWindows,
				NULL,
				m_hWnd
				);
		}
	}

#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
	
	//////////////////////////////////////////////////////////////////////////

	CFrameWnd * pFrame = GetActiveFrame();
CView * pView = NULL;
	if( pFrame != this && pFrame != NULL )
	{
		ASSERT_VALID( pFrame );
		pView = pFrame->GetActiveView();
	}
	if( pView == NULL )
	{
		return FALSE;
	}
	ASSERT_VALID( pView );
	return
		pView->SendMessage(
			CExtPopupMenuWnd::g_nMsgPrepareMenu,
			wParam,
			lParam
			);
}

LRESULT CMainFrame::OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam)
{
CFrameWnd * pFrame = GetActiveFrame();
CView * pView = NULL;
	if( pFrame != this && pFrame != NULL )
	{
		ASSERT_VALID( pFrame );
		pView = pFrame->GetActiveView();
	}
	if( pView == NULL )
	{
		return FALSE;
	}
	ASSERT_VALID( pView );
	return
		pView->SendMessage(
			CExtPopupMenuWnd::g_nMsgPopupDrawItem,
			wParam,
			lParam
			);
}

LRESULT CMainFrame::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
CFrameWnd * pFrame = GetActiveFrame();
CView * pView = NULL;
	if( pFrame != this && pFrame != NULL )
	{
		ASSERT_VALID( pFrame );
		pView = pFrame->GetActiveView();
	}
	if( pView == NULL )
	{
		return FALSE;
	}
	ASSERT_VALID( pView );
	return
		pView->SendMessage(
			CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
			wParam,
			lParam
			);
}

LRESULT CMainFrame::OnColorSelectCustom(WPARAM wParam, LPARAM lParam)
{
CFrameWnd * pFrame = GetActiveFrame();
CView * pView = NULL;
	if( pFrame != this && pFrame != NULL )
	{
		ASSERT_VALID( pFrame );
		pView = pFrame->GetActiveView();
	}
	if( pView == NULL )
	{
		return FALSE;
	}
	ASSERT_VALID( pView );
	return
		pView->SendMessage(
			CExtPopupColorMenuWnd::g_nMsgNotifyCustColor,
			wParam,
			lParam
			);
}

void CMainFrame::OnMditabsTop() 
{
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	m_wndMdiTabs.OrientationSet( __ETWS_ORIENT_TOP );
	RecalcLayout();
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)
}
void CMainFrame::OnUpdateMditabsTop(CCmdUI* pCmdUI) 
{
#if (defined __EXT_MFC_NO_TABMDI_CTRL)
	pCmdUI->Enable( FALSE );
#else
	pCmdUI->SetRadio(
		(m_wndMdiTabs.OrientationGet() == __ETWS_ORIENT_TOP) ? TRUE : FALSE
		);
	pCmdUI->Enable( m_wndMdiTabs.GetSafeHwnd() != NULL && m_wndMdiTabs.IsWindowVisible() );
#endif // else from (defined __EXT_MFC_NO_TABMDI_CTRL)
}

void CMainFrame::OnMditabsBottom() 
{
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	m_wndMdiTabs.OrientationSet( __ETWS_ORIENT_BOTTOM );
	RecalcLayout();
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)
}
void CMainFrame::OnUpdateMditabsBottom(CCmdUI* pCmdUI) 
{
#if (defined __EXT_MFC_NO_TABMDI_CTRL)
	pCmdUI->Enable( FALSE );
#else
	pCmdUI->SetRadio(
		(m_wndMdiTabs.OrientationGet() == __ETWS_ORIENT_BOTTOM) ? TRUE : FALSE
		);
	pCmdUI->Enable( m_wndMdiTabs.GetSafeHwnd() != NULL && m_wndMdiTabs.IsWindowVisible() );
#endif // else from (defined __EXT_MFC_NO_TABMDI_CTRL)
}

void CMainFrame::OnMditabsLeft() 
{
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	m_wndMdiTabs.OrientationSet( __ETWS_ORIENT_LEFT );
	RecalcLayout();
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)
}
void CMainFrame::OnUpdateMditabsLeft(CCmdUI* pCmdUI) 
{
#if (defined __EXT_MFC_NO_TABMDI_CTRL)
	pCmdUI->Enable( FALSE );
#else
	pCmdUI->SetRadio(
		(m_wndMdiTabs.OrientationGet() == __ETWS_ORIENT_LEFT) ? TRUE : FALSE
		);
	pCmdUI->Enable( m_wndMdiTabs.GetSafeHwnd() != NULL && m_wndMdiTabs.IsWindowVisible() );
#endif // else from (defined __EXT_MFC_NO_TABMDI_CTRL)
}

void CMainFrame::OnMditabsRight() 
{
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	m_wndMdiTabs.OrientationSet( __ETWS_ORIENT_RIGHT );
	RecalcLayout();
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)
}
void CMainFrame::OnUpdateMditabsRight(CCmdUI* pCmdUI) 
{
#if (defined __EXT_MFC_NO_TABMDI_CTRL)
	pCmdUI->Enable( FALSE );
#else
	pCmdUI->SetRadio(
		(m_wndMdiTabs.OrientationGet() == __ETWS_ORIENT_RIGHT) ? TRUE : FALSE
		);
	pCmdUI->Enable( m_wndMdiTabs.GetSafeHwnd() != NULL && m_wndMdiTabs.IsWindowVisible() );
#endif // else from (defined __EXT_MFC_NO_TABMDI_CTRL)
}

void CMainFrame::OnMditabsShowClose() 
{
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	if( m_wndMdiTabs.GetTabWndStyle() & __ETWS_SHOW_BTN_CLOSE )
		m_wndMdiTabs.ModifyTabWndStyle( __ETWS_SHOW_BTN_CLOSE, 0, true );
	else
		m_wndMdiTabs.ModifyTabWndStyle( 0, __ETWS_SHOW_BTN_CLOSE, true );
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)
}
void CMainFrame::OnUpdateMditabsShowClose(CCmdUI* pCmdUI) 
{
#if (defined __EXT_MFC_NO_TABMDI_CTRL)
	pCmdUI->Enable( FALSE );
#else
	pCmdUI->SetCheck(
		(m_wndMdiTabs.GetTabWndStyle() & __ETWS_SHOW_BTN_CLOSE) ? TRUE : FALSE
		);
	pCmdUI->Enable( m_wndMdiTabs.GetSafeHwnd() != NULL && m_wndMdiTabs.IsWindowVisible() );
#endif // else from (defined __EXT_MFC_NO_TABMDI_CTRL)
}

void CMainFrame::OnMditabsShowCtxHelp() 
{
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	if( m_wndMdiTabs.GetTabWndStyle() & __ETWS_SHOW_BTN_HELP )
		m_wndMdiTabs.ModifyTabWndStyle( __ETWS_SHOW_BTN_HELP, 0, true );
	else
		m_wndMdiTabs.ModifyTabWndStyle( 0, __ETWS_SHOW_BTN_HELP, true );
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)
}
void CMainFrame::OnUpdateMditabsShowCtxHelp(CCmdUI* pCmdUI) 
{
#if (defined __EXT_MFC_NO_TABMDI_CTRL)
	pCmdUI->Enable( FALSE );
#else
	pCmdUI->SetCheck(
		(m_wndMdiTabs.GetTabWndStyle() & __ETWS_SHOW_BTN_HELP) ? TRUE : FALSE
		);
	pCmdUI->Enable( m_wndMdiTabs.GetSafeHwnd() != NULL && m_wndMdiTabs.IsWindowVisible() );
#endif // else from (defined __EXT_MFC_NO_TABMDI_CTRL)
}

void CMainFrame::OnUistyle2000() 
{
	if( !g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) ) )
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManager )
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
			| RDW_FRAME | RDW_ALLCHILDREN
		);
	CExtControlBar::stat_RedrawFloatingFrames( this );
	CExtControlBar::stat_RecalcBarMetrics( this );
}
void CMainFrame::OnUpdateUistyle2000(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) )
			? FALSE
			: TRUE
		);
}
void CMainFrame::OnUistyleXp() 
{
	if(		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerOffice2003) ) )
		)
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManagerXP )
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
			| RDW_FRAME | RDW_ALLCHILDREN
		);
	CExtControlBar::stat_RedrawFloatingFrames( this );
	CExtControlBar::stat_RecalcBarMetrics( this );
}
void CMainFrame::OnUpdateUistyleXp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		(	g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerOffice2003) ) )
		)	? TRUE
			: FALSE
		);
}
void CMainFrame::OnViewLikeOffice2003() 
{
	if(		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerOffice2003) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerStudio2005) ) )
		)
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManagerOffice2003 )
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
			| RDW_FRAME | RDW_ALLCHILDREN
		);
	CExtControlBar::stat_RedrawFloatingFrames( this );
	CExtControlBar::stat_RecalcBarMetrics( this );
}
void CMainFrame::OnUpdateViewLikeOffice2003(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		(	g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerOffice2003) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerStudio2005) ) )
		)
			? TRUE
			: FALSE
		);
}

void CMainFrame::OnViewLikeStudio2005() 
{
	if( g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerStudio2005) ) )
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManagerStudio2005)
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
			| RDW_FRAME | RDW_ALLCHILDREN
		);
	CExtControlBar::stat_RedrawFloatingFrames( this );
	CExtControlBar::stat_RecalcBarMetrics( this );
}
void CMainFrame::OnUpdateViewLikeStudio2005(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerStudio2005) )
			? TRUE
			: FALSE
		);
}

void CMainFrame::OnContextHelp() 
{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( CExtCustomizeSite::IsCustomizeMode() )
		return;
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	theApp.DoContextHelp();
}

void CMainFrame::OnUpdateContextHelp(CCmdUI* pCmdUI) 
{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( CExtCustomizeSite::IsCustomizeMode() )
	{
		pCmdUI->Enable( FALSE );
		return;
	} // if( CExtCustomizeSite::IsCustomizeMode() )
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	pCmdUI->Enable( TRUE );
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
CExtBarButton * CMainFrame::OnCreateToolbarButton(
	CExtToolControlBar * pBar,
	CExtCustomizeCmdTreeNode * pNodeI,
	CExtCustomizeCmdTreeNode * pNodeC
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
#ifdef _DEBUG
	if( pNodeI != NULL )
	{
		ASSERT_VALID( pNodeI );
		ASSERT( pNodeI->GetCmdID(false) != ID_SEPARATOR );
	}
#endif // _DEBUG
	ASSERT_VALID( pNodeC );
	ASSERT( pNodeC->GetCmdID(false) != ID_SEPARATOR );
CExtBarButton * pTBB = NULL;
	switch( pNodeC->GetCmdID(false) )
	{
		case ID_OBJECT_FILLCOLOR:
		{
			CString sNoFill;
			VERIFY( sNoFill.LoadString( IDS_STRING_NO_COLOR_FILL ) );
			pTBB =
				new CExtBarColorButton(
					pBar,
					ID_OBJECT_FILLCOLOR, 0, COLORREF(-1), RGB(0,0,0),
					ID_OBJECT_FILLCOLOR, true, true,
					(LPCTSTR)sNoFill, NULL,
					CExtBarColorButton::__DIT_CHAR
					);
			ASSERT_VALID( pTBB );
			//pTBB->SetSeparatedDropDown();
			break;
		} // case ID_OBJECT_FILLCOLOR:
		case ID_OBJECT_LINECOLOR:
		{
			CString sNoOutline;
			VERIFY( sNoOutline.LoadString( IDS_STRING_NO_COLOR_OUTLINE ) );
			pTBB =
				new CExtBarColorButton(
					pBar,
					ID_OBJECT_LINECOLOR, 0, COLORREF(-1), RGB(0,0,0),
					ID_OBJECT_LINECOLOR, true, true,
					(LPCTSTR)sNoOutline, NULL,
					CExtBarColorButton::__DIT_FRAME
					);
			ASSERT_VALID( pTBB );
			break;
		} // ID_OBJECT_LINECOLOR
	} // switch( pNodeC->GetCmdID(false) )
	if( pTBB != NULL )
	{
		ASSERT_VALID( pTBB );
		if( pNodeI != NULL )
		{
			pTBB->SetBasicCmdNode( pNodeI );
			pTBB->OnCustomizeUpdateProps( pNodeI );
		} // if( pNodeI != NULL )
		else
			pTBB->OnCustomizeUpdateProps( pNodeC );
		pTBB->SetCustomizedCmdNode( pNodeC );
		return pTBB;
	} // if( pTBB != NULL )
	return CExtCustomizeSite::OnCreateToolbarButton( pBar, pNodeI, pNodeC );
}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

void CMainFrame::OnCustomize() 
{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	CExtCustomizeSite::DoFormCustomization();
#endif
}
void CMainFrame::OnUpdateCustomize(CCmdUI* pCmdUI) 
{
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	pCmdUI->Enable( TRUE );
#else
	pCmdUI->Enable( FALSE );
#endif
}

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
HICON CMainFrame::OnColorItemGenerateIcon(
	CExtBarColorButton * pTBB,
	CExtCustomizeCmdTreeNode * pNode,
	COLORREF clr
	)
{
	ASSERT_VALID( pNode );
	pTBB;
UINT nCmdID = pNode->GetCmdID(false);
	if( nCmdID == ID_OBJECT_FILLCOLOR )
	{
		LOGFONT lf;
		::memset( &lf, 0 , sizeof(LOGFONT) );
		g_PaintManager->m_FontBold.GetLogFont( &lf );
		_tcscpy( lf.lfFaceName, _T("Times New Roman") );
		lf.lfHeight = ::MulDiv( lf.lfHeight, 3, 2 );
		lf.lfWeight = 900;
		CFont fontForLetter;
		VERIFY(
			fontForLetter.CreateFontIndirect( &lf )
			);
		ASSERT( fontForLetter.GetSafeHandle() != NULL );
		HICON hIcon =
			CExtPaintManager::stat_GenerateColorIconChar(
				clr,
				(HFONT)fontForLetter.GetSafeHandle()
				);
		ASSERT( hIcon != NULL );
		return hIcon;
	} // if( nCmdID == ID_OBJECT_FILLCOLOR )
	if( nCmdID == ID_OBJECT_LINECOLOR )
	{
		HICON hIcon =
			CExtPaintManager::stat_GenerateColorIconFrame( clr );
		ASSERT( hIcon != NULL );
		return hIcon;
	} // if( nCmdID == ID_OBJECT_LINECOLOR )
	return CExtCustomizeSite::OnColorItemGenerateIcon( pTBB, pNode, clr );
}

bool CMainFrame::OnColorItemValueGet(
	CExtBarColorButton * pTBB,
	CExtCustomizeCmdTreeNode * pNode,
	COLORREF & clr,
	bool bSelected
	)
{
	ASSERT_VALID( pNode );
	pTBB;
UINT nCmdID = pNode->GetCmdID(false);
	if( nCmdID == ID_OBJECT_FILLCOLOR )
	{
		CFrameWnd * pFrame = GetActiveFrame();
		if( pFrame == NULL )
			return false;
		CDrawView * pView = (CDrawView *)pFrame->GetActiveView();
		if( pView == NULL )
			return false;
		POSITION pos = pView->m_selection.GetHeadPosition();
		if( pos == NULL )
			return false;
		CDrawObj * pObj = pView->m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
		if( bSelected )
			clr = pObj->IsNoFill() ? COLORREF(-1) : pObj->GetFillColor();
		else
			clr = COLORREF(-1);
		return true;
	} // if( nCmdID == ID_OBJECT_FILLCOLOR )
	if( nCmdID == ID_OBJECT_LINECOLOR )
	{
		CFrameWnd * pFrame = GetActiveFrame();
		if( pFrame == NULL )
			return false;
		CDrawView * pView = (CDrawView *)pFrame->GetActiveView();
		if( pView == NULL )
			return false;
		POSITION pos = pView->m_selection.GetHeadPosition();
		if( pos == NULL )
			return false;
		CDrawObj * pObj = pView->m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
		if( bSelected )
			clr = pObj->IsNoOutline() ? COLORREF(-1) : pObj->GetLineColor();
		else
			clr = COLORREF(-1);
		return true;
	} // if( nCmdID == ID_OBJECT_LINECOLOR )
	return CExtCustomizeSite::OnColorItemValueGet( pTBB, pNode, clr, bSelected );
}
bool CMainFrame::OnColorItemValueSet(
	CExtBarColorButton * pTBB,
	CExtCustomizeCmdTreeNode * pNode,
	COLORREF & clr,
	bool bSelected
	)
{
	ASSERT_VALID( pNode );
	pTBB;
	bSelected;
UINT nCmdID = pNode->GetCmdID(false);
CFrameWnd * pFrame = GetActiveFrame();
	if( pFrame == NULL )
		return false;
CDrawView * pView = (CDrawView *)pFrame->GetActiveView();
	if( pView == NULL )
		return false;
POSITION pos = pView->m_selection.GetHeadPosition();
	if( pos == NULL )
		return false;
	while( pos != NULL )
	{
		CDrawObj * pObj = pView->m_selection.GetNext( pos );
		ASSERT( pObj != NULL );
		switch( nCmdID )
		{
		case ID_OBJECT_FILLCOLOR:
			if( clr == COLORREF(-1) )
				pObj->SetNoFill();
			else
			{
				pObj->SetNoFill( FALSE );
				pObj->SetFillColor( clr );
			}
		break;
		case ID_OBJECT_LINECOLOR:
			if( clr == COLORREF(-1) )
				pObj->SetNoOutline();
			else
			{
				pObj->SetNoOutline( FALSE );
				pObj->SetLineColor( clr );
			}
		break;
		} // pView->
	}
	pView->Invalidate();
	pView->UpdateWindow();
	CExtCustomizeSite::RedrawCommandItems( nCmdID );
	return true;
}

#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( nCode == CN_COMMAND )
	{
#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
#ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	
		if( nID == ID_WINDOWS_LIST )
		{
			CMyMdiWindowsListDlg dlg( this );
			dlg.DoModal();
		}
#endif // #ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	
#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)
	}


	if( ID_MYEXTBTN_SLIDER == nID )
	{
		if( nCode == CN_UPDATE_COMMAND_UI )
		{


			CWinApp * pApp = ::AfxGetApp();
			ASSERT( pApp != NULL );
			CExtCmdItem * pCmdItemSlider =
				g_CmdManager->CmdGetPtr(
				pApp->m_pszProfileName,
				ID_MYEXTBTN_SLIDER
				);
			ASSERT( pCmdItemSlider != NULL );
			/*CZoomBarSliderButton * pZoomSliderTBB = (CZoomBarSliderButton *)
				STATIC_DOWNCAST(
						CExtBarSliderButton,
						m_wndToolBarZoom.GetButton(
							m_wndToolBarZoom.CommandToIndex(ID_MYEXTBTN_SLIDER)
							)
					);*/
			//ULONG nPos = pZoomSliderTBB->ScrollPosGet();
		
			CCmdUI * pCmdUI = (CCmdUI*)pExtra;
			ASSERT( pCmdUI != NULL );
			pCmdUI->Enable(	MDIGetActive() != NULL ); 
			return TRUE;
		}
	} // if( ID_MYEXTBTN_SLIDER == nID )



	return CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

