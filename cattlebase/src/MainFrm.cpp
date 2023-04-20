#include "stdafx.h"
#include "MainFrm.h"
#include "cattlebase.h"
#include <psapi.h>

#include "Doc/InDoc/NewInDocDlg.h"
#include "dlgs/NewHentDlg.h"
#include "dlgs/NewExpenseDlg.h"

#include "Printing/ui/PrintDlg.h"
#include "dlgs\options.h"
#include "dlgs\Stocks.h"

#include "dlgs\statdlg.h"
#include "dlgs\logindlg.h"
#include "dlgs\rulesview.h"
#include "dlgs\ExpenseCategoriesDlg.h"
#include "dlgs\Agents.h"

#include "sync\SyncDlg.h"

#include "MainCfg.h"

#include "grids\BaseDataGrid.h"


#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "missing/paths.h"
#include "dlgs/SubDirSelectDlg.h"
#include "classes/BrowseForDir.h"
#include "missing/util.h"
#include "gridtabs.h"
#include "mainfrm_ctrl.h"
#include "log.h"

#include "tasktranslators/LoadTaskTranslator.h"


#include "classes/forallactions/FastSelectAction.h"
#include "classes/match/HentMatcher.h"
#include "classes/match/InvMatcher.h"
#include "classes/match/CowMatcher.h"
#include "classes/match/DocMatcher.h"
#include "classes/match/ExpenseMatcher.h"


#include "datatypes/RRInvoice.h"
#include "grids/CowGrid.h"
#include "grids/HentsGrid.h"


#include "ctrls\EANEdit.h"


#include "dlgs\NewOutDocDlg.h"
#include "exceptions/StrException.h"
#include "classes/forallactions/CountAction.h"
//#include "hkey/hkey.h"
#include "taskprogress/TaskProgressDlg.h"

#include "barcodescanner/BCScannerDrv.h"
#include "classes/TempStringLoader.h"
#include "hkey/ui/KeyStateDlg.h"
#include "exceptions/SQLException.h"
#include "repo/ui/RepoAccountsSyncCtrlDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)

	ON_WM_CREATE()
	ON_WM_SETFOCUS()
//	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()

	ON_COMMAND_EX(IDR_TOOLBAR_BIG_TRUE_COLOR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_BIG_TRUE_COLOR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_MAINFRAME, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_MAINFRAME, OnUpdateControlBarMenu)
	
	ON_COMMAND_EX(IDR_FASTSELECT_TOOLBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_FASTSELECT_TOOLBAR,OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR_GRIDCTRL, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_GRIDCTRL,OnUpdateControlBarMenu)


	ON_COMMAND( IDM_TOOLBAR_NEWINDOC , OnNewInDoc )
	ON_COMMAND( IDM_TOOLBAR_NEWOUTDOC, OnNewOutDoc )
	ON_COMMAND( IDM_TOOLBAR_NEWHENT, OnNewHent )
	ON_COMMAND( IDM_TOOLBAR_FIND, OnFind )
	
	ON_COMMAND( IDM_TOOLBAR_SHOWALL, OnUnmarkAll )
	ON_COMMAND( IDM_TOOLBAR_OPTIONS, OnOptions )
	ON_COMMAND( IDM_TOOLBAR_EXIT, OnExit )
	ON_COMMAND( IDM_TOOLBAR_TERMSYNC,OnTermSync)
	ON_COMMAND( IDM_TOOLBAR_NEWEXPENSE, OnExpense )
	ON_COMMAND( IDM_TOOLBAR_HENTSYNC, OnHentSync )

	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_NEWINDOC , OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_NEWOUTDOC, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_NEWHENT, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_FIND, OnUpdateAnyCommand )
	
	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_SHOWALL, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_OPTIONS, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_EXIT, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_TERMSYNC,OnUpdateAnyCommand)
	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_NEWEXPENSE, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_HENTSYNC, OnUpdateAnyCommand )

	ON_COMMAND( IDMM_STOCKEDIT, OnStockEdit )
	ON_COMMAND( IDMM_DAYSTATS, OnStats )
	ON_COMMAND( IDMM_RULEEDIT, OnRuleEdit)
	ON_COMMAND (IDMM_LOGOUT, OnLogout)
	ON_COMMAND( IDMM_EDITEXPENSECATEGORIES, OnEditExpenseCategory)
	ON_COMMAND( IDMM_EDITAGENTS, OnEditAgents)

	ON_UPDATE_COMMAND_UI( IDMM_STOCKEDIT, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( IDMM_DAYSTATS, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( IDMM_EDITEXPENSECATEGORIES, OnUpdateAnyCommand)
	ON_UPDATE_COMMAND_UI( IDMM_EDITAGENTS, OnUpdateAnyCommand)
	


	ON_COMMAND_EX(IDA_INSERTNEW,OnInsertNew)
	ON_COMMAND_EX(IDA_INSERTNEW2,OnInsertNew)

	ON_COMMAND(IDA_MARKROW,OnMarkRow)
	ON_COMMAND(IDA_UNMARKROW,OnUnMarkRow)
	ON_COMMAND(IDA_DELROW,OnDelRow)
	
	ON_COMMAND(IDM_TOOLBAR_FASTSELECT_BTN_GO,OnFastSelect)
	ON_NOTIFY(EN_EDITCOMPLETE,IDC_FASTSELECT_EDIT,OnFastSelReturn)
	ON_UPDATE_COMMAND_UI( IDC_FASTSELECT_EDIT, OnUpdateFastSelEdit )
	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_FASTSELECT_BTN_GO, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( IDM_TOOLBAR_FASTSELECT_BTN_OPT, OnUpdateAnyCommand )

	ON_UPDATE_COMMAND_UI_RANGE(IDM_FASTSELECT_MENU_MIN,IDM_FASTSELECT_MENU_MAX,OnFastSelectOptUpdateCommand)
	ON_COMMAND_RANGE(IDM_FASTSELECT_MENU_MIN,IDM_FASTSELECT_MENU_MAX,OnFastSelectOptCommand)

	ON_UPDATE_COMMAND_UI(IDM_TOOLBAR_GRIDFONTZOOMOUT, OnUpdateGridFontZoomOut )
	ON_UPDATE_COMMAND_UI(IDM_TOOLBAR_GRIDFONTZOOMIN, OnUpdateGridFontZoomIn )
	ON_UPDATE_COMMAND_UI(IDM_TOOLBAR_GRIDGOTOBOTTOM, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI(IDM_TOOLBAR_GRIDGOTOTOP, OnUpdateAnyCommand )

	ON_COMMAND(IDM_TOOLBAR_GRIDFONTZOOMOUT, OnGridFontZoomOut )
	ON_COMMAND(IDM_TOOLBAR_GRIDFONTZOOMIN, OnGridFontZoomIn )
	ON_COMMAND(IDM_TOOLBAR_GRIDGOTOBOTTOM, OnGridGoToBottom )
	ON_COMMAND(IDM_TOOLBAR_GRIDGOTOTOP, OnGridGoToTop )
	
	
	ON_UPDATE_COMMAND_UI(IDC_INDICATOR_STATISTICS,OnUpdateStats)
	ON_UPDATE_COMMAND_UI(IDC_INDICATOR_NOPASSIMGDB,OnUpdatePassImgDBStat)
	ON_UPDATE_COMMAND_UI(IDC_INDICATOR_BARCODESCANNER,OnUpdateBarcodeScannerStatus)


	ON_COMMAND_RANGE(IDM_GRID_COMMANDS_MIN,IDM_GRID_COMMANDS_MAX,OnGridMenuCommand)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_GRID_COMMANDS_MIN,IDM_GRID_COMMANDS_MAX,OnGridMenuUpdateCommand)
	

	
	
	ON_REGISTERED_MESSAGE(CExtPopupMenuWnd::g_nMsgPopupDrawLeftArea,OnDrawPopupLeftArea)
	
		
	ON_MESSAGE(WM_UPDATEGRID,OnUpdateGrid)
	ON_MESSAGE(WM_GETSELROWS,OnGetSelRows)
	ON_MESSAGE(WM_DOARCHIVE,OnDoArchive)
	ON_MESSAGE(WM_RESTORECOPY,OnRestoreBackup)
	ON_MESSAGE(WM_SETACTIVEPANE,OnSetActivePane)
	ON_MESSAGE(WM_LOCKGRID,OnLockGrid)
	


	ON_UPDATE_COMMAND_UI_RANGE(ID_PRINTDOC_FIRST,ID_PRINTDOC_LAST,OnUpdatePrintDoc)
	ON_COMMAND_RANGE(ID_PRINTDOC_FIRST,ID_PRINTDOC_LAST,OnPrintDocs)


	
	ON_COMMAND_EX(IDMM_LOAD_BACKUP,OnLoadArchiveBackupDB)
	ON_COMMAND_EX(IDMM_LOAD_ARCHIVE,OnLoadArchiveBackupDB)
	ON_COMMAND(IDMM_LOAD_PRIMARYBASE,OnLoadPrimaryDB)
	ON_COMMAND(IDMM_LOAD_CUSTOMPATH,OnLoadCustomDirDB)
	




	


END_MESSAGE_MAP()

static UINT indicators[] =
{	
	ID_SEPARATOR,
	IDS_HKEY_NOTPRESENT_PROGRESS,
	//ID_SEPARATOR,
	IDC_INDICATOR_BARCODESCANNER,
	IDC_INDICATOR_NOPASSIMGDB,
	IDC_INDICATOR_STATISTICS,	
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction


CMainFrame::CMainFrame() 
{
	g_PaintManager.InstallPaintManager( new (CExtPaintManagerOffice2010_R1) );
	CExtPopupMenuWnd::g_bMenuExpanding = false;
	CExtPopupMenuWnd::g_bMenuHighlightRarely = false;
	m_FastSelMarkRow = AppCfg().GetBool(CFG_FASTSELMARKROW);
	m_FastSelSelRow = AppCfg().GetBool(CFG_FASTSELSELROW);
	m_FastSelFindFirst = AppCfg().GetBool(CFG_FASTSELFINDFIRST);
	m_FastSelPosOnRow = AppCfg().GetBool(CFG_FASTSELPOSONROW);	


	HINSTANCE hInstResource = AfxGetResourceHandle();
	ASSERT( hInstResource != NULL );
	if( hInstResource != NULL )
	{	
		m_hIconGood = (HICON)::LoadImage(hInstResource,MAKEINTRESOURCE( IDI_GOOD ),IMAGE_ICON,16,16,0);	
		m_hIconBad = (HICON)::LoadImage(hInstResource,MAKEINTRESOURCE( IDI_BAD ),IMAGE_ICON,16,16,0);	
		m_hIconNoBarcode = (HICON)::LoadImage(hInstResource,MAKEINTRESOURCE( IDI_NOBARCODE ),IMAGE_ICON,16,16,0);	
		m_hIconYesBarcode = (HICON)::LoadImage(hInstResource,MAKEINTRESOURCE( IDI_YESBARCODE ),IMAGE_ICON,16,16,0);		
		ASSERT( m_hIconGood != NULL && m_hIconBad != NULL && m_hIconNoBarcode != NULL && m_hIconYesBarcode != NULL );
	}
	m_bShowingKeyState = false;	
}

CMainFrame::~CMainFrame()
{
}

static UINT g_statBasicCommands[] =
{
	ID_APP_ABOUT,
	IDM_TOOLBAR_EXIT,
	IDM_TOOLBAR_NEWINDOC,
	IDM_TOOLBAR_NEWOUTDOC,
	IDM_TOOLBAR_NEWHENT,
	IDM_TOOLBAR_NEWEXPENSE,
	IDM_TOOLBAR_FIND,
	IDM_TOOLBAR_SHOWALL,
	IDM_TOOLBAR_OPTIONS,
	IDMM_STOCKEDIT,	
	0, // end of list
};

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CDateTime lastcopy;
	lastcopy.ParseDate(CDateTime::YYYYMMDD,(LPCSTR)AppCfg().GetString(CFGID_LASTCOPY));
	bool makeBackup = false;
	if(lastcopy.IsNull())
		makeBackup = true;
	else
	{		
		if(lastcopy.GetDiffrenceInSeconds(CDateTime::now()) > AppCfg().GetNum(CFGID_MAKECOPY)*24*60*60)
			makeBackup = true;
	}

	if(makeBackup)
		CBackupDlg::MakeBackupCopy(this,AppCfg().GetString(CFGID_BACKUPDIR));


	ULONGLONG passDBFileSize = CPassImgBase::GetInstance()->GetPassDbFileSize();
	ULONGLONG maxPassDBFileSize = (99*(AppCfg().GetNum(CFG_MAXPASS_SIZE_MB))/100)*1024*1024; //allow 1% size compare margin

	if(passDBFileSize >= maxPassDBFileSize)
	{
		do
		{
			if(AppCfg().GetBool(CFG_ASKBEFOREPASSMOVE))
			{
				if(AfxMessageBox(IDS_ASKBEFOREPASSBDFILEMOVE,MB_YESNO) == IDNO)
					break;		
			}			
			CPassDBMngmntOpt::MovePassDBFile(AppCfg().GetString(CFG_PASSMOVEDIRECTORY));
			AfxMessageBox(IDS_PASSDBFILEWASMOVED);
		}
		while(FALSE);
	}
	
//	m_CowAmount = ary_cows.GetSize();

	CWinApp * pApp = ::AfxGetApp();
		ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
	



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
		g_CmdManager->UpdateFromToolBar(
			pApp->m_pszProfileName,
			IDR_TOOLBAR_BIG_TRUE_COLOR
			)
		);

		VERIFY(
		g_CmdManager->UpdateFromToolBar(
			pApp->m_pszProfileName,
			IDR_FASTSELECT_TOOLBAR
			)
		);

		VERIFY(
		g_CmdManager->UpdateFromToolBar(
			pApp->m_pszProfileName,
			IDR_TOOLBAR_GRIDCTRL
			)
		);

	VERIFY(
	g_CmdManager->SetBasicCommands(
		pApp->m_pszProfileName,
		g_statBasicCommands
		)
	);

	/*CExtCmdItem * pCmdItem;
	pCmdItem = g_CmdManager->CmdAllocPtr( pApp->m_pszProfileName, IDR_FASTSELECT_TOOLBAR );
	ASSERT( pCmdItem != NULL );

	CString str;
	str.LoadString(IDS_FASTSELECT);
	pCmdItem->m_sTipTool = pCmdItem->m_sTipStatus = str;

	*/	
		if( !m_wndMenuBar.Create(
			NULL, // _T("Menu Bar"),
			this,
			IDR_MAINFRAME
			)
		)
    {
        LOG("Failed to create menubar");
        return -1;      
    }
	if((! m_wndToolBar.Create(	NULL, this, IDR_TOOLBAR_BIG_TRUE_COLOR) )
		||	(! m_wndToolBar.LoadToolBar(IDR_TOOLBAR_BIG_TRUE_COLOR) )
		)
	{
		LOG("Failed to create true-color toolbar");
		return -1;      
	}
	
	if(!SetupStatusBar())
		return -1;
	
	if(!SetupFastSelectToolBar())
		return -1;
	if(!SetupGridCtrlToolBar())
		return -1;

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
	//m_wndToolBarFastSelect.EnableDocking(CBRS_ALIGN_ANY);
		
	if(	!CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName)
		)
	{
		DockControlBar( &m_wndToolBarFastSelect );		
		DockControlBar( &m_wndToolBarGridCtrl );
	}

	CBarcodeScannerDriver::ConnectToScanner();

	return 0;
}

