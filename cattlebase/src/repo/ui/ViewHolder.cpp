#include "stdafx.h"
#include "ViewHolder.h"
#include "RepoAccountSessionView.h"


ViewHolder::ViewHolder()
{
	m_pActiveSessionView = NULL;
}
	
void ViewHolder::Show(RepoAccountSessionView* pSessionView)
{
	if(pSessionView)
	{
		CRect rect;
		GetClientRect(&rect);
		if(m_pActiveSessionView)
		{
			m_pActiveSessionView->ShowWindow(SW_HIDE);
		}
		pSessionView->SetWindowPos(&wndTop,0,0,rect.Width(),rect.Height(),SWP_SHOWWINDOW | SWP_FRAMECHANGED);
		m_pActiveSessionView = pSessionView;
	}
	else
	{
	}
}

BOOL ViewHolder::Create(CWnd* pParentWnd,const RECT& rect,UINT nID)
{
	BOOL bCreate = CStatic::Create("",WS_CHILD | SS_CENTERIMAGE| WS_TABSTOP,rect,pParentWnd,nID);	
	if(bCreate)
	{
		ModifyStyleEx(0,WS_EX_CONTROLPARENT);
		ASSERT(GetExStyle() & WS_EX_CONTROLPARENT);
	}

	return bCreate;

}



