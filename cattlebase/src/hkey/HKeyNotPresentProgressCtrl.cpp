#include "stdafx.h"
#include "HKeyNotPresentProgressCtrl.h"
#include "KeyManager.h"
#include "../cattlebase.h"

#define CHECK_KEY_PRESENT_EVERY_MS (3*1000)
#define TIMER_ID 101
#define ABSENCE_SCALE 100

CHardwareKeyNotPresentProgressCtrl::CHardwareKeyNotPresentProgressCtrl()
{
}

LRESULT CHardwareKeyNotPresentProgressCtrl::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_CREATE)
	{
		LRESULT lResult = CExtProgressWnd::WindowProc( uMsg, wParam, lParam );
		if(lResult == 0)
		{			
			SetRange(0,ABSENCE_SCALE);
			SetPos(ABSENCE_SCALE);
			SetStep(-1);
			SetTimer(TIMER_ID,CHECK_KEY_PRESENT_EVERY_MS,NULL);
		}
		return lResult;		
	}
	else if( uMsg == WM_TIMER && wParam  == TIMER_ID)
	{
		KillTimer(TIMER_ID);
		KeyManager* pKeyManager = GetApplicationKeyManager();
		time_t tMaxAbsence = pKeyManager->GetAbsentLimitSecs();
		time_t tKeyAbsence = pKeyManager->GetAbsentTimeSecs();

		short absence = ABSENCE_SCALE;
		if(tKeyAbsence > 0)
		{
			if(tKeyAbsence > tMaxAbsence)
			{
				absence = 0;
			}
			else
			{
				absence = ABSENCE_SCALE - (short)((tKeyAbsence*ABSENCE_SCALE)/tMaxAbsence);
			}
		}			
		
		SetPos(absence);			
		
		SetTimer(TIMER_ID,CHECK_KEY_PRESENT_EVERY_MS,NULL);
	}
	else if( uMsg == WM_DESTROY )
	{
		KillTimer(TIMER_ID);
	}
	
	return CExtProgressWnd::WindowProc( uMsg, wParam, lParam );
}
