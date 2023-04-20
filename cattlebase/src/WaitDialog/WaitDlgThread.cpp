#include "stdafx.h"
#include "WaitDlgThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CWaitDlgThread, CWinThread)
	ON_THREAD_MESSAGE(TM_EXITTHREAD,OnExitThread)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CWaitDlgThread, CWinThread)

CWaitDlgThread::CWaitDlgThread()
{
   m_bShowCancelButton = false;
}
void CWaitDlgThread::SetShowCancelButton(bool bShowCancel)
{
	m_bShowCancelButton = bShowCancel;
}
void CWaitDlgThread::SetWaitText(const CString& waitText)
{
	m_Text = waitText;
}

CWaitDlgThread::~CWaitDlgThread()
{
	
}

BOOL CWaitDlgThread::InitInstance()
{	

	CWnd	desktop;
	desktop.Attach( ::GetDesktopWindow() );
	
	m_pWaitDlg = new CWaitDlg;

	m_pWaitDlg->EnableAbortButton(m_bShowCancelButton);
    m_pWaitDlg->SetWaitText(m_Text);
	m_pWaitDlg->Create( CWaitDlg::IDD, &desktop );

	m_pMainWnd = m_pWaitDlg;
	m_pActiveWnd = m_pWaitDlg;

	m_pMainWnd->ShowWindow( SW_SHOW );
	m_pMainWnd->UpdateWindow();
 
   desktop.Detach();
   
   return TRUE;
}

void CWaitDlgThread::OnExitThread(WPARAM wParam,LPARAM lParam)
{
	delete m_pWaitDlg;
	ExitInstance();
}
