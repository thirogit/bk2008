
// DPU-S445.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DPU-S445.h"
#include "DPU-S445Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDPUS445App

BEGIN_MESSAGE_MAP(CDPUS445App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDPUS445App construction

CDPUS445App::CDPUS445App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDPUS445App object

CDPUS445App theApp;


// CDPUS445App initialization

BOOL CDPUS445App::InitInstance()
{
	CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CDPUS445Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