bool CMainFrame::SetupFastSelectToolBar()
{
	if((! m_wndToolBarFastSelect.Create(	NULL, this, IDR_FASTSELECT_TOOLBAR) )
		||	(! m_wndToolBarFastSelect.LoadToolBar(IDR_FASTSELECT_TOOLBAR) )
		)
	{
		LOG("Failed to create fast select toolbar");
		return false;      
	}
		
	 if( !m_fastSelect.CreateEx(
			WS_EX_CLIENTEDGE, 
			_T("EDIT"), 
			NULL,
            WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|WS_TABSTOP|ES_LEFT,
            CRect(0,0,100,21),
            &m_wndToolBarFastSelect,
            IDC_FASTSELECT_EDIT))
    {
        LOG("Failed to create ID_BAR_FAST_SELECT");
        return false;      
    }

	VERIFY(	m_wndToolBarFastSelect.InsertButton(0,IDC_FASTSELECT_EDIT) );

    m_wndToolBarFastSelect.SetButtonCtrl( 
        m_wndToolBarFastSelect.CommandToIndex(IDC_FASTSELECT_EDIT),
        &m_fastSelect
    );

    m_fastSelect.SetFont(  
        CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)) );
    
	CMenu menuFastSelect;
	menuFastSelect.LoadMenu(IDR_FASTSELECT_MENU);
	
	
	VERIFY(
		m_wndToolBarFastSelect.SetButtonMenu(
		m_wndToolBarFastSelect.CommandToIndex(IDM_TOOLBAR_FASTSELECT_BTN_OPT),
		 menuFastSelect.Detach(),
		TRUE,
		TRUE,
		FALSE
		)
		);

	m_wndToolBarFastSelect.EnableDocking(CBRS_ALIGN_ANY);
	return true;
	
}

bool CMainFrame::SetupGridCtrlToolBar()
{
	if((! m_wndToolBarGridCtrl.Create(	NULL, this, IDR_TOOLBAR_GRIDCTRL) )
		||	(! m_wndToolBarGridCtrl.LoadToolBar(IDR_TOOLBAR_GRIDCTRL) )
		)
	{
		LOG("Failed to create grid control toolbar");
		return false;      
	}
	m_wndToolBarGridCtrl.EnableDocking(CBRS_ALIGN_ANY);
	return true;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.x = AppCfg().GetNum(CFGID_INITWINDOWPOS_XPOS);
	cs.y = AppCfg().GetNum(CFGID_INITWINDOWPOS_YPOS);
	cs.cx = AppCfg().GetNum(CFGID_INITWINDOWPOS_WIDTH);
	cs.cy = AppCfg().GetNum(CFGID_INITWINDOWPOS_HEIGHT);

	return TRUE;
}


