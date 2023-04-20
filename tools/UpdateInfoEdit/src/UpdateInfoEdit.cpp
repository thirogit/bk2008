// UpdateInfoEdit.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "UpdateInfoEdit.h"
#include "UpdateInfoEditDlg.h"
#include <missing/util.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define __CFG_IMPLEMENTATION__
#include "MainCfg.h"


// CUpdateInfoEditApp

BEGIN_MESSAGE_MAP(CUpdateInfoEditApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUpdateInfoEditApp construction

CUpdateInfoEditApp::CUpdateInfoEditApp()
{
	
}


// The one and only CUpdateInfoEditApp object

CUpdateInfoEditApp theApp;


// CUpdateInfoEditApp initialization

BOOL CUpdateInfoEditApp::InitInstance()
{
	CWinApp::InitInstance();

	VERIFY( AfxInitRichEdit() );

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	CString mainCfgFilePath = CUtil::GetAppDir() + "\\VERINFO.CFG";
	if (AppCfg().Init((LPCSTR)mainCfgFilePath) != CFG_NOERR)
	{
		AfxMessageBox(IDS_ERRCFGERROR);
		return FALSE;
	}

	SetRegistryKey(_T("UpdateInfoEdit"));

	CUpdateInfoEditDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		
	}
	else if (nResponse == IDCANCEL)
	{
		
	}

	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
