#include "stdafx.h"
#include "Wait.h"
#include "WaitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CWait::CWait()
{
	m_bShowCancelButton = false;
	m_Thread = NULL;
}
void CWait::SetWaitText(const CString& waitText)
{
	m_Text = waitText;
}
void CWait::ShowCancelButton()
{
	m_bShowCancelButton = true;
}

CWait::~CWait()
{
}

void CWait::Show()
{
	m_Thread = new CWaitDlgThread();// *)AfxBeginThread(RUNTIME_CLASS(CWaitDlgThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_Thread->SetWaitText(m_Text);
	m_Thread->SetShowCancelButton(m_bShowCancelButton);
	//m_Thread->m_ThreadThread();
	m_Thread->CreateThread();

}

void CWait::Close()
{
	m_Thread->PostThreadMessage(TM_EXITTHREAD,0,0);
	WaitForSingleObject(m_Thread->m_hThread,INFINITE);
	
}

void CWait::SetTask(UINT taskID,int steps)
{
	//m_Thread->m_pMainWnd->SendMessage(WM_PROGRESS_SETTASK,taskID,steps);
}
bool CWait::OnStep(int step)
{
	//return m_Thread->m_pMainWnd->SendMessage(WM_PROGRESS_ONSTEP,step) != 0;
	return true;
}
void CWait::OnTaskError(UINT errorCode)
{
	//m_Thread->m_pMainWnd->SendMessage(WM_PROGRESS_ONERROR,errorCode);
}