void CMainFrame::OnClose() 
// Save current position and state to the .CFG file before the main fram is closed.
// The app will open upp with this position and state the next time it is run.
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	CRect r(wp.rcNormalPosition);
	AppCfg().SetNum(CFGID_INITWINDOWPOS_XPOS,r.left);
	AppCfg().SetNum(CFGID_INITWINDOWPOS_YPOS,r.top);
	AppCfg().SetNum(CFGID_INITWINDOWPOS_WIDTH,r.Width());
	AppCfg().SetNum(CFGID_INITWINDOWPOS_HEIGHT,r.Height());
//	AppCfg().SetNum(CFGID_WINDOW_STYLE,(AfxGetMainWnd()->IsZoomed())?1:0);


	AppCfg().SetBool(CFG_FASTSELMARKROW,m_FastSelMarkRow);
	AppCfg().SetBool(CFG_FASTSELSELROW,m_FastSelSelRow);
	AppCfg().SetBool(CFG_FASTSELFINDFIRST,m_FastSelFindFirst);
	AppCfg().SetBool(CFG_FASTSELPOSONROW,m_FastSelPosOnRow);


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

	

	CFrameWnd::OnClose();
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	
	TVisualObject *pTab = new TVisualObject(1,"",pContext,RUNTIME_CLASS(TTabWnd));		

	
	TVisualObject *pTabView = NULL,*pCurrTabView = NULL;
	CString gridName;

	m_Framework.Add(pTab);

	GridTabs* tabs = GridTabs::GetTabs();
	for(int g = 0;g < tabs->GetSize();g++)
	{
		
		CGridTab *tab = tabs->GetAt(g);

		gridName.LoadString(tab->GetResGridName());
		pTabView = new TVisualObject(tab->GetGridId(),(LPCSTR)gridName,pContext,tab->GetGridClass());
		pTabView->SetHotKey(tab->GetHotKey());
		m_Framework.Add(pTab,pTabView);
		if(tab->GetActiveOnStartup()) pCurrTabView = pTabView;
	

	}
	  
	m_Framework.Create(this);
	m_Framework.SetActivePane(pCurrTabView);

	CMarkWarnShowOptions::GetInstance()->Load();
	EnableUserTabs();		

	
	 
	return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::EnableUserTabs()
{

	GridTabs* tabs = GridTabs::GetTabs();
	TVisualObject *pObject = NULL;	

	for(int g = 0;g < tabs->GetSize();g++)
	{
			
		CGridTab *tab = tabs->GetAt(g);
		pObject = m_Framework.Get(tab->GetGridId());

		if(pObject) 
		{
			m_Framework.EnableTab(pObject,
				(tab->GetGridPrivledges() == ((UserData::PRIVLEDGES)(-1)) || 
				UserData::HasCurUserPriv(tab->GetGridPrivledges()))	
				);
		
		}

	}
}


void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	m_Framework.Destroy();
	SQLDB.CloseCattleBase();
	FlushArrays();
	CBarcodeScannerDriver::DisconnectScanner();
//	m_pKeyDetailedInformationSubmitThread->Stop();
	
	//m_pKeyExpityCheckLogThread->Delete();

}


void CMainFrame::OnAnyCommand()
{
	
}

void CMainFrame::OnUpdateAnyCommand( CCmdUI* pCmdUI )
{
#ifdef SPECIAL_TRIAL_RELEASE
	if(pCmdUI->m_nID == IDM_TOOLBAR_NEWINDOC && ary_cows.GetCount() > 5)
		pCmdUI->Enable( FALSE );
#else
	pCmdUI->Enable( TRUE );
#endif
}




void CMainFrame::OnUpdateFastSelEdit( CCmdUI* pCmdUI )
{
	switch(GetCurrGridID())
	{
		case RRINVSGRID:
		case BUYINVSGRID:
		case SELLINVSGRID:
		case OUTDOCSGRID:
		case INDOCSGRID:
			m_fastSelect.SetContentToNumber();
			break;	
		case COMPANYHENTGRID:
		case FARMERHENTGRID:
		case HENTSGRID:
		case INCOWSGRID:
		case ALLCOWSGRID:
			m_fastSelect.SetContentToEAN();		
			break;
		case EXPENSESGRID:
			m_fastSelect.SetContentToText();
			break;
	}		

	pCmdUI->Enable( TRUE );
}



/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG




void CMainFrame::OnNewInDoc ()
{
	if(HAS_PRIVLEDGE(ADDMODOWN))
	{
		CNewInDocDlg newInDoc;
		newInDoc.DoModal();
	} 
	else AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(m_Framework.ProcessMessage(pMsg)) return TRUE;
	return CFrameWnd::PreTranslateMessage(pMsg);
}



void CMainFrame::OnNewHent ()
{
	
	if(HAS_PRIVLEDGE(ADDMODOWN))
	{			
		CNewHentDlg newHentDlg;
		newHentDlg.DoModal();
	} 
	else AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
}

LRESULT CMainFrame::OnUpdateGrid(WPARAM wParam,LPARAM lParam)
{
	int grid = (int)wParam;
	BaseDataGrid *gv = GetGridView(grid);
	VERIFY(gv);		
	gv->UpdateItemCount();
	gv->Unlock();
	gv->Invalidate(FALSE); //force GetData
	return (LRESULT)0;
}


void CMainFrame::OnNewOutDoc()
{
	if(HAS_PRIVLEDGE(ADDMODOWN))
	{
		CNewOutDocDlg newOutDocDlg;
		newOutDocDlg.DoModal();
	} 
	else AfxMessageBox(IDS_NOPRIVS);
}


void CMainFrame::OnFind()
{
	CFindDlg finddlg;
	finddlg.DoModal();

}


LRESULT CMainFrame::OnGetSelRows(WPARAM wParam,LPARAM lParam)
{

	VERIFY(wParam);
	BaseDataGrid *gv = GetGridView(lParam);
	VERIFY(gv);
	CRawDataArray *rsel = (CRawDataArray*)wParam;
	gv->GetSelectedRows(*rsel);	
	return (LRESULT)0;

}

BaseDataGrid * CMainFrame::GetGridView(int nId)
{

	BaseDataGrid *lv = NULL;
	TVisualFrameworkIterator it(m_Framework);

	while (!it.End()) 
	{
		TVisualObject *pObject = it.Get();
		if(it->GetID() == nId) 
		{
			lv = (BaseDataGrid*)it->GetWnd();	
			break;
		}
		it++;
	}

	return lv;
	
}





LRESULT CMainFrame::OnDrawPopupLeftArea(WPARAM wParam, LPARAM lParam)
{
	wParam;
	CExtPopupMenuWnd::DRAWLEFTAREADATA * pDrawLeftAreaData =
		reinterpret_cast < CExtPopupMenuWnd::DRAWLEFTAREADATA * > ( lParam );
	ASSERT( pDrawLeftAreaData != NULL );

	// get draw DC
	CDC & dc = *( (CDC *) *pDrawLeftAreaData );

	CRect rcItem = LPCRECT(*pDrawLeftAreaData);
	CExtPaintManager::stat_PaintGradientRect(
		dc,
		&rcItem,
		dc.GetNearestColor( RGB( 0, 0, 0 ) ),
		dc.GetNearestColor( RGB( 100, 255, 100 ) ),
		true
		);

	LOGFONT lf;
	::memset(&lf,0,sizeof(LOGFONT));
	g_PaintManager->m_FontNormalVertX.GetLogFont( &lf );
	lf.lfHeight = -12;
	lf.lfWidth = 0;
	lf.lfWeight = 900;
	lf.lfCharSet = DEFAULT_CHARSET;
	_tcscpy( lf.lfFaceName, _T("Tahoma") );
	CFont font;
	VERIFY(
		font.CreateFontIndirect(&lf)
		);
	CFont * pOldFont = dc.SelectObject( &font );
	INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
	COLORREF clrOldText = dc.SetTextColor( RGB( 0, 0, 0 ) );


	TVisualObject *pObject = m_Framework.GetActivePane();
	CString sTitle = pObject->GetTitle();


	CPoint ptText( rcItem.left+4, rcItem.bottom-5 );
	dc.DrawState(
		ptText, rcItem.Size(), (LPCTSTR)sTitle,
		DSS_NORMAL,  TRUE, 0, (CBrush*)NULL
		);
	dc.SetTextColor( RGB( 255, 255, 255 ) );
	ptText.Offset( -1, -2 );
	dc.DrawState(
		ptText, rcItem.Size(), (LPCTSTR)sTitle,
		DSS_NORMAL,  TRUE, 0, (CBrush*)NULL
		);

	
	dc.SetBkMode( nOldBkMode );
	dc.SetTextColor( clrOldText );
	dc.SelectObject( pOldFont );

	return !0;
}

