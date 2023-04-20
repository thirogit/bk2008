#include "stdafx.h"
#include "MyPrintPreview.h"
#include "DYMODocView.h"
/////////////////////////////////////////////////////////////////////////////
// CMyPreviewView

IMPLEMENT_DYNCREATE(CMyPreviewView, CPreviewView)

BEGIN_MESSAGE_MAP(CMyPreviewView, CPreviewView)
	
	ON_COMMAND(AFX_ID_PREVIEW_CLOSE, OnPreviewClose)
	ON_COMMAND(AFX_ID_PREVIEW_NUMPAGE, OnNumPageChange)
	ON_COMMAND(AFX_ID_PREVIEW_NEXT, OnNextPage)
	ON_COMMAND(AFX_ID_PREVIEW_PREV, OnPrevPage)
	ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)
	ON_COMMAND(AFX_ID_PREVIEW_ZOOMIN, OnZoomIn)
	ON_COMMAND(AFX_ID_PREVIEW_ZOOMOUT, OnZoomOut)

END_MESSAGE_MAP()


void CMyPreviewView::OnNextPage()
{
	CPreviewView::OnNextPage();
	m_pToolBar->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
}

void CMyPreviewView::OnPrevPage()
{
	CPreviewView::OnPrevPage();
	m_pToolBar->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
}

void CMyPreviewView::OnZoomIn()
{
	CPreviewView::OnZoomIn();
	m_pToolBar->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
}

void CMyPreviewView::OnZoomOut()
{
	CPreviewView::OnZoomOut();
	m_pToolBar->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
}
void CMyPreviewView::OnNumPageChange()
{
	CPreviewView::OnNumPageChange();
	m_pToolBar->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
}

void CMyPreviewView::OnPreviewPrint()
{
	((CBaseDocView*)m_pPrintView)->Print(1);
	SendMessage(WM_COMMAND,AFX_ID_PREVIEW_CLOSE);
}

void CMyPreviewView::OnPreviewClose()
{

	this->m_pPreviewInfo->m_bContinuePrinting = FALSE;
	CWnd* pMainWnd = GetParentFrame();
	CPreviewView::OnPreviewClose();
	pMainWnd->DestroyWindow();
	//((CDrawView*)m_pPrintView)->OnPrintPreviewClose();

}

CMyPreviewView::CMyPreviewView()
{
}

