#include "stdafx.h"

#include "MarginsCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMarginsCtrl

#define MINUSMARGIN (-200) // 1 cm
#define SLIDERWIDTH 20

CMarginsCtrl::CMarginsCtrl()
{
	m_sliderH = new CSliderCtrl;
	m_sliderV = new CSliderCtrl;
	ASSERT(MINUSMARGIN < 0); 
}

CMarginsCtrl::~CMarginsCtrl()
{
	delete m_sliderH;
	delete m_sliderV;
}


BEGIN_MESSAGE_MAP(CMarginsCtrl, CStatic)
	//{{AFX_MSG_MAP(CMarginsCtrl)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMarginsCtrl message handlers

void CMarginsCtrl::OnPaint() 
{
    CPaintDC dc(this);

	RECT rect;
	GetClientRect(&rect);
	PrintPageSetupMargins(dc.GetSafeHdc(),m_pageRect);
}



/////////////////////////////////////////////////////////////////////////////
// CMarginsCtrl methods
#define ID_SLIDERH 0x7891
#define ID_SLIDERV 0x7892

void CMarginsCtrl::PreSubclassWindow()
{

	CStatic::PreSubclassWindow();

	ModifyStyleEx(GetExStyle(),  WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP );
	CreateControls();
	PosCtrl();
	
}


BOOL CMarginsCtrl::CreateControls()
{
	RECT rect;
	GetClientRect(&rect);
	if(!m_sliderH->Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE  |  WS_TABSTOP, CRect(0, 0, rect.right, SLIDERWIDTH), this, ID_SLIDERH))
		  return FALSE;
 	
	if(!m_sliderV->Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | 
                    TBS_VERT | WS_TABSTOP , CRect(0, 0, SLIDERWIDTH, rect.bottom), this, ID_SLIDERV))
		  return FALSE;


	m_sliderH->EnableWindow(TRUE);
	m_sliderH->ShowWindow(SW_SHOW);


 	m_sliderV->EnableWindow(TRUE);
	m_sliderV->ShowWindow(SW_SHOW);

	return TRUE;
}