void CMainFrame::OnGridMenuUpdateCommand(CCmdUI *pCmdUI)
{
	BaseDataGrid *pGrid = GetCurrGrid();
	if(!pGrid->HandleUpdateMessage(pCmdUI))
	{
		pCmdUI->Enable(TRUE);
	}
}
void CMainFrame::OnGridMenuCommand(UINT id)
{
	BaseDataGrid *pGrid = GetCurrGrid();
	if(!pGrid->HandleMessage(id))
	{
		int nElement = pGrid->GetCurSel();
		if(nElement >= 0)
		{
			if(id == IDM_GRID_DELETEROW)
			{
				pGrid->Lock();
				switch(GetCurrGridID())
				{
					case INCOWSGRID:
						LockGrid(ALLCOWSGRID);
						TrashCow(ary_incows,ary_cows,nElement);
						UpdateGrid(ALLCOWSGRID);
						break;
					case ALLCOWSGRID:
						LockGrid(INCOWSGRID);
						TrashCow(ary_cows,ary_incows,nElement);
						UpdateGrid(INCOWSGRID);
						break;
					case INDOCSGRID:
						LockGrid(ALLCOWSGRID);
						LockGrid(INCOWSGRID);
						TrashInDoc(nElement);
						UpdateGrid(ALLCOWSGRID);
						UpdateGrid(INCOWSGRID);
						break;
					case OUTDOCSGRID:
						LockGrid(INCOWSGRID);
						TrashOutDoc(nElement) ;
						UpdateGrid(INCOWSGRID);
						break;
					case BUYINVSGRID:
						TrashInvoice(ary_buy_invs,nElement);
						break;
					case SELLINVSGRID:
						TrashInvoice(ary_sell_invs,nElement);
						break;
					case RRINVSGRID:
						TrashInvoice(ary_rr_invs,nElement);
						break;

					case HENTSGRID:
			
						switch(ary_hents[nElement]->GetHentType()->henttypeid)
						{

							case HTYPE_COMPANY:
								LockGrid(COMPANYHENTGRID);
								TrashHent(ary_hents,ary_companies,nElement);
								UpdateGrid(COMPANYHENTGRID);
							break;
							case HTYPE_INDIVIDUAL:
								LockGrid(FARMERHENTGRID);
								TrashHent(ary_hents,ary_farmers,nElement);
								UpdateGrid(FARMERHENTGRID);
							break;

						}				
						break;
					case FARMERHENTGRID:
						LockGrid(HENTSGRID);
						TrashHent(ary_hents,ary_farmers,nElement);
						UpdateGrid(HENTSGRID);				
						break;
					case COMPANYHENTGRID:
						LockGrid(HENTSGRID);
						TrashHent(ary_hents,ary_companies,nElement);
						UpdateGrid(HENTSGRID);		
						break;
					case EXPENSESGRID:
						LockGrid(EXPENSESGRID);
						TrashExpense(ary_expenses,nElement);
						UpdateGrid(EXPENSESGRID);
						break;
				}
				pGrid->UpdateItemCount();
				pGrid->Unlock();
			}
		}
	}

	
	

}

void CMainFrame::TrashExpense(CExpenseDataArray &expenses,int nElement)
{
	Expense *pExpense = expenses[nElement];
	if(pExpense->DeleteThisExpense())
	{
		expenses.RemoveAt(nElement);
		delete pExpense;		
	}
	else
	{
		SQLDB.SQLMessageBox(IDS_ERRDELETINGEXPENSE);
	}

}


void CMainFrame::TrashCow(CCowArray &mainArray,CCowArray &subArray,int nElement)
{
	Cow *cowd = mainArray[nElement];
	if(TryTrashCow(cowd))
	{
		CPassImgBase::GetInstance()->DeletePassImg(cowd);
		cowd->DeleteThisCow();
		cowd->GetInDoc()->GetCows()->RemovePtr(cowd);

		mainArray.RemoveAt(nElement);
		subArray.RemovePtr(cowd);
	
		UpdateGrid(ALLCOWSGRID);
		UpdateGrid(INCOWSGRID);
		delete cowd;
						 
	}
}

void CMainFrame::OnExpense()
{
//	if(!CHardwareKey::GetHKey()->IsFeatureEnabled(FEATURE_COLLECTOR))
//	{
//		AfxMessageBox(IDS_FEATUREDISABLED);
//		return;
//	}

	CNewExpenseDlg dlg(this);
	dlg.DoModal();
}

void CMainFrame::OnUnmarkAll() 
{
	//CExportOptionsDlg exportOptionsDlg;
	//exportOptionsDlg.DoModal();

	TVisualObject *pObject = m_Framework.GetActivePane();
	BaseDataGrid *gv = (BaseDataGrid*)pObject->GetWnd();
	gv->UnMarkAllRows();
	gv->Invalidate(FALSE);
}

void CMainFrame::OnExit()
{
	if(AppCfg().GetBool(CFG_ASKBEFOREEXIT) && AfxMessageBox(IDS_ASKEXIT,MB_YESNO,0) != IDYES)
			return;
	PostMessage(WM_COMMAND,ID_APP_EXIT);
}

void CMainFrame::OnOptions()
{
	COptionsSheet optdlg;
	optdlg.DoModal();

//	m_pKeyDetailedInformationSubmitThread->NotifyCompanyInfoChanged();
	CMarkWarnShowOptions::GetInstance()->Load();
	UpdateGrid(INCOWSGRID);
	UpdateGrid(ALLCOWSGRID);
 
}
void CMainFrame::OnStockEdit()
{
	CStocksDlg stocksDlg(this);
	stocksDlg.DoModal();

}

