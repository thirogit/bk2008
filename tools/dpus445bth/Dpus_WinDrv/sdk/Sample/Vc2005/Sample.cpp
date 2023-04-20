// Sample.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Sample.h"
#include "SampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleDlgApp

BEGIN_MESSAGE_MAP(CSampleDlgApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSampleDlgApp construction

CSampleDlgApp::CSampleDlgApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSampleDlgApp object

CSampleDlgApp theApp;


// CSampleDlgApp initialization

BOOL CSampleDlgApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	if( LoadDll() )
	{
		CSampleDlg dlg;
		m_pMainWnd = &dlg;
		dlg.DoModal();
		UnloadDll();
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
