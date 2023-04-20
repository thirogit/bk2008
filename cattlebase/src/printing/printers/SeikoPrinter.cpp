#include "stdafx.h"
#include "SeikoPrinter.h"
#include "../CowBasePrintDoc.h"

#define DPUS445_PAGEWIDTH_DEVUNITS 832


CSeikoPrinter::CSeikoPrinter(const CString& sPrinterName,const CString& sPrinterAddress,PrinterConnectionType connectionType,const CString& sDriverName) : 
		CPrinter(sPrinterName,sPrinterAddress,connectionType,sDriverName)
{

}
CSeikoPrinter::~CSeikoPrinter()
{

}

UINT CSeikoPrinter::GetPageWidth() const
{
	return DPUS445_PAGEWIDTH_DEVUNITS;
}

void CSeikoPrinter::DoPrint(CBaseDocView* pDocView,UINT nNumOfCopies)
{
	CCowBasePrintDoc* pDoc = pDocView->GetDocument();
	
	CSize paperSizeMM10 = pDoc->GetPaperSize();
	HDC hScreenDC = ::GetDC(NULL);
	CDC ScreenDC;
	ScreenDC.Attach(hScreenDC);

	int cxPixelsPerInch = 200;//ScreenDC.GetDeviceCaps(LOGPIXELSX);
	int cyPixelsPerInch = 200;//ScreenDC.GetDeviceCaps(LOGPIXELSY);

	int W =  MulDiv(paperSizeMM10.cx,cxPixelsPerInch,254);
	int H = MulDiv(paperSizeMM10.cy,cyPixelsPerInch,254);
	CBitmap bm;	
	bm.CreateCompatibleBitmap(&ScreenDC,W,H);

	CDC DCGlyphs;
	DCGlyphs.CreateCompatibleDC(&ScreenDC);
	CBitmap* bmOld = DCGlyphs.SelectObject(&bm);
	DCGlyphs.m_bPrinting = TRUE;
	CPrintInfo printInfo; 

	pDocView->OnBeginPrinting(&DCGlyphs,&printInfo);
	pDocView->OnPrepareDC(&DCGlyphs,NULL);


	CSize vport = DCGlyphs.GetViewportExt();
	DCGlyphs.SetViewportExt(MulDiv(vport.cx,200,96),
		                MulDiv(vport.cy,200,96));



	int nNumOfPages = pDoc->GetPrintPages();
	printInfo.SetMinPage(1);
	printInfo.SetMaxPage(nNumOfPages);

	fipWinImage image;

	for(UINT iPage = printInfo.GetMinPage();iPage <= printInfo.GetMaxPage();iPage++)
	{
		printInfo.m_nCurPage = iPage;
		pDocView->OnPrint(&DCGlyphs,&printInfo);
		
		image.copyFromBitmap((HBITMAP)bm);
		image.convertTo8Bits();

		RotateBestFit(image);
	
		for(UINT nCopy = 0;nCopy < nNumOfCopies;nCopy++)
		{
			WriteImage(image);
			FeedPaper(15);
		}

	}

	pDocView->OnEndPrinting(&DCGlyphs,&printInfo);

	DCGlyphs.SelectObject(bmOld);
	DCGlyphs.DeleteDC();
	
	ScreenDC.Detach();
	ReleaseDC(NULL,hScreenDC);

}

void CSeikoPrinter::RotateBestFit(fipImage& image)
{
	UINT X = image.getWidth();
	UINT Y = image.getHeight();
	UINT paperW = GetPageWidth();

	if(X > Y)
	{
		if(X <= paperW)
		{
			return; 
		}
		else
		{
			if(Y <= paperW)
			{
				image.rotate(-90.0);
			}
			else
			{
				if((Y-paperW) < (X-paperW))
				{
					image.rotate(-90.0);
				}
				else
				{
					return;
				}
			}
		}
	}
	else // X < Y
	{
		if(Y <= paperW)
		{
			image.rotate(-90.0);
		}
	}

}

bool CSeikoPrinter::WriteImage(fipImage& image)
{
	if(image.getBitsPerPixel() != 8)
		return false;

	DWORD	cbWritten	= 0;
	WORD xDots = image.getWidth();
	WORD yDots = image.getHeight();
    if ((xDots >= 0x10000) || (yDots > 0xfa0))
    {
       return false;
    }
    //if ((xDots > 0x340))
   // {
    //   return false;
   // }
	BYTE buf[] = { 0x12, 0x3d, 0, 0x1d, 0x76, 0x30, 0, 0, 0, 0, 0 };
	
	int num = 0;
	int length = (xDots + 7) / 8;               
	int wrote = 0;
	int num8 = 0x2080 / length;
	if (num8 > 400)
	{
		num8 = 400;
	}
	int num5 = ((yDots + num8) - 1) / num8;
	BYTE *lines = new BYTE[length];
                   
	for (int i = 0; i < num5; i++)
	{
		int num7 = (yDots > num8) ? num8 : yDots;
		buf[7] = (BYTE)(length % 0x100);
		buf[8] = (BYTE)(length / 0x100);
		buf[9] = (BYTE)(num7 % 0x100);
		buf[10] = (BYTE)(num7 / 0x100);

		WritePrinter(buf, sizeof(buf));
		
		for (int j = i * num8; j < ((i * num8) + num7); j++)
		{
			memset(lines,0,length);
			BYTE* scanLine = image.getScanLine(image.getHeight()-1-j);
			for(int jj = 0;jj < image.getWidth() && jj < GetPageWidth();jj++)
			{
				int bit = jj%8;
				if(scanLine[jj] < 180)
				{
					lines[jj/8] |= (1 << bit);
				}
			}
			WritePrinter(lines, length);
		}
		
		yDots -= num8;
		//Sleep(200);
	}
	delete[] lines;   
	return true;
}


void CSeikoPrinter::FeedPaper(short px)
{
	BYTE    feedPaperCmd[3] = {0x1B,0x4A, (BYTE)px};
	WritePrinter(feedPaperCmd,sizeof(feedPaperCmd));
}



