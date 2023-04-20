// I2of5.cpp: implementation of the CI2of5 class.
//
//	Copyright 2002 Neil Van Eps
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "I2of5.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CI2of5::CI2of5()
{
	// I2of5
	m_nSymbology = I2OF5;
}

CI2of5::~CI2of5()
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

void CI2of5::DrawBitmap()
{
	int		i,nNumber;

	// draw the start character
	DrawPattern("nnnn");

	// for each character in the message
	for (i=0;i<m_csMessage.GetLength();i+=2)
	{
		// retrieve the next two digit number
		nNumber = m_csMessage.GetAt(i) - '0';
		nNumber = nNumber * 10;
		nNumber += m_csMessage.GetAt(i+1) - '0';

		// draw the two digit number
		DrawPattern(RetrievePattern(nNumber));
	}

	// draw the stop character
	DrawPattern("wnn");

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

void CI2of5::DrawPattern(CString csCharPattern)
{
	int			i,nXPixel,nYPixel,nTempWidth;
	CDC			oDC;

	// attach to the device context
	oDC.Attach(m_hDC);

	// initialize X pixel value
	nXPixel = m_nStartingXPixel;
	
	for (i=0;i<csCharPattern.GetLength();i++)
	{
		// decide if narrow or wide bar
		if (csCharPattern.GetAt(i)=='n')
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

CString CI2of5::RetrievePattern(int nTwoDigitNumber)
{
	CString	csCharPattern;
	switch (nTwoDigitNumber)
	{
		case 0:
			csCharPattern = "nnnnwwwwnn";
			break;
		case 1:
			csCharPattern = "nwnnwnwnnw";
			break;
		case 2:
			csCharPattern = "nnnwwnwnnw";
			break;
		case 3:
			csCharPattern = "nwnwwnwnnn";
			break;
		case 4:
			csCharPattern = "nnnnwwwnnw";
			break;
		case 5:
			csCharPattern = "nwnnwwwnnn";
			break;
		case 6:
			csCharPattern = "nnnwwwwnnn";
			break;
		case 7:
			csCharPattern = "nnnnwnwwnw";
			break;
		case 8:
			csCharPattern = "nwnnwnwwnn";
			break;
		case 9:
			csCharPattern = "nnnwwnwwnn";
			break;
		case 10:
			csCharPattern = "wnnnnwnwwn";
			break;
		case 11:
			csCharPattern = "wwnnnnnnww";
			break;
		case 12:
			csCharPattern = "wnnwnnnnww";
			break;
		case 13:
			csCharPattern = "wwnwnnnnwn";
			break;
		case 14:
			csCharPattern = "wnnnnwnnww";
			break;
		case 15:
			csCharPattern = "wwnnnwnnwn";
			break;
		case 16:
			csCharPattern = "wnnwnwnnwn";
			break;
		case 17:
			csCharPattern = "wnnnnnnwww";
			break;
		case 18:
			csCharPattern = "wwnnnnnwwn";
			break;
		case 19:
			csCharPattern = "wnnwnnnwwn";
			break;
		case 20:
			csCharPattern = "nnwnnwnwwn";
			break;
		case 21:
			csCharPattern = "nwwnnnnnww";
			break;
		case 22:
			csCharPattern = "nnwwnnnnww";
			break;
		case 23:
			csCharPattern = "nwwwnnnnwn";
			break;
		case 24:
			csCharPattern = "nnwnnwnnww";
			break;
		case 25:
			csCharPattern = "nwwnnwnnwn";
			break;
		case 26:
			csCharPattern = "nnwwnwnnwn";
			break;
		case 27:
			csCharPattern = "nnwnnnnwww";
			break;
		case 28:
			csCharPattern = "nwwnnnnwwn";
			break;
		case 29:
			csCharPattern = "nnwwnnnwwn";
			break;
		case 30:
			csCharPattern = "wnwnnwnwnn";
			break;
		case 31:
			csCharPattern = "wwwnnnnnnw";
			break;
		case 32:
			csCharPattern = "wnwwnnnnnw";
			break;
		case 33:
			csCharPattern = "wwwwnnnnnn";
			break;
		case 34:
			csCharPattern = "wnwnnwnnnw";
			break;
		case 35:
			csCharPattern = "wwwnnwnnnn";
			break;
		case 36:
			csCharPattern = "wnwwnwnnnn";
			break;
		case 37:
			csCharPattern = "wnwnnnnwnw";
			break;
		case 38:
			csCharPattern = "wwwnnnnwnn";
			break;
		case 39:
			csCharPattern = "wnwwnnnwnn";
			break;
		case 40:
			csCharPattern = "nnnnwwnwwn";
			break;
		case 41:
			csCharPattern = "nwnnwnnnww";
			break;
		case 42:
			csCharPattern = "nnnwwnnnww";
			break;
		case 43:
			csCharPattern = "nwnwwnnnwn";
			break;
		case 44:
			csCharPattern = "nnnnwwnnww";
			break;
		case 45:
			csCharPattern = "nwnnwwnnwn";
			break;
		case 46:
			csCharPattern = "nnnwwwnnwn";
			break;
		case 47:
			csCharPattern = "nnnnwnnwww";
			break;
		case 48:
			csCharPattern = "nwnnwnnwwn";
			break;
		case 49:
			csCharPattern = "nnnwwnnwwn";
			break;
		case 50:
			csCharPattern = "wnnnwwnwnn";
			break;
		case 51:
			csCharPattern = "wwnnwnnnnw";
			break;
		case 52:
			csCharPattern = "wnnwwnnnnw";
			break;
		case 53:
			csCharPattern = "wwnwwnnnnn";
			break;
		case 54:
			csCharPattern = "wnnnwwnnnw";
			break;
		case 55:
			csCharPattern = "wwnnwwnnnn";
			break;
		case 56:
			csCharPattern = "wnnwwwnnnn";
			break;
		case 57:
			csCharPattern = "wnnnwnnwnw";
			break;
		case 58:
			csCharPattern = "wwnnwnnwnn";
			break;
		case 59:
			csCharPattern = "wnnwwnnwnn";
			break;
		case 60:
			csCharPattern = "nnwnwwnwnn";
			break;
		case 61:
			csCharPattern = "nwwnwnnnnw";
			break;
		case 62:
			csCharPattern = "nnwwwnnnnw";
			break;
		case 63:
			csCharPattern = "nwwwwnnnnn";
			break;
		case 64:
			csCharPattern = "nnwnwwnnnw";
			break;
		case 65:
			csCharPattern = "nwwnwwnnnn";
			break;
		case 66:
			csCharPattern = "nnwwwwnnnn";
			break;
		case 67:
			csCharPattern = "nnwnwnnwnw";
			break;
		case 68:
			csCharPattern = "nwwnwnnwnn";
			break;
		case 69:
			csCharPattern = "nnwwwnnwnn";
			break;
		case 70:
			csCharPattern = "nnnnnwwwwn";
			break;
		case 71:
			csCharPattern = "nwnnnnwnww";
			break;
		case 72:
			csCharPattern = "nnnwnnwnww";
			break;
		case 73:
			csCharPattern = "nwnwnnwnwn";
			break;
		case 74:
			csCharPattern = "nnnnnwwnww";
			break;
		case 75:
			csCharPattern = "nwnnnwwnwn";
			break;
		case 76:
			csCharPattern = "nnnwnwwnwn";
			break;
		case 77:
			csCharPattern = "nnnnnnwwww";
			break;
		case 78:
			csCharPattern = "nwnnnnwwwn";
			break;
		case 79:
			csCharPattern = "nnnwnnwwwn";
			break;
		case 80:
			csCharPattern = "wnnnnwwwnn";
			break;
		case 81:
			csCharPattern = "wwnnnnwnnw";
			break;
		case 82:
			csCharPattern = "wnnwnnwnnw";
			break;
		case 83:
			csCharPattern = "wwnwnnwnnn";
			break;
		case 84:
			csCharPattern = "wnnnnwwnnw";
			break;
		case 85:
			csCharPattern = "wwnnnwwnnn";
			break;
		case 86:
			csCharPattern = "wnnwnwwnnn";
			break;
		case 87:
			csCharPattern = "wnnnnnwwnw";
			break;
		case 88:
			csCharPattern = "wwnnnnwwnn";
			break;
		case 89:
			csCharPattern = "wnnwnnwwnn";
			break;
		case 90:
			csCharPattern = "nnwnnwwwnn";
			break;
		case 91:
			csCharPattern = "nwwnnnwnnw";
			break;
		case 92:
			csCharPattern = "nnwwnnwnnw";
			break;
		case 93:
			csCharPattern = "nwwwnnwnnn";
			break;
		case 94:
			csCharPattern = "nnwnnwwnnw";
			break;
		case 95:
			csCharPattern = "nwwnnwwnnn";
			break;
		case 96:
			csCharPattern = "nnwwnwwnnn";
			break;
		case 97:
			csCharPattern = "nnwnnnwwnw";
			break;
		case 98:
			csCharPattern = "nwwnnnwwnn";
			break;
		case 99:
			csCharPattern = "nnwwnnwwnn";
			break;
	}

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

void CI2of5::BitmapToClipboard()
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
