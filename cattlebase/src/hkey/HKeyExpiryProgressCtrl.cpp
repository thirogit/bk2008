#include "stdafx.h"
#include "HKeyExpiryProgressCtrl.h"



CHardwareKeyExpiryProgressCtrl::CHardwareKeyExpiryProgressCtrl()
{
}
LRESULT CHardwareKeyExpiryProgressCtrl::WindowProc(    
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
		)
	{

		if( uMsg == WM_TIMER )
		{
			StepIt();
		}
		else if( uMsg == WM_DESTROY )
		{
			KillTimer(0);
		}
		LRESULT lResult = CExtProgressWnd::WindowProc( uMsg, wParam, lParam );
		return lResult;
	}
	 
