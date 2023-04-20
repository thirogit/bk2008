#include "stdafx.h"
#include "CustomPreviewView.h"
#include "CustomPaperView.h"

IMPLEMENT_DYNCREATE(CCustomPreviewView, CMyPreviewView)


CCustomPreviewView::CCustomPreviewView()
{
}

void CCustomPreviewView::PositionPage(UINT nPage)
{
	//CPreviewView::PositionPage(nPage);
	CSize windowSize = CalcPageDisplaySize();

	//VERIFY(m_dcPrint.Escape(GETPHYSPAGESIZE, 0, NULL,
	//		(LPVOID)&m_pPageInfo[nPage].sizeUnscaled));

	CSize* pSize = &m_pPageInfo[nPage].sizeUnscaled;
	CSize paperSize = ((CCustomPaperView*)m_pPrintView)->GetPaperSize();
	pSize->SetSize(paperSize.cx,paperSize.cy);

	HDC hDCScreen = ::GetDC(NULL);
	int cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);
	int cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);

	// Convert page size to screen coordinates
	pSize->cx = MulDiv(pSize->cx, cxPixelsPerInch, m_sizePrinterPPI.cx);
	pSize->cy = MulDiv(pSize->cy, cyPixelsPerInch, m_sizePrinterPPI.cy);

	m_pPageInfo[nPage].sizeZoomOutRatio = CalcScaleRatio(windowSize, *pSize);

	SetScaledSize(nPage);
}


void CCustomPreviewView::OnDraw(CDC* pDC)
{
	ASSERT_VALID(pDC);

	// don't do anything if not fully initialized
	if (m_pPrintView == NULL || m_dcPrint.m_hDC == NULL)
		return;

	CPoint ViewportOrg = pDC->GetViewportOrg();

	CPen rectPen;
	rectPen.CreatePen(PS_SOLID, 2, GetSysColor(COLOR_WINDOWFRAME));
	CPen shadowPen;
	shadowPen.CreatePen(PS_SOLID, 3, GetSysColor(COLOR_BTNSHADOW));

	m_pPreviewInfo->m_bContinuePrinting = TRUE;     // do this once each paint

	for (UINT nPage = 0; nPage < m_nPages; nPage++)
	{
		int nSavedState = m_dcPrint.SaveDC();       // Save pristine state of DC

		// Use paint DC for print preview output
		m_pPreviewDC->SetOutputDC(pDC->GetSafeHdc());

		m_pPreviewInfo->m_nCurPage = m_nCurrentPage + nPage;

		// Only call PrepareDC if within page range, otherwise use default
		// rect to draw page rectangle
		if (m_nCurrentPage + nPage >= m_nCurrentPage && 
		    m_nCurrentPage + nPage >= nPage && 
		    m_nCurrentPage + nPage <= m_pPreviewInfo->GetMaxPage())
		{
			m_pPrintView->OnPrepareDC(m_pPreviewDC, m_pPreviewInfo);
		}

		// Set up drawing rect to entire page (in logical coordinates)
		m_pPreviewInfo->m_rectDraw.SetRect(0, 0,
			m_pPreviewDC->GetDeviceCaps(HORZRES),
			m_pPreviewDC->GetDeviceCaps(VERTRES));
		m_pPreviewDC->DPtoLP(&m_pPreviewInfo->m_rectDraw);

		// Draw empty page on screen

		pDC->SaveDC();          // save the output dc state

		CSize* pRatio = &m_pPageInfo[nPage].sizeScaleRatio;
		CRect* pRect = &m_pPageInfo[nPage].rectScreen;

		if (pRatio->cx == 0)
		{   // page position has not been determined
			PositionPage(nPage);    // compute page position
			if (m_nZoomState != _AFX_ZOOM_OUT)
			{
				ViewportOrg = -GetDeviceScrollPosition();
				if (m_bCenter)
				{
					CRect rect;
					GetClientRect(&rect);
					// if client area is larger than total device size,
					// override scroll positions to place origin such that
					// output is centered in the window
					if (m_totalDev.cx < rect.Width())
						ViewportOrg.x = (rect.Width() - m_totalDev.cx) / 2;
					if (m_totalDev.cy < rect.Height())
						ViewportOrg.y = (rect.Height() - m_totalDev.cy) / 2;
				}
			}
		}

		pDC->SetMapMode(MM_TEXT);   // Page Rectangle is in screen device coords
		pDC->SetViewportOrg(ViewportOrg);
		pDC->SetWindowOrg(0, 0);

		pDC->SelectStockObject(HOLLOW_BRUSH);
		pDC->SelectObject(&rectPen);
		pDC->Rectangle(pRect);

		pDC->SelectObject(&shadowPen);

		pDC->MoveTo(pRect->right + 1, pRect->top + 3);
		pDC->LineTo(pRect->right + 1, pRect->bottom + 1);
		pDC->MoveTo(pRect->left + 3, pRect->bottom + 1);
		pDC->LineTo(pRect->right + 1, pRect->bottom + 1);

		// erase background to white (most paper is white)
		CRect rectFill = *pRect;
		rectFill.left += 1;
		rectFill.top += 1;
		rectFill.right -= 2;
		rectFill.bottom -= 2;
		::FillRect(pDC->m_hDC, rectFill, (HBRUSH)GetStockObject(WHITE_BRUSH));

		pDC->RestoreDC(-1);     // restore to synchronized state

		if (!m_pPreviewInfo->m_bContinuePrinting ||
				m_nCurrentPage + nPage > m_pPreviewInfo->GetMaxPage())
		{
			m_pPreviewDC->ReleaseOutputDC();
			m_dcPrint.RestoreDC(nSavedState);   // restore to untouched state

			// if the first page is not displayable, back up one page
			// but never go below 1
			if (nPage == 0 && m_nCurrentPage > 1)
				SetCurrentPage(m_nCurrentPage - 1, TRUE);
			break;
		}

		// Display page number
		OnDisplayPageNumber(m_nCurrentPage, nPage + 1);

		// Set scale ratio for this page
		m_pPreviewDC->SetScaleRatio(pRatio->cx, pRatio->cy);

		CSize PrintOffset;
		//VERIFY(m_pPreviewDC->Escape(GETPRINTINGOFFSET, 0, NULL, (LPVOID)&PrintOffset));

		PrintOffset.SetSize(0,0);

		m_pPreviewDC->PrinterDPtoScreenDP((LPPOINT)&PrintOffset);
		PrintOffset += (CSize)pRect->TopLeft();
		PrintOffset += CSize(1, 1);
		PrintOffset += (CSize)ViewportOrg;  // For Scrolling

		m_pPreviewDC->SetTopLeftOffset(PrintOffset);

		m_pPreviewDC->ClipToPage();
		((CCustomPaperView*)m_pPrintView)->OnPrint(m_pPreviewDC, m_pPreviewInfo);

		m_pPreviewDC->ReleaseOutputDC();

		m_dcPrint.RestoreDC(nSavedState);   // restore to untouched state

	}

	rectPen.DeleteObject();
	shadowPen.DeleteObject();
}
