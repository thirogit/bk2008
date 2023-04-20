#include "stdafx.h"
#include "mainfrm_ctrl.h"

void SendMainFrmMessage(UINT msg,WPARAM wParam, LPARAM lParam)
{
	AfxGetApp()->m_pMainWnd->SendMessage(msg,wParam,lParam);
}