bool CMainFrame::TryTrashCow(Cow *cowd)
{	
	Doc *docd = cowd->GetInDoc();
	if(AppCfg().GetBool(CFG_DOUBLEASKDEL))
	{
		if(AfxMessageBox(IDS_ASKCOWDEL,MB_YESNO,0) == IDNO)
		return false;
	}

	if(AfxMessageBox(IDS_CONFIRMCOWDEL,MB_YESNO) == IDNO)
	return false;

		if(!cowd->CanDeleteThisCow())
		{
			AfxMessageBox(IDS_ERRCANTDELCOW);
			return false;							       
		}
		if(docd->GetCowCount() == 1) //tring delete one and only cow in this PZ
		{
			AfxMessageBox(IDS_ERRONLYCOW,MB_OK,0); 
			return false;
		}	
	return true;
}
//***************************************************************************************
bool CMainFrame::TrashHent(CHentArray &mainArray,CHentArray &subArray,int nElement)
{
	int i,s;
	
	Hent *hentd = mainArray[nElement];
    CString msg;

	if(!hentd->CanDeleteThisHent())
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return false;
	}

	if(AfxMessageBox(IDS_CONFHENTDELETE,MB_YESNO,0) == IDNO)
		return false;
	

	for(i=0,s = ary_cows.GetSize();i<s;i++)
	if(ary_cows[i]->GetFirstOwner() == hentd)
	{
		msg.Format(IDS_HENTASSGNEDASFSTOWNR,(LPCSTR)ary_cows[i]->GetEAN());
		AfxMessageBox(msg,MB_OK,0);
		return false;
	}

	for(i=0,s = ary_indocs.GetSize();i<s;i++)
	if(ary_indocs[i]->GetHent() == hentd)
	{
		msg.Format(IDS_HENTASSIGNEDTOINDOC,ary_indocs[i]->GetId());
		MessageBox(msg);
		return false;
	}
	for(i=0,s = ary_outdocs.GetSize();i<s;i++)
	if(ary_outdocs[i]->GetHent() == hentd)
	{
		msg.Format(IDS_HENTASSIGNEDTOOUTDOC,ary_outdocs[i]->GetId());
		AfxMessageBox(msg,MB_OK,0);
		return false;
	}

	mainArray.RemoveAt(nElement);
	subArray.RemovePtr(hentd);

	if(!hentd->DeleteThisHent())
	{
		SQLDB.SQLMessageBox(IDS_ERRWHILEDELETINGHENT);		
		return false;
	}
	
	delete hentd;		


	return true;
}
//***************************************************************************************
bool CMainFrame::TrashInDoc(int iAryIdx)
{
	CString msg;
	Cow *cow;
		
	InDoc *pInDoc = (InDoc*)ary_indocs[iAryIdx];

	if(!pInDoc->CanDeleteThisDoc())
	{
		AfxMessageBox(IDS_ERRCANTDELDOC,MB_OK,0);
		return false;
	}

	if(AfxMessageBox(IDS_CONFIRMDOCDEL,MB_YESNO) == IDNO)
		return false;

	CowArrayPtr cattle = pInDoc->GetCows();

	CCowArray::ArrayIterator cowIt = cattle->iterator();
	while(cowIt.hasNext())
	{
		cow = *(cowIt++);
		if(!cow->CanDeleteThisCow())
		{
			AfxMessageBox(IDS_ERRCANTDELCOW,MB_OK,0);
			return false;
		}
	}

	cowIt = cattle->iterator();
	while(cowIt.hasNext())
	{
		cow = *(cowIt++);
		if(!cow->DeleteThisCow(false))
		{
			SQLDB.SQLMessageBox(IDS_ERRWHILEDELETINGCOWS);
			SQLDB.Rollback();
			return false;
		}
	}

	if(!pInDoc->DeleteThisDoc(false)) 
	{ 
		SQLDB.SQLMessageBox(IDS_ERRWHILEDELETINGDOC);
		SQLDB.Rollback(); 
		return false;
	}

	cowIt = cattle->iterator();
	while(cowIt.hasNext())
	{
		cow = *(cowIt++);
		CPassImgBase::GetInstance()->DeletePassImg(cow);
	}
	
	ary_cows.RemoveArrayPtr(cattle);	
	ary_incows.RemoveArrayPtr(cattle);
	

	UpdateGrid(INCOWSGRID);
	UpdateGrid(ALLCOWSGRID);
				
	ary_indocs.RemoveAt(iAryIdx);
	cattle->FreeAndFlush();
	delete pInDoc;
	return true;
}
//***************************************************************************************
bool CMainFrame::TrashOutDoc(int iAryIdx)
{
	OutDoc *pOutDoc = (OutDoc*)ary_outdocs[iAryIdx];

	if(!pOutDoc->CanDeleteThisDoc())
	{
		AfxMessageBox(IDS_ERRCANTDELDOC,MB_OK,0);
		return false;
	}

	if(AfxMessageBox(IDS_CONFIRMDOCDEL,MB_YESNO) == IDNO)
		return false;

	CCowDataMatcher cowMatcher;
	CCountAction<CCowArray::ArrayAction,CCowDataMatcher,Cow> countInCows(&cowMatcher);
	CCowArray::ArrayIterator it = pOutDoc->GetCows()->iterator();
	while(it.hasNext())
	{
		cowMatcher.FindEAN((*(it++))->GetEAN());
		ary_incows.ForAllAction(countInCows);

		if(countInCows.GetCount() > 0)
		{
			AfxMessageBox(IDS_DELOUTDOCCOWAGAINNOTSOLD);
			return false;
		}

		countInCows.Reset();
		cowMatcher.Reset();
	}

	CCowArray::ArrayIterator cowIt =  pOutDoc->GetCows()->iterator();
	Cow* pCow;
	while(cowIt.hasNext())
	{
		pCow = *(cowIt++);
		pCow->DeleteOutDoc();
		pCow->ChangeOutGrp(NULL_INOUT_GRP);
	
		if(!pCow->UpdateThisCow(false))
		{
			SQLDB.SQLMessageBox(IDS_ERRUPDATINGCOWDELOUTDOC);
			while(!cowIt.isFirst())
				(*(--cowIt))->RollbackChanges();
			
			SQLDB.Rollback();
			return false;
		}
				
	}

	
	if(!pOutDoc->DeleteThisDoc(false)) //commit here
	{
		pOutDoc->GetCows()->RollbackChanges();
		SQLDB.SQLMessageBox(IDS_ERRWHILEDELETINGDOC);
		SQLDB.Rollback();
		return false;
	}

	pOutDoc->GetCows()->CommitChanges();
	SQLDB.Commit();
	ary_incows.AppendPtr(pOutDoc->GetCows());	
	ary_outdocs.RemoveAt(iAryIdx);
	delete pOutDoc;
	UpdateGrid(INCOWSGRID);

	return true;
}

void CMainFrame::OnStats()
{
	CStatDlg stat;
	stat.DoModal();
}
//***************************************************************************************
void  CMainFrame::OnUpdateStats(CCmdUI * pCmdUI)
{
	CString stattxt;
	int cowsize = ary_cows.GetSize(),balance = ary_incows.GetSize();
	stattxt.Format(IDS_BOTTOMSTATTEXT,cowsize,balance/*,cowsize-balance*/);
	pCmdUI->SetText( stattxt );
}
//***************************************************************************************
BOOL CMainFrame::OnInsertNew(UINT nID)
{
	
	switch(GetCurrGridID())
	{
		case INCOWSGRID:
		case ALLCOWSGRID:
		{
			if(nID == IDA_INSERTNEWSHIFT)
			{
				OnNewOutDoc();
				break;
			}
		}
		case INDOCSGRID:
		{
			OnNewInDoc();
			break;
		}

		case COMPANYHENTGRID:
		case FARMERHENTGRID:
		case HENTSGRID:
		{
			OnNewHent();
		break;
		}
		
		case OUTDOCSGRID:
		{	
			CNewOutDocDlg newoutdocdlg;
			newoutdocdlg.DoModal();
		break;
		}
	}
	return TRUE;	
}

void CMainFrame::OnMarkRow()
{

	BaseDataGrid *gv = GetCurrGrid();
	CListCtrl &list = gv->GetListCtrl();
	
	POSITION pos = list.GetFirstSelectedItemPosition();
	int item;
	while(pos)
	{	
		item = list.GetNextSelectedItem(pos);
		gv->GetItemPtr(item)->bMark = true;
	}
	gv->Invalidate(FALSE);
	

}

void CMainFrame::OnUnMarkRow()
{
	OnGridMenuCommand(IDM_GRID_UNMARK);
}

void CMainFrame::OnRuleEdit()
{
	CRulesView rulesDlg;
	rulesDlg.DoModal();
}

void CMainFrame::OnEditExpenseCategory()
{
	CExpenseCategoriesDlg expenseCategories(this);
	expenseCategories.DoModal();
}


void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
//	CFrameWnd::OnUpdateFrameTitle(true);
	CString str,lastlogin;

	UserData *pCurUser = UserData::GetCurrentUser();
	if(pCurUser)
	{
		if(pCurUser->lastlogin.IsNull())
			lastlogin.LoadString(IDS_FIRSTLOGIN);
		else
			lastlogin = pCurUser->lastlogin.GetFullDate();

		str.Format(IDS_MAINCAPTION,(LPCSTR)pCurUser->username,(LPCSTR)lastlogin);
	}
	else str.LoadString(IDS_ERRNOLOGGEDUSER);

	AfxSetWindowText(m_hWnd, (LPCTSTR) str);
}
void CMainFrame::OnLogout()
{
	CLoginDlg loginDlg;
	if(loginDlg.DoModal() == IDOK)
	{
		OnUpdateFrameTitle(TRUE);
		ChangeGrid(INCOWSGRID);
		EnableUserTabs();
		Invalidate();
	}
	else PostMessage(WM_COMMAND,ID_APP_EXIT);
}
void CMainFrame::OnUpdatePrintDoc(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}
void CMainFrame::OnPrintDocs(UINT nID)
{
	
	CRawDataArray selArray;
	GetGridSelectedRows<CRawDataArray>(GetCurrGridID(),selArray);
	CPrintDlg printDlg(selArray,nID);
	printDlg.DoModal();
}

void  CMainFrame::FlushArrays()
{
		ary_countries.FreeAndFlush();
		ary_stocks.FreeAndFlush();
		ary_hents.FreeAndFlush();
		ary_companies.RemoveAll();
		ary_farmers.RemoveAll();
		ary_incows.RemoveAll();
		ary_cows.FreeAndFlush();
		ary_indocs.FreeAndFlush();
		ary_outdocs.FreeAndFlush();
		ary_users.FreeAndFlush();
		ary_reasons.FreeAndFlush();
		ary_htypes.FreeAndFlush();
		ary_rules.FreeAndFlush();
		ary_sell_invs.FreeAndFlush();
		ary_buy_invs.FreeAndFlush();
		ary_rr_invs.FreeAndFlush();
		ary_expenses.FreeAndFlush();
		VERIFY(ary_expensecategories.RemoveAll());
		ary_agents.FreeAndFlush();


}

