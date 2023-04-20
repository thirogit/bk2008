#include "stdafx.h"
#include "TimerTipWnd.h"

#define HIDE_TIMER_ID 1002
#define MIN_SHOW_TIME_MS 1000

BEGIN_MESSAGE_MAP(CTimerTipWnd,CTipWnd)
	
	ON_WM_TIMER()

END_MESSAGE_MAP()


CTimerTipWnd::CTimerTipWnd() 
{
}

CTimerTipWnd::~CTimerTipWnd()
{

}

BOOL CTimerTipWnd::Show(  CPoint& pPoint, LPCTSTR lpszItemText, UINT hideAfterMiliseconds)
{
	if(hideAfterMiliseconds < MIN_SHOW_TIME_MS)
		return FALSE;

	if(CTipWnd::Show(pPoint,lpszItemText))
	{
		SetTimer(HIDE_TIMER_ID,hideAfterMiliseconds,NULL);	
		return TRUE;
	}
	return FALSE;
}

void CTimerTipWnd::OnTimer(UINT nTimerID)
{
	if(nTimerID == HIDE_TIMER_ID)
	{
		KillTimer(nTimerID);
		Hide();
	}
	CTipWnd::OnTimer(nTimerID);
}

