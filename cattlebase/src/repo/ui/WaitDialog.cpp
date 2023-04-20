#include "stdafx.h"
#include "WaitDialog.h"

#include <boost/thread/future.hpp>


ExceptionInterceptor::ExceptionInterceptor(boost::function<void()> &fn) : m_pException(NULL),m_Function(fn)
{
}

void ExceptionInterceptor::Invoke()
{
	m_pException = NULL;
	try
	{
		m_Function();
	}
	catch(CException* e)
	{
		m_pException = e;
	}
}

bool ExceptionInterceptor::HasEception()
{
	return m_pException != NULL;
}

CException* ExceptionInterceptor::GetException()
{
	return m_pException;
}



CWaitDialog::CWaitDialog (CWnd* pParentWnd, const CString& sWaitText) : CDialog ()
{
   m_pParentWnd = pParentWnd;
   m_sWaitText = sWaitText;   
}

void CWaitDialog::WaitFor(boost::function<void()> &f)
{
	Show();

	ExceptionInterceptor incterceptor(f);

	boost::packaged_task<void> waitTask(boost::bind(&ExceptionInterceptor::Invoke,&incterceptor));
	boost::unique_future<void> future = waitTask.get_future();

	boost::thread task(boost::move(waitTask));
	while(!future.is_ready())
	{
		Pump();
	}
	Hide();

	if(incterceptor.HasEception())
		throw incterceptor.GetException();
}

CWaitDialog::~CWaitDialog ()
{
    
}

void CWaitDialog::Show ()
{
    m_pParentWnd->EnableWindow (FALSE);
    Create (IDD_BUSY_DLG,m_pParentWnd);
    CStatic* pWaitTextCtrl = (CStatic*) GetDlgItem (IDC_BUSY_TEXT);
    pWaitTextCtrl->SetWindowText (m_sWaitText);     
    ShowWindow (SW_SHOW);
}

BOOL CWaitDialog::Pump ()
{
    MSG msg;

    // Retrieve and dispatch any waiting messages.
    while (::PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE)) {
        if (!AfxGetApp ()->PumpMessage ()) {
            ::PostQuitMessage (0);
            return FALSE;
        }
    }

    // Simulate the framework's idle processing mechanism.
    LONG lIdle = 0;
    while (AfxGetApp ()->OnIdle (lIdle++));
    return TRUE;
}





void CWaitDialog::Hide ()
{
	m_pParentWnd->EnableWindow (TRUE);
	ShowWindow (SW_HIDE);
    DestroyWindow ();   
}

