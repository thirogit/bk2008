#include "stdafx.h"
#include "DYMODocView.h"
#include <afxpriv.h>
#include "CustomPreviewView.h"
#include "PrinterTypeChoice.h"
#include "DYMOPrinterDialog.h"
#include "SeikoPrinterDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CDYMODocView, CScrollView)

BEGIN_MESSAGE_MAP(CDYMODocView, CScrollView)

END_MESSAGE_MAP()

CDYMODocView::CDYMODocView()
{


}

CDYMODocView::~CDYMODocView()
{

}



void CDYMODocView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);
}

void CDYMODocView::OnDraw(CDC* pDC)
{
	CDYMOBaseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	CSize lblSize = pDoc->GetPixelPageSize();
	CRect rect(0,0,lblSize.cx,lblSize.cy);
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC,lblSize.cx,lblSize.cy);

	CDC DCGlyphs;
	DCGlyphs.CreateCompatibleDC(pDC);
	CBitmap* bmOld = DCGlyphs.SelectObject(&bm);
	pDoc->Draw(&DCGlyphs);

	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),&DCGlyphs, 0, 0, SRCCOPY);

	DCGlyphs.SelectObject(bmOld);
	DCGlyphs.DeleteDC();
	
}



void CDYMODocView::OnInitialUpdate()
{
	CSize size = GetDocument()->GetPixelPageSize();
	SetPageSize(size);
}

CSize CDYMODocView::GetPaperSize()
{
	return GetDocument()->GetPixelPageSize();
}

void CDYMODocView::SetPageSize(CSize size)
{
	SetScrollSizes(MM_TEXT, size);
}

BOOL CDYMODocView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(GetDocument()->GetPrintPages());
	if(pInfo->m_bPreview)
	{
		pInfo->m_pPD->m_pd.hDC = ::GetDC(NULL);
	}
	return DoPreparePrinting(pInfo);
}

void CDYMODocView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnBeginPrinting(pDC,pInfo);
}

void CDYMODocView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{

}

void CDYMODocView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	GetDocument()->SetFields(pInfo->m_nCurPage);
	OnDraw(pDC);
}

void CDYMODocView::PrintDYMO(int nCopies)
{
	CDYMOPrinterDialog DYMODlg(nCopies,this);
	if(DYMODlg.PreparePrinting())
	{
		UINT nNumOfCopies = DYMODlg.GetNumberOfCopies();
		CPrinter* pPrinter = DYMODlg.GetPrinter();
		
		CDYMOBaseDoc* pDYMODoc = GetDocument();
		int nNumOfPages = pDYMODoc->GetPrintPages();
		for(int iPage = 1;iPage <= nNumOfPages;iPage++)
		{
			pDYMODoc->SetFields(iPage);
			pPrinter->Print(this,nNumOfCopies);
		}
	}
}

void CDYMODocView::PrintSeiko(int nCopies)
{
	CSeikoPrinterDialog SeikoDlg(nCopies,this);
	if(SeikoDlg.PreparePrinting())
	{
		CPrinter* pPrinter = SeikoDlg.GetPrinter();
		UINT nNumOfCopies = SeikoDlg.GetNumberOfCopies();
		pPrinter->Print(this,nNumOfCopies);		
	}
}



void CDYMODocView::Print(int nCopies)
{	
	UINT uPrinterType = CPrinterChoiceDlg::ChoosePrinterType(PRINT_MASK_ALL,this);

	switch(uPrinterType)
	{
	case PRINT_DYMO:
		PrintDYMO(nCopies);		
		break;
	case PRINT_SEIKO:
		PrintSeiko(nCopies);
		break;

	case PRINT_WINPRINT:
		CScrollView::OnFilePrint();
		break;
	}
}


void CDYMODocView::PrintPreview()
{

	ASSERT_VALID( this );
	CPrintPreviewState * pState = new CPrintPreviewState;

	if( !DoPrintPreview(
		AFX_IDD_PREVIEW_TOOLBAR,
		this,
		RUNTIME_CLASS(CCustomPreviewView),
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