LRESULT CMainFrame::OnRestoreBackup(WPARAM wParam,LPARAM lParam)
{

	char *backupDir = NULL,*msgDir;
	CString copyDir,errmsg,strgFile,strgPath;

	switch(lParam)
	{
	case 0:

		msgDir = (char*)wParam;
		backupDir = new char[strlen(msgDir)+1];
		strcpy(backupDir,msgDir);
		PostMessage(WM_RESTORECOPY,(WPARAM)backupDir,1);
	break;
	case 1:
		FlushArrays();
		UpdateAllGrids();
		SQLDB.CloseCattleBase();
		backupDir = (char*)wParam;
		copyDir = backupDir;
		delete[] backupDir;

		if(!SQLDB.RestoreBackup(copyDir))
		{
			errmsg.Format(IDS_ERRRESTOREBACKUP,(LPCSTR)SQLDB.GetLastError());
			AfxMessageBox((LPCSTR)errmsg,MB_OK,0);
			return (LRESULT)0;
		}
		strgPath = paths_ConcatPath(copyDir,strgFile);

		if(boost::filesystem::exists(boost::filesystem::path((LPCSTR)strgPath)))
		{
			if(AfxMessageBox(IDS_ASKRESTOREPASSDBFILE,MB_YESNO,0) == IDYES)
			{			
				CPassImgBase *passImgDB = CPassImgBase::GetInstance();
				passImgDB->ClosePassImgBase();
				passImgDB->RestorePassFile(copyDir);
				if(!passImgDB->OpenPassImgBase())
				{
					AfxMessageBox(IDS_CANTOPENPASSDB);
				}
			}
		}
		else
		{
			AfxMessageBox(IDS_LEAVINGOLDPASSFILE,MB_OK,0);
		}
		
		LoadDB(NULL);
		
		
	break;

	}
			
	return (LRESULT)0;

}
void CMainFrame::UpdateAllGrids()
{
	    OnUpdateGrid(ALLCOWSGRID,0);
		OnUpdateGrid(INCOWSGRID,0);
		OnUpdateGrid(INDOCSGRID,0);
		OnUpdateGrid(OUTDOCSGRID,0);
		OnUpdateGrid(HENTSGRID,0);
		OnUpdateGrid(SELLINVSGRID,0);
		OnUpdateGrid(BUYINVSGRID,0);
		OnUpdateGrid(RRINVSGRID,0);
		OnUpdateGrid(COMPANYHENTGRID,0);
		OnUpdateGrid(FARMERHENTGRID,0);
		OnUpdateGrid(EXPENSESGRID,0);
}

void CMainFrame::OnDelRow()
{
	OnGridMenuCommand(IDM_GRID_DELETEROW);
}


BaseDataGrid * CMainFrame::GetCurrGrid()
{
	TVisualObject *pObject = m_Framework.GetActivePane();
	return (BaseDataGrid*)pObject->GetWnd();
}

UINT CMainFrame::GetCurrGridID()
{	
	return  m_Framework.GetActivePane()->GetID();
}

bool CMainFrame::TrashInvoice(CInvoiceDataArray &invArray,int nItem)
{

	Invoice *pInv = invArray[nItem];

	if(!pInv->CanDeleteThisInvoice())
	{
		AfxMessageBox(IDS_ERRCANTDELINV,MB_OK,0);
		return false;
	}

	if(AfxMessageBox(IDS_CONFIRMINVDEL,MB_YESNO) == IDNO)
		return false;

	bool bSuccess = false;
	switch(pInv->GetThisInvoiceType())
	{
		case IN_INV_VAT:
		case OUT_INV_VAT:
			bSuccess = TrashVATInvoice((VATInvoice*)pInv);
			break;	
		case IN_INV_RR:
			bSuccess = TrashRRInvoice((RRInvoice*)pInv);
			break;
		case CORRCT_INV:
			bSuccess = TrashCorrectInvoice(pInv);
			break;
	}	

	if(bSuccess)
	{
		invArray.RemoveAt(nItem);
		delete pInv;
		return true;
	}
	return false;

}
bool CMainFrame::TrashVATInvoice(VATInvoice* pInv)
{
	CDocDataArray::ArrayIterator docIt = pInv->invoiceDocs.iterator();
	Doc *pDoc = NULL;
	while(docIt.hasNext())
	{
		pDoc = *(docIt++);
		pDoc->DeleteInvoice();
		if(!pDoc->UpdateThisDoc(false))
		{
			while(!docIt.isFirst()) (*(--docIt))->RollbackChanges();
			SQLDB.Rollback();
			SQLDB.SQLMessageBox(IDS_ERRUPDATINGDOCWHENDELINV);							
			return false;
		}
	}

	if(!DeleteCowInvoiceEntries(pInv->invEntries,false) || !pInv->DeleteThisInvoice(false) )
	{
		pInv->invoiceDocs.RollbackChanges();
		SQLDB.Rollback();
		SQLDB.SQLMessageBox(IDS_ERRDELETEINGINVOICE);
		return false;
	}
	
	SQLDB.Commit();
	pInv->invoiceDocs.CommitChanges();
	pInv->invEntries.FreeAndFlush();
	return true;	
			
}

bool CMainFrame::TrashRRInvoice(RRInvoice* pInv)
{
	CCowInvoiceEntryArray::ArrayIterator it = pInv->invEntries.iterator();
	Cow *pCow = NULL;
	while(it.hasNext())
	{
		pCow = (*(it++))->GetCow();
		pCow->DeleteRRInvoice();
		if(!pCow->UpdateThisCow(false))
		{
			while(!it.isFirst()) (*(--it))->GetCow()->RollbackChanges();
			SQLDB.Rollback();
			SQLDB.SQLMessageBox(IDS_ERRUPDATINGCOWWHENDELINV);
			return false;
		}
	}

	if(!DeleteCowInvoiceEntries(pInv->invEntries,false) || !pInv->DeleteThisInvoice(false) )
	{
		pInv->invEntries.RollbackCowChanges();
		SQLDB.Rollback();
		SQLDB.SQLMessageBox(IDS_ERRDELETEINGINVOICE);
		return false;
	}

	SQLDB.Commit();
	pInv->invEntries.CommitCowChanges();
	pInv->invEntries.FreeAndFlush();
	return true;
}


bool CMainFrame::TrashCorrectInvoice(Invoice* pInv)
{
	if(!DeleteCowInvoiceEntries(pInv->invEntries,false))
	{
		SQLDB.Rollback();
		SQLDB.SQLMessageBox(IDS_ERRDELETINGCORRECTINVOICE);		
		return false;
	}
	if(!pInv->DeleteThisInvoice(false))
	{
		SQLDB.Rollback();
		SQLDB.SQLMessageBox(IDS_ERRDELETINGCORRECTINVOICE);		
		return false;
	}
	SQLDB.Commit();
	pInv->GetCorrectFor()->SetCorrect(NULL);
	pInv->invEntries.FreeAndFlush();

	return true;
}


bool CMainFrame::DeleteCowInvoiceEntries(CCowInvoiceEntryArray &invEntries,bool bCommit)
{
	CCowInvoiceEntryArray::ArrayIterator it = invEntries.iterator();
	while(it.hasNext())
	{
		if(!(*(it++))->DeleteThis(bCommit))
		{
			return false;
		}
	}
	return true;
}

void CMainFrame::OnTermSync()
{
#ifndef SPECIAL_TRIAL_RELEASE
	if(!GetApplicationKeyManager()->IsFeatureEnabled(FEATURE_COLLECTOR))
	{
		AfxMessageBox(IDS_FEATUREDISABLED);
		return;
	}
	
	CSyncDlg syncDlg(this);
	syncDlg.DoModal();
#else
	AfxMessageBox(IDS_FEATUREDISABLED);
#endif
}


void CMainFrame::OnFastSelect()
{
	int docno;
	CString fastSelStr;	
	TVisualObject *pObject = m_Framework.GetActivePane();
	BaseDataGrid *gv  = (BaseDataGrid*)pObject->GetWnd();
	UINT gridID = pObject->GetID();

	m_fastSelect.GetWindowText(fastSelStr);
	m_fastSelect.SelAllText();
	
	if(gridID == BUYINVSGRID ||
		gridID == SELLINVSGRID ||
		gridID == OUTDOCSGRID ||
		gridID == INDOCSGRID ||
		gridID == RRINVSGRID)
	{
		docno = atoi((LPCSTR)fastSelStr);
		if(docno <= 0) return;
	}
	else
	{
		if(fastSelStr.GetLength() == 0) return;
	}
	UINT fastSelectFlags = FASTSELECT_NOOPTION;

	if(m_FastSelSelRow)	fastSelectFlags |= FASTSELECT_SELROW;
	if(m_FastSelMarkRow) fastSelectFlags |= FASTSELECT_MARKROW;
	if(m_FastSelPosOnRow) fastSelectFlags |= FASTSELECT_POSONROW;
	if(m_FastSelFindFirst) fastSelectFlags |= FASTSELECT_FINDFIRST;

	CInvDataMatcher invMatcher;
	CCowDataMatcher cowMatcher;
	CDocDataMatcher docMatcher;
	CHentMatcher hentMatcher;
	CExpenseDataMatcher expenseMatcher;

	CFastSelectAction<CInvoiceDataArray::ArrayAction,CInvDataMatcher,Invoice> fastSelInvoice(&invMatcher,gv,fastSelectFlags);
	CFastSelectAction<CCowArray::ArrayAction,CCowDataMatcher,Cow> fastSelCow(&cowMatcher,gv,fastSelectFlags);
	CFastSelectAction<CDocDataArray::ArrayAction,CDocDataMatcher,Doc> fastSelDoc(&docMatcher,gv,fastSelectFlags);
	CFastSelectAction<CHentArray::ArrayAction,CHentMatcher,Hent> fastSelHent(&hentMatcher,gv,fastSelectFlags);
	CFastSelectAction<CExpenseDataArray::ArrayAction,CExpenseDataMatcher,Expense> fastSelExpense(&expenseMatcher,gv,fastSelectFlags);

    

	switch(gridID)
	{
		case BUYINVSGRID:
			invMatcher.FindInvNo(docno,docno);
			ary_buy_invs.ForAllAction(fastSelInvoice);
		break;
		case SELLINVSGRID:
			invMatcher.FindInvNo(docno,docno);
			ary_sell_invs.ForAllAction(fastSelInvoice);				
		break;
		case RRINVSGRID:
			invMatcher.FindInvNo(docno,docno);
			ary_rr_invs.ForAllAction(fastSelInvoice);
		break;
		case OUTDOCSGRID:
			docMatcher.FindDocNo(docno,docno);
			ary_outdocs.ForAllAction(fastSelDoc);
		break;
		case INDOCSGRID:
			docMatcher.FindDocNo(docno,docno);
			ary_indocs.ForAllAction(fastSelDoc);						
		break;

		case COMPANYHENTGRID: 
			hentMatcher.FindARiMRNo(fastSelStr);
			ary_companies.ForAllAction(fastSelHent);
		break;
		case FARMERHENTGRID:
			hentMatcher.FindARiMRNo(fastSelStr);
			ary_farmers.ForAllAction(fastSelHent);
		break;

		case HENTSGRID:
			hentMatcher.FindARiMRNo(fastSelStr);
			ary_hents.ForAllAction(fastSelHent);
		break;
		case INCOWSGRID:
			cowMatcher.FindEAN(fastSelStr);
			ary_incows.ForAllAction(fastSelCow);
		break;
		case ALLCOWSGRID:
			cowMatcher.FindEAN(fastSelStr);
			ary_cows.ForAllAction(fastSelCow);			
		break;
		case EXPENSESGRID:
			expenseMatcher.FindExpenseTitle(fastSelStr);
			ary_expenses.ForAllAction(fastSelExpense);
			break;
	}		

	gv->Invalidate(FALSE);	
}
void CMainFrame::OnFastSelReturn(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnFastSelect();
	*pResult = 0;
}

