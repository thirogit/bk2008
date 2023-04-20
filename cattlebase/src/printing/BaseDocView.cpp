#include "stdafx.h"
#include "BaseDocView.h"
#include "MyPrintPreview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CBaseDocView, CScrollView)
		
//	ON_COMMAND(IDC_GOPRINT, OnPrint)
		
END_MESSAGE_MAP()

CBaseDocView::CBaseDocView()
{

	m_pOldFrame = NULL;
	m_pCtrlDlg = NULL;
	nCurPrintPage = -1;


}

CCowBasePrintDoc* CBaseDocView::GetDocument()
{
	return (CCowBasePrintDoc*)GetDocument();
}


BOOL CBaseDocView::PreCreateWindow(CREATESTRUCT& cs)
{
	ASSERT(cs.style & WS_CHILD);
	if (cs.lpszClass == NULL)
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE|WS_EX_STATICEDGE);
	return TRUE;
}

void CBaseDocView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
	// Show the original frame
	m_pOldFrame->ShowWindow(SW_SHOW);
	// Restore main frame pointer
	AfxGetApp()->m_pMainWnd = m_pOldFrame;

	m_pCtrlDlg->EnableWindow(TRUE);
}

void CBaseDocView::OnFilePrint()
{
	Print(1);
}

void CBaseDocView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrint(pDC,pInfo);
}
void CBaseDocView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnBeginPrinting(pDC,pInfo);
}
void CBaseDocView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnEndPrinting(pDC,pInfo);
}