BOOL CMarginsCtrl::PosCtrl()
{

	int PAGESIZEX,PAGESIZEY,MARGINV,MARGINH,temp;

	//default
	PAGESIZEX=2100; 
	PAGESIZEY=2790; 
	MARGINV=30;
	MARGINH=30;

	/*int PageWidthDev=0,PageHeightDev=0 ;*/
	int  PrinterType=0;


	/*long LOGPInchX=0;
	long LOGPInchY=0;*/
	long PhyWidth=0;
	long PhyHeight=0;
	int  PhyOffsetX=0,PhyOffsetY=0;
	


	CDC *printDC = NULL;
	printDC = CreatePrinterDC();
	if (printDC != NULL) 
	{
		PrinterType = printDC->GetDeviceCaps(TECHNOLOGY);
		if ((PrinterType == DT_PLOTTER) || (PrinterType == DT_RASPRINTER)) 
		{

		/*	PageWidthDev = printDC->GetDeviceCaps(HORZRES);
			PageHeightDev = printDC->GetDeviceCaps(VERTRES);

			LOGPInchX = printDC->GetDeviceCaps(LOGPIXELSX);
			LOGPInchY= printDC->GetDeviceCaps(LOGPIXELSY);*/

			PhyWidth = printDC->GetDeviceCaps(PHYSICALWIDTH);
			PhyHeight = printDC->GetDeviceCaps(PHYSICALHEIGHT);

			PhyOffsetX = printDC->GetDeviceCaps(PHYSICALOFFSETX);
			PhyOffsetY = printDC->GetDeviceCaps(PHYSICALOFFSETY);

			CSize Tsize(PhyWidth,PhyHeight);
			printDC->SetMapMode(MM_LOMETRIC);
			printDC->DPtoLP(&Tsize);
			
			PAGESIZEX = Tsize.cx;
			PAGESIZEY = Tsize.cy;
		

			Tsize.SetSize(PhyOffsetX,PhyOffsetY); 

			printDC->DPtoLP(&Tsize);

			MARGINH = Tsize.cx;
			MARGINV = Tsize.cy;		
			
		}
		::DeleteDC(printDC->GetSafeHdc());
	}


	if(PAGESIZEX > PAGESIZEY)
	{
		temp = PAGESIZEX;
		PAGESIZEX = PAGESIZEY;
		PAGESIZEY = temp;

		temp = MARGINH;
		MARGINH = MARGINV;
		MARGINV = temp;
	}

	
	PtPaperSizeMMs.cx = PAGESIZEX;
	PtPaperSizeMMs.cy = PAGESIZEY;


	RtMinMarginMMs.left = MARGINH;
	RtMinMarginMMs.right = MARGINH;
	RtMinMarginMMs.top  = MARGINV;
	RtMinMarginMMs.bottom = MARGINV;

	RECT client,sliderV,sliderH;
	GetClientRect(&client);

	int cHeight = client.bottom-client.top;
	int cWidth = (cHeight*PtPaperSizeMMs.cx)/PtPaperSizeMMs.cy;
	client.right = client.left+cWidth;

//	TRACE("client left %d, top %d, right %d ,bottom %d\n",client.left,client.top,client.right,client.bottom);

		m_sliderV->GetClientRect(&sliderV);
		m_sliderH->GetClientRect(&sliderH);

		int posX = sliderV.right-sliderV.left;
		int posY = 0;
		int sizeCX = client.right-client.left-posX;
		int sizeCY = sliderH.bottom-sliderH.top;
		m_pageRect.left = posX;
	

	m_sliderH->SetWindowPos(NULL,posX,posY,sizeCX,sizeCY,0);

	posY = sizeCY;
	posX = 0;

	sizeCX = sliderV.right-sliderV.left;
	sizeCY = client.bottom-client.top-posY;

	m_pageRect.top = posY;

	m_sliderV->SetWindowPos(NULL,posX,posY,sizeCX,sizeCY,0);


	
	m_pageRect.right = client.right;
	m_pageRect.bottom = client.bottom;

	OffX = 0;
	OffY = 0;

	m_sliderV->SetRange(MINUSMARGIN,PtPaperSizeMMs.cy);
	m_sliderV->SetTicFreq(5);
	m_sliderV->SetPageSize(5);
	m_sliderH->SetRange(MINUSMARGIN,PtPaperSizeMMs.cx);
	m_sliderH->SetTicFreq(5);
	m_sliderH->SetPageSize(5);


	return TRUE;  
}