void CMainFrame::OnFastSelectOptCommand(UINT nID)
{	
	switch(nID)
	{
		case IDM_FASTSELECT_MENU_SELECTFOUND:
			m_FastSelSelRow = !m_FastSelSelRow;
			if(!(m_FastSelSelRow || m_FastSelMarkRow))
				m_FastSelMarkRow = TRUE;
			break;
		case IDM_FASTSELECT_MENU_SETPOSONROW:
			m_FastSelPosOnRow = !m_FastSelPosOnRow;
			break;
		case IDM_FASTSELECT_MENU_FINDFIRSTROW:
			m_FastSelFindFirst = !m_FastSelFindFirst;
			break;
		case IDM_FASTSELECT_MENU_MARKFOUND:
			m_FastSelMarkRow = !m_FastSelMarkRow;
			if(!(m_FastSelSelRow || m_FastSelMarkRow))
				m_FastSelSelRow = TRUE;
			break;
	}
}

void CMainFrame::OnFastSelectOptUpdateCommand(CCmdUI *pCmdUI)
{
	BOOL bCheck = FALSE;
	switch(pCmdUI->m_nID)
	{
		case IDM_FASTSELECT_MENU_SELECTFOUND:
			bCheck = m_FastSelSelRow;
			break;
		case IDM_FASTSELECT_MENU_SETPOSONROW:
			bCheck = m_FastSelPosOnRow;
			break;
		case IDM_FASTSELECT_MENU_FINDFIRSTROW:
			bCheck = m_FastSelFindFirst;
			break;
		case IDM_FASTSELECT_MENU_MARKFOUND:
			bCheck = m_FastSelMarkRow;
			break;
	}
	pCmdUI->SetCheck(bCheck);
}


LRESULT CMainFrame::OnSetActivePane(WPARAM wParam,LPARAM lParam)
{
	TVisualObject *pObject = m_Framework.Get(wParam);
	if(pObject) pObject->SetActivePane();
	return (LRESULT)0;
}







LRESULT CMainFrame::OnDoArchive(WPARAM wParam,LPARAM lParam)
{

	char nameBuff[30];
	time_t now = time(NULL);
	struct tm *t1;
	CPassImgBase* passImgDB	= CPassImgBase::GetInstance();

	FlushArrays();
	UpdateAllGrids();
	SQLDB.CloseCattleBase();

	CString checkExists,archiveDir = AppCfg().GetString(CFG_ARCHIVEDIR);
	if(!paths_IsPathAbsolute(archiveDir))
		archiveDir = paths_ConcatPath(CUtil::GetAppDir(),archiveDir);
		
	t1  = localtime(&now);	
	strftime(nameBuff,sizeof(nameBuff),"archive_%Y%m%d",t1);

	archiveDir =  paths_ConcatPath(archiveDir,CString(nameBuff));


	checkExists = archiveDir; 
	int r = 1;
	while(paths_DoesPathExist(checkExists) != DOES_NOT_EXIST)
	{
		checkExists.Format("%s_%d",(LPCSTR)archiveDir,r);
		r++;
	}
	archiveDir = checkExists;

	if(!paths_CreateIfNeeded(archiveDir))
	{
		AfxMessageBox(CStrException::ErrorString(GetLastError()));
		return (LRESULT)0;
	}

	if(!SQLDB.DoBackup(archiveDir))
		SQLDB.SQLMessageBox(IDS_ERRDOINGARCHIVE);
	else
	{
		
		if(AppCfg().GetBool(CFG_DOPASSIMGDBARCHIVE))
		{
			if(!passImgDB->DoBackup(archiveDir))
			{					
				AfxMessageBox(CStrException::ErrorString(GetLastError()));					
			}
			else
				passImgDB->PurgeDB();
		}
	}	
	
	if(!SQLDB.OpenCattleBase())
	{
		SQLDB.SQLMessageBox(IDS_ERRCANTOPENCATTLEBASE);
		PostMessage(WM_COMMAND,ID_APP_EXIT);
		return (LRESULT)0;
	}

	if(!SQLDB.DropDocs())
	{
		SQLDB.SQLMessageBox(IDS_ERRCANTDROPDOCS);
		PostMessage(WM_COMMAND,ID_APP_EXIT);
		return (LRESULT)0;
	}

	if(!SQLDB.LoadUsers(ary_users))
	{
		SQLDB.SQLMessageBox(IDS_ERRCANTLOADUSERS);
		PostMessage(WM_COMMAND,ID_APP_EXIT);
		return (LRESULT)0;
	}

	CLoginDlg loginDlg(NULL,true,true,this);
	if(loginDlg.DoModal() != IDOK)
	{
		SQLDB.CloseCattleBase();
		ary_users.FreeAndFlush();
		PostMessage(WM_COMMAND,ID_APP_EXIT);
		return (LRESULT)0;        
	}

	if(!SQLDB.LoadDB())
	{
		SQLDB.SQLMessageBox(IDS_CANTLOADDB);
		PostMessage(WM_COMMAND,ID_APP_EXIT);
		return (LRESULT)0;
	}
	UpdateAllGrids();


	return (LRESULT)0;
}

void CMainFrame::OnLoadPrimaryDB()
{
	LoadDB(NULL);
}

void CMainFrame::OnLoadCustomDirDB()
{	
	CString strBaseDir,browseCaption;
	browseCaption.LoadString(IDS_CHOOSEBASEDIR);
	if(CBrowseForDir::BrowseForDir(strBaseDir,(LPCSTR)browseCaption,this))
	{
			LoadDB((LPCSTR)strBaseDir);
	}
}

BOOL CMainFrame::OnLoadArchiveBackupDB(UINT nID)
{
	CString loadDir,wildCard,loadWildPath;
	UINT cfgItem;
	UINT nCaption;	
	UINT wildCardRes;

	switch(nID)
	{
	case IDMM_LOAD_ARCHIVE:
		cfgItem = CFG_ARCHIVEDIR;
		wildCardRes = IDS_ARCHIVEWILDCARD;
		nCaption = IDS_LOADARCHIVECAPTION;
		break;
	case IDMM_LOAD_BACKUP:
		cfgItem = CFGID_BACKUPDIR;
		wildCardRes = IDS_BACKUPWILDCARD;
		nCaption = IDS_LOADBACKUPCAPTION;
		break;
	default:
		ASSERT(FALSE);
	}

	loadDir = AppCfg().GetString(cfgItem);
	wildCard.LoadString(wildCardRes);

	if(!paths_IsPathAbsolute(loadDir))
		loadDir = paths_ConcatPath(CUtil::GetAppDir(),loadDir);
	loadWildPath = paths_ConcatPath(loadDir,wildCard);


	CSubDirSelect selectDlg(loadWildPath);
	selectDlg.SetCaption(nCaption);
	if(selectDlg.DoModal() == IDOK)
	{
		loadDir = paths_ConcatPath(loadDir,selectDlg.GetSelectedSubDirPath());
		LoadDB((LPCSTR)loadDir);
	}

	return TRUE;
}

