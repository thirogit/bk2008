#include "stdafx.h"
#include "CowDrwDocView.h"
#include <afxpriv.h>
#include "MyPrintPreview.h"
#include "PrinterTypeChoice.h"
#include "SeikoPrinterDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCowDrwDocView, CScrollView)

BEGIN_MESSAGE_MAP(CCowDrwDocView, CScrollView)

END_MESSAGE_MAP()

CCowDrwDocView::CCowDrwDocView()
{


}

CCowDrwDocView::~CCowDrwDocView()
{

}



void CCowDrwDocView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);

	SetUpDC(pDC);


	if(pInfo != NULL)
	{
		pInfo->m_rectDraw.SetRect(0, 0,	pDC->GetDeviceCaps(HORZRES),pDC->GetDeviceCaps(VERTRES));
		pDC->DPtoLP(&pInfo->m_rectDraw);

		CCowDrwDoc* pDoc = GetDocument();
		CSize l_PrintMarg = pDoc->GetPaperSize()-pDoc->GetPageSize();
		CSize l_phyMarg = pDoc->GetPhyMarginsSize();
		CSize l_margOff = pDoc->GetMarginOffset();

		pDC->SetWindowOrg(
			-(pInfo->m_rectDraw.Width()+l_PrintMarg.cx)/2 + l_phyMarg.cx,
			(-pInfo->m_rectDraw.Height() + l_PrintMarg.cy )/2  + l_phyMarg.cy);

		pDC->OffsetWindowOrg(-l_margOff.cx,l_margOff.cy);


	}
	else
	{
		CSize size = GetDocument()->GetPaperSize();
		pDC->SetWindowOrg(-size.cx/2,size.cy/2);
	}



}


void CCowDrwDocView::SetUpDC(CDC *pDC)
{

	// mapping mode is MM_ANISOTROPIC
	// these extents setup a mode similar to MM_LOENGLISH
	// MM_LOENGLISH is in .01 physical inches
	// these extents provide .01 logical inches

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(254, -254);
}


void CCowDrwDocView::OnDraw(CDC* pDC)
{
	CCowDrwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = NULL;



	// only paint the rect that needs repainting
	CRect client;




	pDC->GetClipBox(client);
	CRect rect = client;
	DocToClient(rect);


	if (!pDC->IsPrinting())
	{
		// draw to offscreen bitmap for fast looking repaints
		if (dc.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;

				// offset origin more because bitmap is just piece of the whole drawing
				dc.OffsetViewportOrg(-rect.left, -rect.top);
				pOldBitmap = dc.SelectObject(&bitmap);
				dc.SetBrushOrg(rect.left % 8, rect.top % 8);

				// might as well clip to the same rectangle
				dc.IntersectClipRect(client);
			}
		}
	}

	// paint background
	CBrush brush;
	if (!brush.CreateSolidBrush(pDoc->GetPaperColor()))
		return;

	brush.UnrealizeObject();
	pDrawDC->FillRect(client, &brush);


	pDoc->Draw(pDrawDC);

	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBitmap);
	}
}



void CCowDrwDocView::OnInitialUpdate()
{
	CSize size = GetDocument()->GetPaperSize();
	SetPageSize(size);
}

void CCowDrwDocView::SetPageSize(CSize size)
{
	CClientDC dc(NULL);
	SetUpDC(&dc);
	CSize vport = dc.GetViewportExt();	

	size.cy = MulDiv(size.cy, vport.cy, 254);
	size.cx = MulDiv(size.cx, vport.cx, 254);

	SetScrollSizes(MM_TEXT, size);	
}

BOOL CCowDrwDocView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation

#ifdef FOR_CATTLEBASE

	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(GetDocument()->GetPrintPages());

#endif

	return DoPreparePrinting(pInfo);
}

void CCowDrwDocView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CCowDrwDoc *pDoc = GetDocument();
	CScrollView::OnBeginPrinting(pDC,pInfo);

	// check page size -- user could have gone into print setup
	// from print dialog and changed paper or orientation
	pDoc->ComputePageSize();
	nCurPrintPage = -1;

}

void CCowDrwDocView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{


}

void CCowDrwDocView::ClientToDoc(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);
}

void CCowDrwDocView::ClientToDoc(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rect);
	ASSERT(rect.left <= rect.right);
	ASSERT(rect.bottom <= rect.top);
}

void CCowDrwDocView::DocToClient(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
}

void CCowDrwDocView::DocToClient(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}

void CCowDrwDocView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CCowDrwDoc *pDoc = GetDocument();
	
	pDoc->SetPrintDC(pDC);

	if(pInfo->m_nCurPage != nCurPrintPage)
	{
		pDoc->PreparePage(pInfo->m_nCurPage);
		nCurPrintPage = pInfo->m_nCurPage;
	}	

	OnDraw(pDC);

	pDoc->PrintPage(pInfo->m_nCurPage);
	pDoc->SetPrintDC(NULL);
}


void CCowDrwDocView::Print(int nCopies)
{	
	UINT uPrinterType = CPrinterChoiceDlg::ChoosePrinterType(PRINT_SEIKO | PRINT_WINPRINT, this);

	switch(uPrinterType)
	{
	case PRINT_SEIKO:
		PrinteSeiko(nCopies);
		break;
	case PRINT_WINPRINT:
		PrintWindows(nCopies);
		break;
	}	
}

void CCowDrwDocView::PrinteSeiko(int nCopies)
{
	CSeikoPrinterDialog SeikoDlg(nCopies,this);
	if(SeikoDlg.PreparePrinting())
	{		
		UINT nNumOfCopies = SeikoDlg.GetNumberOfCopies();
		CPrinter* pPrinter = SeikoDlg.GetPrinter();
		pPrinter->Print(this,nNumOfCopies);
	}

}

void CCowDrwDocView::PrintWindows(int nCopies)
{
	CPrintDialog dlg(FALSE);

	if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd))
	{
		LPDEVMODE pDM;
		if ( (dlg.m_pd.hDevMode) &&
			(pDM = (DEVMODE*)GlobalLock(dlg.m_pd.hDevMode)) )
		{     
			pDM->dmCopies = nCopies;
			GlobalUnlock(dlg.m_pd.hDevMode);
		}
	}
	CScrollView::OnFilePrint();	
}


void CCowDrwDocView::PrintPreview()
{

	ASSERT_VALID( this );
	CPrintPreviewState * pState = new CPrintPreviewState;

	if( !DoPrintPreview(
		AFX_IDD_PREVIEW_TOOLBAR,
		this,
		RUNTIME_CLASS(CMyPreviewView),
		pState
		)
		)
	{		
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
	else
		m_pCtrlDlg->EnableWindow(FALSE);
}



