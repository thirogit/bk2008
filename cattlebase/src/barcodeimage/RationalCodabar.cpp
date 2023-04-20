// RationalCodabar.cpp: implementation of the CRationalCodabar class.
//
//	Copyright 2002 Neil Van Eps
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RationalCodabar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRationalCodabar::CRationalCodabar()
{
	// rational codabar
	m_nSymbology = RATIONALCODABAR;
}

CRationalCodabar::~CRationalCodabar()
{

}

////////////////////////////////////////////////////////////////////////////////////
//
//	Name:
//		DrawBitmap()
//
//	Description:
//		draws a barcode using the previously loaded data
//
//	Arguments:
//		none
//
//	Return:
//		void
//
//	Called by:
//		public class interface
//
////////////////////////////////////////////////////////////////////////////////////

void CRationalCodabar::DrawBitmap()
{
	int		i;

	// draw each character in the message
	for (i=0;i<m_csMessage.GetLength();i++)
		DrawPattern(RetrievePattern(m_csMessage.GetAt(i)));

	return;
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Name:
//		DrawPattern()
//
//	Description:
//		draws the passed character pattern at the end of the barcode
//
//	Arguments:
//		CString		csPattern	-	the bar pattern to draw
//
//	Return:
//		void
//
//	Called by:
//		CRationalCodabar::DrawBitmap()
//
////////////////////////////////////////////////////////////////////////////////////

void CRationalCodabar::DrawPattern(CString csPattern)
{
	int			i,nXPixel,nYPixel,nTempWidth;
	CDC			oDC;

	// attach to the device context
	oDC.Attach(m_hDC);

	// initialize X pixel value
	nXPixel = m_nStartingXPixel;
	
	for (i=0;i<csPattern.GetLength();i++)
	{
		// decide if narrow or wide bar
		if (csPattern.GetAt(i)=='n')
			nTempWidth = m_nNarrowBarPixelWidth;
		else
			nTempWidth = m_nWideBarPixelWidth;
		
		// X value for loop
		for (nXPixel=m_nStartingXPixel;nXPixel<m_nStartingXPixel+nTempWidth;nXPixel++)
		{
			// Y value for loop
			for (nYPixel=m_nStartingYPixel;nYPixel<m_nStartingYPixel+m_nPixelHeight;nYPixel++)
			{
				// if this is a bar
				if (i%2==0)
					oDC.SetPixelV(nXPixel,nYPixel,COLORBLACK);
				else
					oDC.SetPixelV(nXPixel,nYPixel,COLORWHITE);
			}
		}

		// advance the starting position
		m_nStartingXPixel+= nTempWidth;
	}

	// detach from the device context
	oDC.Detach();
	
	return;
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Name:
//		RetrievePattern()
//
//	Description:
//		retrieves the bar pattern for a given character
//
//	Arguments:
//		char	cInputCharacter	-	the input character to get the bar pattern for
//
//	Return:
//		CString	-	the bar pattern for the input character
//
//	Called by:
//		CRationalCodabar::DrawBitmap()
//
////////////////////////////////////////////////////////////////////////////////////

CString CRationalCodabar::RetrievePattern(char c)
{
	CString csCharPattern;
	
	if (c == '0')
		csCharPattern = "nnnnnwwn";
	else if (c == '1')
		csCharPattern = "nnnnwwnn";
	else if (c == '2')
		csCharPattern = "nnnwnnwn";
	else if (c == '3')
		csCharPattern = "wwnnnnnn";
	else if (c == '4')
		csCharPattern = "nnwnnwnn";
	else if (c == '5')
		csCharPattern = "wnnnnwnn";
	else if (c == '6')
		csCharPattern = "nwnnnnwn";
	else if (c == '7')
		csCharPattern = "nwnnwnnn";
	else if (c == '8')
		csCharPattern = "nwwnnnnn";
	else if (c == '9')
		csCharPattern = "wnnwnnnn";
	else if (c == '-')
		csCharPattern = "nnnwwnnn";
	else if (c == '$')
		csCharPattern = "nnwwnnnn";
	else if (c == ':')
		csCharPattern = "wnnnwnwn";
	else if (c == '/')
		csCharPattern = "wnwnnnwn";
	else if (c == '.')
		csCharPattern = "wnwnwnnn";
	else if (c == '+')
		csCharPattern = "nnwnwnwn";
	else if (c == 'A')
		csCharPattern = "nnwwnwnn";
	else if (c == 'B')
		csCharPattern = "nwnwnnwn";
	else if (c == 'C')
		csCharPattern = "nnnwnwwn";
	else if (c == 'D')
		csCharPattern = "nnnwwwnn";

	return csCharPattern;
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Name:
//		BitmapToClipboard()
//
//	Description:
//		puts the specified bitmap on the clipboard
//
//	Arguments:
//		none
//
//	Return:
//		void
//
//	Called by:
//		public class interface - called by users of the class
//
////////////////////////////////////////////////////////////////////////////////////

void CRationalCodabar::BitmapToClipboard()
{
	CDC					memDC;
	CBitmap				oBitmap;

	memDC.CreateCompatibleDC(NULL);

	m_hDC = memDC.GetSafeHdc();

	// create compatible, correctly sized bitmap
	oBitmap.CreateCompatibleBitmap(&memDC,m_nFinalBarcodePixelWidth,m_nPixelHeight);

	// select our bitmap into the device context
	CBitmap * oldbm = memDC.SelectObject(&oBitmap);

	// turn area white - stock black bitmap is selected
	memDC.BitBlt(0,0,m_nFinalBarcodePixelWidth,m_nPixelHeight,NULL,0,0,WHITENESS);

	// draw bitmap into memory device context
	DrawBitmap();
	
	// put bitmap on clipboard
	::OpenClipboard(NULL);
	::EmptyClipboard();
	::SetClipboardData(CF_BITMAP, oBitmap.m_hObject);
	::CloseClipboard();
		
	//	deselect object out of device context
	memDC.SelectObject(oldbm);

	// make sure bitmap not deleted with CBitmap object		
	oBitmap.Detach();

	return;
}
