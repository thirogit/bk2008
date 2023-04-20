/************************************************************************
 Author: David A. Jones
 Date:	January 25, 2004

 Synopsis:

 See the WinTimerthread.h for more details of this class

 ***********************************************************************/
#include "stdafx.h"
#include "WinTimerThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Static declarations
static CMapPtrToPtr g_MapTimerID_TO_CWinThreadPtr;

static void CALLBACK TimerProc(
	HWND hwnd,         // handle to window
	UINT uMsg,         // WM_TIMER message
	UINT_PTR idEvent,  // timer identifier
	DWORD dwTime       // current system time 
	)
{
	CWinThread* pThread;

	if (g_MapTimerID_TO_CWinThreadPtr.Lookup((void *) idEvent, (void*&) pThread))
	{
		pThread->PostThreadMessage(WM_TIMER, idEvent, dwTime);
	}
}
// Class 
IMPLEMENT_DYNCREATE(CWinTimerThread, CWinThread)

CWinTimerThread::CWinTimerThread()
{
	
}

CWinTimerThread::~CWinTimerThread()
{
	POSITION pos;
	UINT_PTR idEvent;
	CWinThread* pThread;

	//get the starting point in the list of timer events
	pos = g_MapTimerID_TO_CWinThreadPtr.GetStartPosition();
	//loop while there are list items
	while (pos)
	{
		// get timer event entry
		g_MapTimerID_TO_CWinThreadPtr.GetNextAssoc(pos, (void *&) idEvent, (void*&) pThread);
		// check if the idevent is refering to this object
		if (pThread == this)
		{
		   RemoveTimer(idEvent);
		}
	}
}


UINT_PTR CWinTimerThread::AddTimer(UINT uElapse)
{
	UINT_PTR idEvent;

	idEvent = ::SetTimer(NULL, NULL, uElapse, (TIMERPROC) TimerProc);
	g_MapTimerID_TO_CWinThreadPtr.SetAt((void *) idEvent, this);
	return idEvent;
}
BOOL CWinTimerThread::RemoveTimer(UINT_PTR idEvent)
{
	::KillTimer(NULL, idEvent);
	return g_MapTimerID_TO_CWinThreadPtr.RemoveKey((void *) idEvent);
}