// Code93.cpp: implementation of the CCode93 class.
//
//	Copyright 2002 Neil Van Eps
//
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Code93.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCode93::CCode93()
{
	// code 93
	m_nSymbology = CODE93;
}

CCode93::~CCode93()
{

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
//		public class interface
//
////////////////////////////////////////////////////////////////////////////////////

void CCode93::BitmapToClipboard()
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

void CCode93::DrawBitmap()
{
	long		i,nFirstNumber,nSecondNumber;
	CString		csCurrentPattern;

	// draw start character
	DrawPattern(RetrievePattern(47));
	
	// draw each character in the message
	for (i=0;i<m_csMessage.GetLength();i++)
	{
		// get current ASCII character
		ASCIItoCode93Sequence((long)m_csMessage.GetAt(i),&nFirstNumber,&nSecondNumber);

		DrawPattern(RetrievePattern(nFirstNumber));
		if (nSecondNumber!=-1)
			DrawPattern(RetrievePattern(nSecondNumber));
	}

	// add the check digit characters
	DrawCheckDigits();

	// draw stop character
	DrawPattern(RetrievePattern(48));

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

void CCode93::DrawPattern( CString csPattern )
{
	int			i,nXPixel,nYPixel;
	CDC			oDC;

	// attach to the device context
	oDC.Attach(m_hDC);

	// initialize X pixel value
	nXPixel = m_nStartingXPixel;
	
	for (i=0;i<csPattern.GetLength();i++)
	{
		// X value for loop
		for (nXPixel=m_nStartingXPixel;nXPixel<m_nStartingXPixel+m_nNarrowBarPixelWidth;nXPixel++)
		{
			// Y value for loop
			for (nYPixel=m_nStartingYPixel;nYPixel<m_nStartingYPixel+m_nPixelHeight;nYPixel++)
			{
				// if this is a bar
				if (csPattern.GetAt(i)=='b')
					oDC.SetPixelV(nXPixel,nYPixel,COLORBLACK);
				else
					oDC.SetPixelV(nXPixel,nYPixel,COLORWHITE);
			}
		}

		// advance the starting position
		m_nStartingXPixel+= m_nNarrowBarPixelWidth;
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
//		CCode93::DrawBitmap()
//
////////////////////////////////////////////////////////////////////////////////////

CString	CCode93::RetrievePattern(long c)
{
	CString	csCharPattern;

	// Code93 characters follow are all 9 narrow bar elements wide
	// b - bar element
	// s - space element
	// 9 elements in all

	switch (c)
	{
		case 0:
			csCharPattern = "bsssbsbss";
			break;
		case 1:
			csCharPattern = "bsbssbsss";
			break;
		case 2:
			csCharPattern = "bsbsssbss";
			break;
		case 3:
			csCharPattern = "bsbssssbs";
			break;
		case 4:
			csCharPattern = "bssbsbsss";
			break;
		case 5:
			csCharPattern = "bssbssbss";
			break;
		case 6:
			csCharPattern = "bssbsssbs";
			break;
		case 7:
			csCharPattern = "bsbsbssss";
			break;
		case 8:
			csCharPattern = "bsssbssbs";
			break;
		case 9:
			csCharPattern = "bssssbsbs";
			break;
		case 10:
			csCharPattern = "bbsbsbsss";
			break;
		case 11:
			csCharPattern = "bbsbssbss";
			break;
		case 12:
			csCharPattern = "bbsbsssbs";
			break;
		case 13:
			csCharPattern = "bbssbsbss";
			break;
		case 14:
			csCharPattern = "bbssbssbs";
			break;
		case 15:
			csCharPattern = "bbsssbsbs";
			break;
		case 16:
			csCharPattern = "bsbbsbsss";
			break;
		case 17:
			csCharPattern = "bsbbssbss";
			break;
		case 18:
			csCharPattern = "bsbbsssbs";
			break;
		case 19:
			csCharPattern = "bssbbsbss";
			break;
		case 20:
			csCharPattern = "bsssbbsbs";
			break;
		case 21:
			csCharPattern = "bsbsbbsss";
			break;
		case 22:
			csCharPattern = "bsbssbbss";
			break;
		case 23:
			csCharPattern = "bsbsssbbs";
			break;
		case 24:
			csCharPattern = "bssbsbbss";
			break;
		case 25:
			csCharPattern = "bsssbsbbs";
			break;
		case 26:
			csCharPattern = "bbsbbsbss";
			break;
		case 27:
			csCharPattern = "bbsbbssbs";
			break;
		case 28:
			csCharPattern = "bbsbsbbss";
			break;
		case 29:
			csCharPattern = "bbsbssbbs";
			break;
		case 30:
			csCharPattern = "bbssbsbbs";
			break;
		case 31:
			csCharPattern = "bbssbbsbs";
			break;
		case 32:
			csCharPattern = "bsbbsbbss";
			break;
		case 33:
			csCharPattern = "bsbbssbbs";
			break;
		case 34:
			csCharPattern = "bssbbsbbs";
			break;
		case 35:
			csCharPattern = "bssbbbsbs";
			break;
		case 36:
			csCharPattern = "bssbsbbbs";
			break;
		case 37:
			csCharPattern = "bbbsbsbss";
			break;
		case 38:
			csCharPattern = "bbbsbssbs";
			break;
		case 39:
			csCharPattern = "bbbssbsbs";
			break;
		case 40:
			csCharPattern = "bsbbsbbbs";
			break;
		case 41:
			csCharPattern = "bsbbbsbbs";
			break;
		case 42:
			csCharPattern = "bbsbsbbbs";
			break;
		case 43:
			csCharPattern = "bssbssbbs";
			break;
		case 44:
			csCharPattern = "bbbsbbsbs";
			break;
		case 45:
			csCharPattern = "bbbsbsbbs";
			break;
		case 46:
			csCharPattern = "bssbbssbs";
			break;
		case 47:
			csCharPattern = "bsbsbbbbs";
			break;
		case 48:
			csCharPattern = "bsbsbbbbsb";
			break;
	}

	return csCharPattern;
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Name:
//		AddCheckDigitsToMessage()
//
//	Description:
//		Adds the two code93 check digits to the end of the message
//
//	Arguments:
//		none
//
//	Return:
//		void
//
//	Called by:
//		CCode93::DrawBitmap()
//
////////////////////////////////////////////////////////////////////////////////////

void CCode93::DrawCheckDigits()
{
	long	i,nSum,nWeight,nFirstNumber,nSecondNumber;
	
	// "C" check digit character
	nWeight = 1;
	nSum = 0;
	for (i=m_csMessage.GetLength()-1;i>-1;i--)
	{
		ASCIItoCode93Sequence((long)m_csMessage.GetAt(i),&nFirstNumber,&nSecondNumber);

		// add to the sum
		nSum += (nWeight * nFirstNumber);
		nWeight++;
		if (nWeight>20)
			nWeight=1;

		// if its a two sequence character
		if (nSecondNumber!=-1)
		{
			nSum += (nWeight * nSecondNumber);
			nWeight++;
			if (nWeight>20)
				nWeight=1;
		}
	}

	// draw the "C" check digit character
	DrawPattern(RetrievePattern(nSum%47));
	
	// "K" check digit character - include the "C" check digit character in calculations
	nWeight = 2;
	nSum = nSum%47;
	for (i=m_csMessage.GetLength()-1;i>-1;i--)
	{
		ASCIItoCode93Sequence((long)m_csMessage.GetAt(i),&nFirstNumber,&nSecondNumber);

		// add to the sum
		nSum += (nWeight * nFirstNumber);
		nWeight++;
		if (nWeight>15)
			nWeight=1;

		// if its a two sequence character
		if (nSecondNumber!=-1)
		{
			nSum += (nWeight * nSecondNumber);
			nWeight++;
			if (nWeight>15)
				nWeight=1;
		}
	}

	// draw the "K" check digit character
	DrawPattern(RetrievePattern(nSum%47));

	return;
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Name:
//		ASCIItoCode93Sequence()
//
//	Description:
//		translates an ASCII character into the appropriate CODE93 sequence
//
//	Arguments:
//		long	nFirstNumber	-	first number of the code93 sequence
//		long	nSecondNumber	-	second number of the code93 sequence, -1 if not applicable
//
//	Return:
//		void
//
//	Called by:
//		CCode93::DrawBitmap(), CCode93::DrawCheckDigits()
//
////////////////////////////////////////////////////////////////////////////////////

void CCode93::ASCIItoCode93Sequence(long nASCIINumber,long *nFirstNumber, long *nSecondNumber)
{
	switch (nASCIINumber)
	{
		case 0:
			*nFirstNumber = 44;
			*nSecondNumber = 30;
			break;
		case 1:
			*nFirstNumber = 43;
			*nSecondNumber = 10;
			break;
		case 2:
			*nFirstNumber = 43;
			*nSecondNumber = 11;
			break;
		case 3:
			*nFirstNumber = 43;
			*nSecondNumber = 12;
			break;
		case 4:
			*nFirstNumber = 43;
			*nSecondNumber = 13;
			break;
		case 5:
			*nFirstNumber = 43;
			*nSecondNumber = 14;
			break;
		case 6:
			*nFirstNumber = 43;
			*nSecondNumber = 15;
			break;
		case 7:
			*nFirstNumber = 43;
			*nSecondNumber = 16;
			break;
		case 8:
			*nFirstNumber = 43;
			*nSecondNumber = 17;
			break;
		case 9:
			*nFirstNumber = 43;
			*nSecondNumber = 18;
			break;
		case 10:
			*nFirstNumber = 43;
			*nSecondNumber = 19;
			break;
		case 11:
			*nFirstNumber = 43;
			*nSecondNumber = 20;
			break;
		case 12:
			*nFirstNumber = 43;
			*nSecondNumber = 21;
			break;
		case 13:
			*nFirstNumber = 43;
			*nSecondNumber = 22;
			break;
		case 14:
			*nFirstNumber = 43;
			*nSecondNumber = 23;
			break;
		case 15:
			*nFirstNumber = 43;
			*nSecondNumber = 24;
			break;
		case 16:
			*nFirstNumber = 43;
			*nSecondNumber = 25;
			break;
		case 17:
			*nFirstNumber = 43;
			*nSecondNumber = 26;
			break;
		case 18:
			*nFirstNumber = 43;
			*nSecondNumber = 27;
			break;
		case 19:
			*nFirstNumber = 43;
			*nSecondNumber = 28;
			break;
		case 20:
			*nFirstNumber = 43;
			*nSecondNumber = 29;
			break;
		case 21:
			*nFirstNumber = 43;
			*nSecondNumber = 30;
			break;
		case 22:
			*nFirstNumber = 43;
			*nSecondNumber = 31;
			break;
		case 23:
			*nFirstNumber = 43;
			*nSecondNumber = 32;
			break;
		case 24:
			*nFirstNumber = 43;
			*nSecondNumber = 33;
			break;
		case 25:
			*nFirstNumber = 43;
			*nSecondNumber = 34;
			break;
		case 26:
			*nFirstNumber = 43;
			*nSecondNumber = 35;
			break;
		case 27:
			*nFirstNumber = 44;
			*nSecondNumber = 10;
			break;
		case 28:
			*nFirstNumber = 44;
			*nSecondNumber = 11;
			break;
		case 29:
			*nFirstNumber = 44;
			*nSecondNumber = 12;
			break;
		case 30:
			*nFirstNumber = 44;
			*nSecondNumber = 13;
			break;
		case 31:
			*nFirstNumber = 44;
			*nSecondNumber = 14;
			break;
		case 32:
			*nFirstNumber = 38;
			*nSecondNumber = -1;
			break;
		case 33:
			*nFirstNumber = 45;
			*nSecondNumber = 10;
			break;
		case 34:
			*nFirstNumber = 45;
			*nSecondNumber = 11;
			break;
		case 35:
			*nFirstNumber = 45;
			*nSecondNumber = 12;
			break;
		case 36:
			*nFirstNumber = 39;
			*nSecondNumber = -1;
			break;
		case 37:
			*nFirstNumber = 42;
			*nSecondNumber = -1;
			break;
		case 38:
			*nFirstNumber = 45;
			*nSecondNumber = 15;
			break;
		case 39:
			*nFirstNumber = 45;
			*nSecondNumber = 16;
			break;
		case 40:
			*nFirstNumber = 45;
			*nSecondNumber = 17;
			break;
		case 41:
			*nFirstNumber = 45;
			*nSecondNumber = 18;
			break;
		case 42:
			*nFirstNumber = 45;
			*nSecondNumber = 19;
			break;
		case 43:
			*nFirstNumber = 41;
			*nSecondNumber = -1;
			break;
		case 44:
			*nFirstNumber = 45;
			*nSecondNumber = 21;
			break;
		case 45:
			*nFirstNumber = 36;
			*nSecondNumber = -1;
			break;
		case 46:
			*nFirstNumber = 37;
			*nSecondNumber = -1;
			break;
		case 47:
			*nFirstNumber = 40;
			*nSecondNumber = -1;
			break;
		case 48:
			*nFirstNumber = 0;
			*nSecondNumber = -1;
			break;
		case 49:
			*nFirstNumber = 1;
			*nSecondNumber = -1;
			break;
		case 50:
			*nFirstNumber = 2;
			*nSecondNumber = -1;
			break;
		case 51:
			*nFirstNumber = 3;
			*nSecondNumber = -1;
			break;
		case 52:
			*nFirstNumber = 4;
			*nSecondNumber = -1;
			break;
		case 53:
			*nFirstNumber = 5;
			*nSecondNumber = -1;
			break;
		case 54:
			*nFirstNumber = 6;
			*nSecondNumber = -1;
			break;
		case 55:
			*nFirstNumber = 7;
			*nSecondNumber = -1;
			break;
		case 56:
			*nFirstNumber = 8;
			*nSecondNumber = -1;
			break;
		case 57:
			*nFirstNumber = 9;
			*nSecondNumber = -1;
			break;
		case 58:
			*nFirstNumber = 45;
			*nSecondNumber = 35;
			break;
		case 59:
			*nFirstNumber = 44;
			*nSecondNumber = 15;
			break;
		case 60:
			*nFirstNumber = 44;
			*nSecondNumber = 16;
			break;
		case 61:
			*nFirstNumber = 44;
			*nSecondNumber = 17;
			break;
		case 62:
			*nFirstNumber = 44;
			*nSecondNumber = 18;
			break;
		case 63:
			*nFirstNumber = 44;
			*nSecondNumber = 19;
			break;
		case 64:
			*nFirstNumber = 44;
			*nSecondNumber = 31;
			break;
		case 65:
			*nFirstNumber = 10;
			*nSecondNumber = -1;
			break;
		case 66:
			*nFirstNumber = 11;
			*nSecondNumber = -1;
			break;
		case 67:
			*nFirstNumber = 12;
			*nSecondNumber = -1;
			break;
		case 68:
			*nFirstNumber = 13;
			*nSecondNumber = -1;
			break;
		case 69:
			*nFirstNumber = 14;
			*nSecondNumber = -1;
			break;
		case 70:
			*nFirstNumber = 15;
			*nSecondNumber = -1;
			break;
		case 71:
			*nFirstNumber = 16;
			*nSecondNumber = -1;
			break;
		case 72:
			*nFirstNumber = 17;
			*nSecondNumber = -1;
			break;
		case 73:
			*nFirstNumber = 18;
			*nSecondNumber = -1;
			break;
		case 74:
			*nFirstNumber = 19;
			*nSecondNumber = -1;
			break;
		case 75:
			*nFirstNumber = 20;
			*nSecondNumber = -1;
			break;
		case 76:
			*nFirstNumber = 21;
			*nSecondNumber = -1;
			break;
		case 77:
			*nFirstNumber = 22;
			*nSecondNumber = -1;
			break;
		case 78:
			*nFirstNumber = 23;
			*nSecondNumber = -1;
			break;
		case 79:
			*nFirstNumber = 24;
			*nSecondNumber = -1;
			break;
		case 80:
			*nFirstNumber = 25;
			*nSecondNumber = -1;
			break;
		case 81:
			*nFirstNumber = 26;
			*nSecondNumber = -1;
			break;
		case 82:
			*nFirstNumber = 27;
			*nSecondNumber = -1;
			break;
		case 83:
			*nFirstNumber = 28;
			*nSecondNumber = -1;
			break;
		case 84:
			*nFirstNumber = 29;
			*nSecondNumber = -1;
			break;
		case 85:
			*nFirstNumber = 30;
			*nSecondNumber = -1;
			break;
		case 86:
			*nFirstNumber = 31;
			*nSecondNumber = -1;
			break;
		case 87:
			*nFirstNumber = 32;
			*nSecondNumber = -1;
			break;
		case 88:
			*nFirstNumber = 33;
			*nSecondNumber = -1;
			break;
		case 89:
			*nFirstNumber = 34;
			*nSecondNumber = -1;
			break;
		case 90:
			*nFirstNumber = 35;
			*nSecondNumber = -1;
			break;
		case 91:
			*nFirstNumber = 44;
			*nSecondNumber = 20;
			break;
		case 92:
			*nFirstNumber = 44;
			*nSecondNumber = 21;
			break;
		case 93:
			*nFirstNumber = 44;
			*nSecondNumber = 22;
			break;
		case 94:
			*nFirstNumber = 44;
			*nSecondNumber = 23;
			break;
		case 95:
			*nFirstNumber = 44;
			*nSecondNumber = 24;
			break;
		case 96:
			*nFirstNumber = 44;
			*nSecondNumber = 32;
			break;
		case 97:
			*nFirstNumber = 46;
			*nSecondNumber = 10;
			break;
		case 98:
			*nFirstNumber = 46;
			*nSecondNumber = 11;
			break;
		case 99:
			*nFirstNumber = 46;
			*nSecondNumber = 12;
			break;
		case 100:
			*nFirstNumber = 46;
			*nSecondNumber = 13;
			break;
		case 101:
			*nFirstNumber = 46;
			*nSecondNumber = 14;
			break;
		case 102:
			*nFirstNumber = 46;
			*nSecondNumber = 15;
			break;
		case 103:
			*nFirstNumber = 46;
			*nSecondNumber = 16;
			break;
		case 104:
			*nFirstNumber = 46;
			*nSecondNumber = 17;
			break;
		case 105:
			*nFirstNumber = 46;
			*nSecondNumber = 18;
			break;
		case 106:
			*nFirstNumber = 46;
			*nSecondNumber = 19;
			break;
		case 107:
			*nFirstNumber = 46;
			*nSecondNumber = 20;
			break;
		case 108:
			*nFirstNumber = 46;
			*nSecondNumber = 21;
			break;
		case 109:
			*nFirstNumber = 46;
			*nSecondNumber = 22;
			break;
		case 110:
			*nFirstNumber = 46;
			*nSecondNumber = 23;
			break;
		case 111:
			*nFirstNumber = 46;
			*nSecondNumber = 24;
			break;
		case 112:
			*nFirstNumber = 46;
			*nSecondNumber = 25;
			break;
		case 113:
			*nFirstNumber = 46;
			*nSecondNumber = 26;
			break;
		case 114:
			*nFirstNumber = 46;
			*nSecondNumber = 27;
			break;
		case 115:
			*nFirstNumber = 46;
			*nSecondNumber = 28;
			break;
		case 116:
			*nFirstNumber = 46;
			*nSecondNumber = 29;
			break;
		case 117:
			*nFirstNumber = 46;
			*nSecondNumber = 30;
			break;
		case 118:
			*nFirstNumber = 46;
			*nSecondNumber = 31;
			break;
		case 119:
			*nFirstNumber = 46;
			*nSecondNumber = 32;
			break;
		case 120:
			*nFirstNumber = 46;
			*nSecondNumber = 33;
			break;
		case 121:
			*nFirstNumber = 46;
			*nSecondNumber = 34;
			break;
		case 122:
			*nFirstNumber = 46;
			*nSecondNumber = 35;
			break;
		case 123:
			*nFirstNumber = 44;
			*nSecondNumber = 25;
			break;
		case 124:
			*nFirstNumber = 44;
			*nSecondNumber = 26;
			break;
		case 125:
			*nFirstNumber = 44;
			*nSecondNumber = 27;
			break;
		case 126:
			*nFirstNumber = 44;
			*nSecondNumber = 28;
			break;
		case 127:
			*nFirstNumber = 44;
			*nSecondNumber = 29;
			break;
	}
}