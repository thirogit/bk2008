/************************************************************************
 Author: David A. Jones
 Date:	January 25, 2004

 Synopsis:

 Replaces the CWinThread class to provide timer messages using the new
 methods to AddTimer, and RemoveTimer.

 When a Timer event processed by the TimerProc() static callback it will
 lookup the idEvent for the thread object to post the thread message. 
 The thread message is posted to the corresponding thread in the form
 of a WM_TIMER message with idEvent as the WPARAM and the dwTime in the 
 LPARAM.

 The m_autoremove member will remove all timer requests outstanding
 and it set by default. If you wish to manage your own add remove then 
 you can turn this off in the initialization of your derrived class

 ***********************************************************************/

#if !defined(CWINTIMERTHREAD_H)
#define CWINTIMERTHREAD_H
#pragma once

class CWinTimerThread : public CWinThread
{
public:
		DECLARE_DYNCREATE(CWinTimerThread)
		//
		CWinTimerThread();
		virtual ~CWinTimerThread();
protected:
		
		UINT_PTR AddTimer(UINT uElapse);
		BOOL RemoveTimer(UINT_PTR idEvent);

};

#endif // CWINTIMERTHREAD_H