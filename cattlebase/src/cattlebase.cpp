#include "StdAfx.h"
#include "cattlebase.h"
#include "MainFrm.h"
#include "cattlebaseDoc.h"
#include "cattlebaseView.h"
#include "dlgs/welcome.h"
#include <afxsock.h>
#include "log.h"
#include "http_update/HttpVersionInfo.h"
#include "VersionNo.h"
#include "CattlebaseMutex.h"
#include "missing/paths.h"
#include "splash/SplashProgress.h"
#include "hkey/ui/KeyStateDlg.h"
#include <gdiplus.h>
#include "dao/db/SQLSingleConnectionPool.h"
#include "repo/utils/StringEncoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define __CFG_IMPLEMENTATION__
#include "MainCfg.h"

KeyManager* GetApplicationKeyManager()
{
	return ((CCattlebaseApp*)AfxGetApp())->GetKeyManager();
}

BEGIN_MESSAGE_MAP(CCattlebaseApp, CWinAppEx)
	
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	
END_MESSAGE_MAP()

CCattlebaseApp::CCattlebaseApp()
{
	m_pKeyManagerThread = NULL;
	m_pSIMutex = NULL;
	CoInitialize(NULL);
	AfxSocketInit();
	srand((UINT)time(NULL));
}

CCattlebaseApp::~CCattlebaseApp()
{
	if (m_pSIMutex != NULL)
	{	
		m_pSIMutex->Unlock();
		delete m_pSIMutex;
		m_pSIMutex = NULL;
	}
	CoUninitialize();
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CCattlebaseApp object

CCattlebaseApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCattlebaseApp initialization

BOOL CCattlebaseApp::InitInstance()
{
	InitCommonControls();
	CWinAppEx::InitInstance();
	AfxEnableControlContainer();
	InitShellManager();

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	m_pSIMutex = new CMutex(FALSE,MUTEX_NAME);
	if(!m_pSIMutex->Lock(1))
	{
		AfxMessageBox(IDS_ERRMUTEX,MB_OK,0);
		return FALSE;
	}

	CString logsPath = paths_ConcatPath(CUtil::GetAppDir(),"LOGS");
	paths_CreateIfNeeded(logsPath);
	CString todaysLogFileName;
	todaysLogFileName.Format("LOG%s.LOG",CDateTime::now().GetDateInFmt(CDateTime::YYYYMMDD));
	CString todaysLogFilePath = paths_ConcatPath(logsPath,todaysLogFileName);

#ifdef DO_LOGGING
	AMLOG_SETFILENAME(_T((LPCSTR)todaysLogFilePath));
	AMLOG_SETLOGLEVEL_INFO;
#endif
	
//#ifndef NOAUTOUPDATES
//	if(CheckForNewVersion())
//	{
//		if(RunUpdater())
//		{
//			return FALSE;
//		}
//		else
//		{
//			AfxMessageBox(IDS_FAILDTORUNUPDATER);
//		}
//
//	}
//#endif

	
	m_pKeyManagerThread = (KeyManager*)AfxBeginThread(RUNTIME_CLASS(KeyManager));
	m_pKeyManagerThread->m_bAutoDelete = true;

	if(m_pKeyManagerThread->GetKeyState() != KeyManager::VALID)
	{
		CKeyStateDlg keyStateDlg;
		if(keyStateDlg.DoModal() != IDOK)
		{
			return FALSE;
		}
	}

	SQLSingleConnectionPool::GetInstance()->CreatePool(1,5);
	
	
	CString mainCfgFilePath = CUtil::GetAppDir() + "\\CATTLEBASECFG.CFG";
	// Initialize the cfg class and load settings from file if they exist.
	if (AppCfg().Init((LPCSTR)mainCfgFilePath) != CFG_NOERR)
	{
		AfxMessageBox(IDS_ERRCFGERROR);
		return FALSE;
	}
	
	SetRegistryKey(_T("CowBase"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CWelcomeDlg welcome;
	if(welcome.DoModal() != IDOK) return FALSE; 
	
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCattlebaseDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CCattlebaseView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	//m_pMainWnd->SetFocus();


	return TRUE;
}

int CCattlebaseApp::ExitInstance()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	SQLSingleConnectionPool::GetInstance()->DestroyPool();

	if(m_pKeyManagerThread != NULL)
	{
		m_pKeyManagerThread->Shutdown();
	}


	return CWinAppEx::ExitInstance();
}

KeyManager* CCattlebaseApp::GetKeyManager()
{
	return m_pKeyManagerThread;
}

BOOL CCattlebaseApp::CheckForNewVersion()
{
	CSplashProgress splash;
	CHttpVersionInfo *httpVersion = CHttpVersionInfo::GetInstance();
	
	if(httpVersion->CheckForNewerVersion(&splash))
	{
		if(AfxMessageBox(IDS_ASKFORUPDATE,MB_YESNO) == IDYES)
			return TRUE;
	}
	return FALSE;
}
bool CCattlebaseApp::RunUpdater()
{
	STARTUPINFO         siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;

	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));

	siStartupInfo.cb = sizeof(siStartupInfo);

	CString updaterExePath = paths_ConcatPath(CUtil::GetAppDir(),"updater.exe");

	return CreateProcess(updaterExePath, 
				 NULL,
				 0,
				 0,
				 FALSE,
				 CREATE_DEFAULT_ERROR_MODE,
				 0,
				 0,                  
				 &siStartupInfo,
				 &piProcessInfo) == TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
protected:
	CString m_nameversion;
	CString m_licencedTo;
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	
	m_nameversion.LoadString(AFX_IDS_APP_TITLE);

//	CHardwareKey *pHKey = CHardwareKey::GetHKey();
//	m_licencedTo = pHKey->GetLicencedTo();

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_NAMEVERSION,m_nameversion);
	DDX_Text(pDX,IDC_LICENCEDTO,m_licencedTo);
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CCattlebaseApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCattlebaseApp message handlers