void CMainFrame::LoadDB(LPCSTR pszLoadDir)
{
	CPassImgBase *pPassDB = CPassImgBase::GetInstance();

	FlushArrays();
	UpdateAllGrids();
	pPassDB->ClosePassImgBase();
	SQLDB.CloseCattleBase();

	if(!pPassDB->OpenPassImgBase(pszLoadDir))
			AfxMessageBox(IDS_ERRNOPASSDBFILE);				
	
	


	do
	{
		if(!SQLDB.OpenCattleBase(pszLoadDir))
		{			
			SQLDB.SQLMessageBox(IDS_ERROPENINGDB);
			if(pszLoadDir)
			{
				if(AfxMessageBox(IDS_ASKLOADPRIMARYBASE,MB_YESNO) == IDYES)
				{
					LoadDB(NULL);
					return;
				}
			}
			
			PostMessage(WM_COMMAND,ID_APP_EXIT);				
			return;
		}
		try
		{
			if(SQLDB.RequiresUpdate())
			{
				if(AfxMessageBox(IDS_ASKDBUPDATE,MB_YESNO,0) == IDYES)
				{
					if(SQLDB.UpdateToRequired())
					{
						break;	
					}
					
				}
				SQLDB.CloseCattleBase();
				if(AfxMessageBox(IDS_ASKLOADPRIMARYBASE,MB_YESNO) == IDYES)
				{
					LoadDB(NULL);
					return;
				}
				PostMessage(WM_COMMAND,ID_APP_EXIT);
				
			}
			else
			{
				break;
			}
		}
		catch(CSQLException* e)
		{
			AfxMessageBox((LPCSTR)e->GetError());
			e->Delete();
			PostMessage(WM_COMMAND,ID_APP_EXIT);
			return;
		}
	}
	while(false);



	if(!SQLDB.LoadUsers(ary_users))
	{
		SQLDB.SQLMessageBox(IDS_ERRWHILELOADINGUSERS);
		if(pszLoadDir)
		{
			if(AfxMessageBox(IDS_ASKLOADPRIMARYBASE,MB_YESNO) == IDYES)
			{
				LoadDB(NULL);
				return;
			}
		}
	}
	

	CLoginDlg loginDlg(NULL,true,true);
	if(loginDlg.DoModal() != IDOK)
	{
		if(pszLoadDir)
		{
			if(AfxMessageBox(IDS_ASKLOADPRIMARYBASE,MB_YESNO) == IDYES)
			{
				LoadDB(NULL);
				return;
			}
		}
			
		PostMessage(WM_COMMAND,ID_APP_EXIT); 
		return;        
	}

	LoadTaskTranslator loadTaskTrans;
	CTaskProgressDialog TaskDlg(CTempStringLoader(IDS_LOADING),&loadTaskTrans);
	TaskDlg.Create(this); 

	if(SQLDB.LoadDB(true,&TaskDlg))
	{
		UpdateAllGrids();
//		m_pKeyDetailedInformationSubmitThread->NotifyCompanyInfoChanged();
		return;
	}
	else
	{
		if(!TaskDlg.CheckCancelButton())
				SQLDB.SQLMessageBox(IDS_ERROPENINGDB);
		FlushArrays();

		if(pszLoadDir)
		{
			if(AfxMessageBox(IDS_ASKLOADPRIMARYBASE,MB_YESNO) == IDYES)
			{
				LoadDB(NULL);
				return;
			}
		}
		PostMessage(WM_COMMAND,ID_APP_EXIT); 
	}	

}







//***************************************************************************************
LRESULT CMainFrame::OnLockGrid(WPARAM wParam,LPARAM lParam)
{	
	BaseDataGrid *gv = GetGridView((int)wParam);
	VERIFY(gv);	
	gv->Lock();
	return (LRESULT)0;
}
//***************************************************************************************
BOOL CMainFrame::SetupStatusBar()
{
	CString str;

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
		LOG("Failed to create status bar");
		return FALSE;      
	}

	//m_wndStatusBar.DrawPaneSeparatorsSet(false);
	//m_wndStatusBar.OuterRectInFirstBandSet(false);

	int nIndex = m_wndStatusBar.CommandToIndex(IDC_INDICATOR_STATISTICS);
	m_wndStatusBar.SetPaneStyle(0,SBPS_STRETCH );	
	int max = 999999999;
	str.Format(IDS_BOTTOMSTATTEXT,max,max,max);
	CDC *pDC = m_wndStatusBar.GetDC();
	CSize size = pDC->GetTextExtent(str);
	m_wndStatusBar.ReleaseDC(pDC);    
    m_wndStatusBar.SetPaneWidth(nIndex,size.cx);

	nIndex = m_wndStatusBar.CommandToIndex(IDC_INDICATOR_NOPASSIMGDB);
	m_wndStatusBar.SetPaneWidth(nIndex,16);

	nIndex = m_wndStatusBar.CommandToIndex(IDC_INDICATOR_BARCODESCANNER);
	m_wndStatusBar.SetPaneWidth(nIndex,16);

	
	if (!m_KeyNotPresentProgressCtrl.Create(
			WS_CHILD|WS_VISIBLE|WS_TABSTOP, //|PBS_SMOOTH
			CRect(0,0,0,0), 
			&m_wndStatusBar, 
			0))
	{
		LOG("Failed to create key not present progress control");
		return FALSE;
	}

	m_KeyNotPresentProgressCtrl.ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_FRAMECHANGED);
		
	//m_KeyExpiryProgressCtrl.m_bUsePaintManager
	
	nIndex = m_wndStatusBar.CommandToIndex(IDS_HKEY_NOTPRESENT_PROGRESS);
	m_wndStatusBar.SetPaneWidth(nIndex, 100);
	m_wndStatusBar.SetPaneControl(m_KeyNotPresentProgressCtrl, IDS_HKEY_NOTPRESENT_PROGRESS, true);
	
	return TRUE;
	
}
//***************************************************************************************
void  CMainFrame::OnUpdatePassImgDBStat(CCmdUI * pCmdUI)
{
	CString tip;
	if(CPassImgBase::GetInstance()->IsOpen())
	{
		m_wndStatusBar.GetStatusBarCtrl().SetIcon(pCmdUI->m_nIndex,m_hIconGood);
		tip.LoadString(IDS_TIPPASSIMGDBLOADED);
		m_wndStatusBar.SetTipText(pCmdUI->m_nIndex,(LPCSTR)tip);
	}
	else
	{
		
		m_wndStatusBar.GetStatusBarCtrl().SetIcon(pCmdUI->m_nIndex,m_hIconBad);
		tip.LoadString(IDS_TIPPASSIMGDBNOTLOADED);
		m_wndStatusBar.SetTipText(pCmdUI->m_nIndex,(LPCSTR)tip);
		
	}
}

void CMainFrame::OnUpdateBarcodeScannerStatus(CCmdUI * pCmdUI)
{
	if(CBarcodeScannerDriver::IsScannerOnline())
	{
		m_wndStatusBar.GetStatusBarCtrl().SetIcon(pCmdUI->m_nIndex,m_hIconYesBarcode);
	}
	else
	{
		m_wndStatusBar.GetStatusBarCtrl().SetIcon(pCmdUI->m_nIndex,m_hIconNoBarcode);
	
	}
}
	
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	if(!m_bShowingKeyState && GetApplicationKeyManager()->GetKeyState() != KeyManager::VALID)
	{
		m_bShowingKeyState = true;
		CKeyStateDlg keyStateDlg(this);
		if(keyStateDlg.DoModal() == IDCANCEL)
		{
			PostMessage(WM_COMMAND,ID_APP_EXIT);
		}
		else
		{
			m_bShowingKeyState = false;
		}
	}
}
void CMainFrame::OnGridFontZoomOut()
{
	CGrid* pGrid = GetCurrGrid();
	
	pGrid->SetFontHeight(pGrid->GetFontHeight()-1);
}

void CMainFrame::OnGridFontZoomIn()
{
	CGrid* pGrid = GetCurrGrid();
	
	pGrid->SetFontHeight(pGrid->GetFontHeight()+1);
	
}

void CMainFrame::OnGridGoToBottom()
{
	CGrid* pGrid = GetCurrGrid();
	pGrid->SetCurPos(pGrid->GetItemCount()-1);
}

void CMainFrame::OnGridGoToTop()
{
	GetCurrGrid()->SetCurPos(0);
}

void CMainFrame::OnUpdateGridFontZoomIn(CCmdUI* pCmdUI)
{
	CGrid* pGrid = GetCurrGrid();
	
	pCmdUI->Enable(pGrid->GetFontHeight() < pGrid->GetMaxFontSize());
}

void CMainFrame::OnUpdateGridFontZoomOut(CCmdUI* pCmdUI)
{
	CGrid* pGrid = GetCurrGrid();
	
	pCmdUI->Enable(pGrid->GetFontHeight() > pGrid->GetMinFontSize());
}

void CMainFrame::OnEditAgents()
{
	CAgentsDlg agentsDlg(this);
	agentsDlg.DoModal();
}

void CMainFrame::OnHentSync()
{
	CRepoAccountsSyncCtrlDlg hentSyncCtrlDlg(this);
	hentSyncCtrlDlg.DoModal();

}
