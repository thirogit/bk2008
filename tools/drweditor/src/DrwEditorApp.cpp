#include "stdafx.h"
#include "DrwEditorApp.h"
#include "MainFrm.h"

//#include "drawobj.h"
#include "DrwDoc.h"
//#include "drawvw.h"
//#include "splitfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CDrwEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
END_MESSAGE_MAP()


CDrwEditorApp::CDrwEditorApp()
{
	
}


CDrwEditorApp::~CDrwEditorApp()
{
		
}

CDrwEditorApp theApp;

BOOL CDrwEditorApp::InitInstance()
{
	InitCommonControls();

	SetRegistryKey( _T("DRWEDITOR") );

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MFC_VER < 0x700
	#ifdef _AFXDLL
		Enable3dControls();         // Call this when using MFC in a shared DLL
	#else
		Enable3dControlsStatic();   // Call this when linking to MFC statically
	#endif
#endif


	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_DRAWCLTYPE,
		RUNTIME_CLASS(CDrwDoc),
		RUNTIME_CLASS(CSplitFrame),
		RUNTIME_CLASS(CDrwDocView));
	pDocTemplate->SetContainerInfo(IDR_DRAWCLTYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	m_pMainWnd = pMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		m_pMainWnd=NULL;
		return FALSE;
	}

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// The main window has been initialized, so show and update it.
	//pMainFrame->ShowWindow(m_nCmdShow);
	//pMainFrame->UpdateWindow();
	pMainFrame->ActivateFrame( m_nCmdShow );

	return TRUE;
}

// App command to run the dialog
void CDrwEditorApp::OnAppAbout()
{
#ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
	VERIFY( ProfUISAbout() );
#endif // #ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
}

void CDrwEditorApp::DoContextHelp() 
{
	CWinApp::OnContextHelp();
}

void CDrwEditorApp::OnFilePrintSetup()
{
	CWinApp::OnFilePrintSetup();
}
