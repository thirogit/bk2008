// MainTreeCtrl.cpp : implementation of the CMainTreeCtrl class
//

#include "stdafx.h"
#include "MainTreeCtrl.h"
#include "IMainTreeCtrlListener.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectTreeView

IMPLEMENT_DYNCREATE(CMainTreeCtrl, CTreeView)

BEGIN_MESSAGE_MAP(CMainTreeCtrl, CTreeView)
	//{{AFX_MSG_MAP(CProjectTreeView)
	ON_NOTIFY_REFLECT_EX(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT_EX(TVN_ITEMEXPANDED, OnItemExpanded)
	//}}AFX_MSG_MAP
	// Standard printing commands
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectTreeView construction/destruction

CMainTreeCtrl::CMainTreeCtrl():
	listener_(NULL)
{
	
}

CMainTreeCtrl::~CMainTreeCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainTreeCtrl diagnostics

#ifdef _DEBUG
void CMainTreeCtrl::AssertValid() const
{
	CTreeView::AssertValid();
}

void CMainTreeCtrl::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainTreeCtrl message handlers

BOOL CMainTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if (listener_ && (pNMTreeView->itemOld.hItem != pNMTreeView->itemNew.hItem))
		listener_->OnTreeNodeSelection(pNMTreeView->itemOld.hItem, pNMTreeView->itemNew.hItem);
		
	*pResult = 0;

	return TRUE;
}

BOOL CMainTreeCtrl::OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if (listener_)
		listener_->OnTreeNodeExpansion(pNMTreeView->itemNew.hItem);
		
	*pResult = 0;
	
	return TRUE;
}


void CMainTreeCtrl::RegisterListener(IMainTreeCtrlListener* listener)
{
	
	listener_ = listener;
}