void CMarginsCtrl::PrintPageSetupMargins(HDC hDC,RECT &drawRect)
{

    RECT aRt;
	char OrgBuff[60];
   
    HGDIOBJ hPen, hBr;
//    int i;
	
	RtMinMarginMMs;
	
    FillRect(hDC, &drawRect, (HBRUSH)GetStockObject(WHITE_BRUSH));


	hPen = (HGDIOBJ)CreatePen(PS_DOT, 1, RGB(128,128,128));
    hPen = SelectObject(hDC, hPen);
	 // Rectangle() does not work here
 
	MoveToEx( hDC, drawRect.left           , drawRect.top           , NULL );
    LineTo(   hDC, drawRect.right - 1, drawRect.top              );
    MoveToEx( hDC, drawRect.left             , drawRect.top+1             , NULL );
    LineTo(   hDC, drawRect.left          , drawRect.bottom - 1 );
    DeleteObject(SelectObject(hDC, hPen));
   
    MoveToEx( hDC, drawRect.right - 1, drawRect.top            , NULL );
    LineTo(   hDC, drawRect.right - 1, drawRect.bottom - 1 );
    MoveToEx( hDC, drawRect.left           , drawRect.bottom - 1, NULL );
    LineTo(   hDC, drawRect.right    , drawRect.bottom - 1 );

	aRt = drawRect;
	aRt.left   -= ((aRt.right-aRt.left)  * MINUSMARGIN)   / PtPaperSizeMMs.cx;
	aRt.top    -= ((aRt.bottom-aRt.top) * MINUSMARGIN)    / PtPaperSizeMMs.cy;
  
	
    //hPen = (HGDIOBJ)CreatePen(PS_SOLID, 1, RGB(128,128,128));
    //hPen = SelectObject(hDC, hPen);
  
	MoveToEx( hDC, aRt.left           , aRt.top           , NULL );
    LineTo(   hDC, aRt.right - 1, aRt.top              );
    MoveToEx( hDC, aRt.left             , aRt.top+1             , NULL );
    LineTo(   hDC, aRt.left          , aRt.bottom - 1 );
   // DeleteObject(SelectObject(hDC, hPen));

    MoveToEx( hDC, aRt.right - 1, aRt.top            , NULL );
    LineTo(   hDC, aRt.right - 1, aRt.bottom - 1 );
    MoveToEx( hDC, aRt.left           , aRt.bottom - 1, NULL );
    LineTo(   hDC, aRt.right    , aRt.bottom - 1 );

    SetBkMode(hDC, TRANSPARENT);
    hPen = (HGDIOBJ)CreatePen(PS_DOT, 1, RGB(128, 128, 128));
    hPen = SelectObject(hDC, hPen);


    hBr  = (HGDIOBJ)GetStockObject(NULL_BRUSH);
    hBr  = SelectObject(hDC, hBr);


    InflateRect(&aRt, -1, -1);
 
//	WORD wFlags;

  
       /* if ( !wFlags ||
             IsDlgButtonChecked(GetParent(hWnd), ID_SETUP_R_PORTRAIT) )
        {
            //
            //  Paper in portrait.
            //
            wFlags |= 0x0004;
        }


        if (pPI->pPD->Flags & PI_WPAPER_ENVELOPE)
        {
            wFlags |= 0x0008;
            if (aRt.right < aRt.bottom)
            {
                //
                //  Envelope in portrait.
                //
                wFlags |= 0x0010;
            }
        }
    */ 

     
   

    /*aRt.left   += aRt.right  * RtMinMarginMMs.left   / PtPaperSizeMMs.cx;
    aRt.top    += aRt.bottom * RtMinMarginMMs.top    / PtPaperSizeMMs.cy;
    aRt.right  -= aRt.right  * RtMinMarginMMs.right  / PtPaperSizeMMs.cx;
    aRt.bottom -= aRt.bottom * RtMinMarginMMs.bottom / PtPaperSizeMMs.cy;*/

   
	int dOffX = ((aRt.right-aRt.left)*OffX)/PtPaperSizeMMs.cx;
	int dOffY = ((aRt.bottom-aRt.top)*OffY)/PtPaperSizeMMs.cy;
            
    //Rectangle(hDC, aRt.left, aRt.top, aRt.right, aRt.bottom);

	hPen = (HGDIOBJ)CreatePen(PS_SOLID, 1, RGB(255,0,0));
	 DeleteObject(SelectObject(hDC, hPen));

	MoveToEx(hDC,aRt.left+dOffX,aRt.top+dOffY,NULL);
	LineTo(hDC,aRt.right,aRt.top+dOffY);
    
    MoveToEx(hDC,aRt.left+dOffX,aRt.top+dOffY,NULL);
	LineTo(hDC,aRt.left+dOffX,aRt.bottom);
    


	HFONT orgFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	orgFont = (HFONT)SelectObject(hDC,orgFont);

	RECT OrgTextRc;
	SIZE textSize;
	int len;
	if(dOffX || dOffY)
	{

		sprintf(OrgBuff,"%+d.%d mm , %+d.%d mm",OffX/10,abs(OffX%10),OffY/10,abs(OffY%10));
		len = strlen(OrgBuff);
		::GetTextExtentPoint32(hDC,OrgBuff,len,&textSize);
		SetRect(&OrgTextRc,(aRt.left+aRt.right-textSize.cx)/2,(aRt.top+aRt.bottom-textSize.cy)/2,aRt.right,aRt.bottom);
		DrawText(hDC,OrgBuff,len,&OrgTextRc,0);
	}

	
   


	SelectObject(hDC,orgFont);
    DeleteObject(SelectObject(hDC, hPen));
    SelectObject(hDC, hBr);
    
}


void CMarginsCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int min,max,page;
	bool l_GoPaint = false;
	
	switch(nSBCode)
	{
	case SB_THUMBTRACK:
	case TB_LINEDOWN:
	case TB_LINEUP:
	case TB_TOP:
	case SB_THUMBPOSITION:
	
		
		OffX = nPos;
		l_GoPaint = true;
	
		break;

	case  SB_PAGEDOWN:

		max = m_sliderH->GetRangeMax();
		page = m_sliderH->GetPageSize();
		
		if((OffX + page) < max) OffX += page;
		else OffX = max;
		l_GoPaint = true;
	
		break;
	case SB_PAGEUP:

		min = m_sliderH->GetRangeMin();
		
		page = m_sliderH->GetPageSize();
	
			if((OffX - page) > min) OffX -= page;
		else OffX = min;
		l_GoPaint = true;

		break;
	}
	if(l_GoPaint)
	{
		CClientDC dc(this);
		PrintPageSetupMargins(dc.GetSafeHdc(),m_pageRect);
		TRACE("OffX = %d\n",OffX);
	}


}

void CMarginsCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int min,max,page;
	bool l_GoPaint = false;
	
	switch(nSBCode)
	{
	case SB_THUMBTRACK:
	case TB_LINEDOWN:
	case TB_LINEUP:
	case TB_TOP:
	case SB_THUMBPOSITION:
	
		OffY = nPos;
		l_GoPaint = true;
		break;

	case  SB_PAGEDOWN:

		max = m_sliderV->GetRangeMax();
		page = m_sliderV->GetPageSize();
	
		
		if((OffY + page) < max) OffY += page;
		else OffY = max;
		l_GoPaint = true;
		break;
	case SB_PAGEUP:

		min = m_sliderV->GetRangeMin();
		
		page = m_sliderV->GetPageSize();

			if((OffY - page) > min) OffY -= page;
		else OffY = min;
	l_GoPaint = true;

		break;
	}

	if(l_GoPaint)
	{
		CClientDC dc(this);
		PrintPageSetupMargins(dc.GetSafeHdc(),m_pageRect);
		TRACE("OffY = %d\n",OffY);
	}


}
bool CMarginsCtrl::SetOffX(int sOffX)
{
if( sOffX >= MINUSMARGIN && sOffX <= PtPaperSizeMMs.cx)
{
	OffX = sOffX;
	m_sliderH->SetPos(OffX);
	Invalidate();
	return true;
}
return false;
}
bool CMarginsCtrl::SetOffY(int sOffY)
{

if( sOffY >= MINUSMARGIN && sOffY <= PtPaperSizeMMs.cy)
{
	OffY = sOffY;
	m_sliderV->SetPos(OffY);
	Invalidate();
	return true;
}
return false;

}


CDC* CMarginsCtrl::CreatePrinterDC()
{
   PRINTDLG PrtDlg;
   HDC      hDC;
   
   if (!AfxGetApp()->GetPrinterDeviceDefaults(&PrtDlg))
   {
       
		return NULL;

   } 
   else
   {
      CPrintDialog dlg(FALSE); 

      dlg.m_pd.hDevMode = PrtDlg.hDevMode;
      dlg.m_pd.hDevNames = PrtDlg.hDevNames;
    
      hDC = dlg.CreatePrinterDC();
   }
      
   CDC* pDC = CDC::FromHandle(hDC);
   pDC->m_bPrinting = TRUE;              
   return pDC;        
}

