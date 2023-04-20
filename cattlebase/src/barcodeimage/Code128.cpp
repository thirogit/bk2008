// Code128.cpp: implementation of the CCode128 class.
//
//	Copyright 2002 Neil Van Eps
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Code128.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCode128::CCode128()
{
	// default to subset a
	m_nCurrentSubset = SUBSETA;

	// code 128
	m_nSymbology = COD128;
}

CCode128::~CCode128()
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

void CCode128::DrawBitmap()
{
	long	nChar,nNextChar,nCharacterPosition,nCheckDigit;

	// calculate the check digit
	nCheckDigit = GetCheckDigit();
	
	// draw start character for current subset
	if (m_nCurrentSubset==SUBSETA)
		DrawPattern(RetrievePattern(103));
	else if (m_nCurrentSubset==SUBSETB)
		DrawPattern(RetrievePattern(104));
	else if (m_nCurrentSubset==SUBSETC)
		DrawPattern(RetrievePattern(105));
	
	// initialize position in message	
	nCharacterPosition = 0;

	while (nCharacterPosition < m_csMessage.GetLength())
	{
		if (m_nCurrentSubset==SUBSETC)
		{
			// if it's a switch to subsetA - same character (103) for all subsets
			if (g_nASCIItoCode128SubsetAB[SUBSETA][m_csMessage.GetAt(nCharacterPosition)]==101)
			{
				// draw the startA code
				DrawPattern(RetrievePattern(101));

				// we've moved one message character
				nCharacterPosition++;

				// actually change the subset
				m_nCurrentSubset = SUBSETA;
			}
			// if it's a switch to subsetB - same character (104) for all subsets
			else if (g_nASCIItoCode128SubsetAB[SUBSETA][m_csMessage.GetAt(nCharacterPosition)]==100)
			{
				// draw the startB code
				DrawPattern(RetrievePattern(100));

				// we've moved one message character
				nCharacterPosition++;

				// actually change the subset
				m_nCurrentSubset = SUBSETB;
			}
			// it's FNC1 - just print it out
			else if (g_nASCIItoCode128SubsetAB[SUBSETA][m_csMessage.GetAt(nCharacterPosition)]==102)
			{
				// draw the FNC1
				DrawPattern(RetrievePattern(100));

				// we've moved one message character
				nCharacterPosition++;
			}
			// it's a digit - pull two at a time
			else
			{
				CString		csTemp;

				// get the next two characters
				csTemp = m_csMessage.Mid(nCharacterPosition,2);

				// convert them to longs
				nChar = atol((const char *)csTemp);

				// draw the code 128 character
				DrawPattern(RetrievePattern(nChar));

				// we've moved two message characters
				nCharacterPosition += 2;
			}
		}
		// we're in SUBSETA or SUBSETB
		else
		{
			// handle upper ASCII characters if necessary
			long nTemp2 = m_csMessage.GetAt(nCharacterPosition);
			if (nTemp2<-1)
				nTemp2 = nTemp2&255;
			
			// retrieve the message character
			nChar = g_nASCIItoCode128SubsetAB[m_nCurrentSubset][nTemp2];

			// draw the char
			DrawPattern(RetrievePattern(nChar));

			// we've moved one character position
			nCharacterPosition++;

			// if switch in SUBSETA
			if (m_nCurrentSubset==SUBSETA)
			{
				if (nChar==100)
					m_nCurrentSubset = SUBSETB;
				else if (nChar==99)
					m_nCurrentSubset = SUBSETC;
			}
			// if switch in SUBSETB
			else if (m_nCurrentSubset==SUBSETB)
			{
				if (nChar==101)
					m_nCurrentSubset = SUBSETA;
				else if (nChar==99)
					m_nCurrentSubset = SUBSETC;
			}
			// if a shift character
			else if (nChar==98)
			{
				// shift subsets for the next character only
				if (m_nCurrentSubset==SUBSETA)
					nNextChar = g_nASCIItoCode128SubsetAB[SUBSETB][m_csMessage.GetAt(nCharacterPosition)];
				else
					nNextChar = g_nASCIItoCode128SubsetAB[SUBSETA][m_csMessage.GetAt(nCharacterPosition)];

				// draw the shifted character
				DrawPattern(RetrievePattern(nChar));

				// since we've handled two characters advance character position again
				nCharacterPosition++;
			}
		}
	}

	// draw check digit
	DrawPattern(RetrievePattern(nCheckDigit));
	
	// draw stop character
	DrawPattern(RetrievePattern(106));

	return;
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

void CCode128::BitmapToClipboard()
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
//		CCode128::DrawBitmap()
//
////////////////////////////////////////////////////////////////////////////////////

void CCode128::DrawPattern(CString csPattern)
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
//		CCode128::DrawBitmap()
//
////////////////////////////////////////////////////////////////////////////////////

CString CCode128::RetrievePattern(long c)
{
	CString csCharPattern;
	
	switch (c)
	{
		case 0:
			csCharPattern = "bbsbbssbbss";
			break;
		case 1:
			csCharPattern = "bbssbbsbbss";
			break;
		case 2:
			csCharPattern = "bbssbbssbbs";
			break;
		case 3:
			csCharPattern = "bssbssbbsss";
			break;
		case 4:
			csCharPattern = "bssbsssbbss";
			break;
		case 5:
			csCharPattern = "bsssbssbbss";
			break;
		case 6:
			csCharPattern = "bssbbssbsss";
			break;
		case 7:
			csCharPattern = "bssbbsssbss";
			break;
		case 8:
			csCharPattern = "bsssbbssbss";
			break;
		case 9:
			csCharPattern = "bbssbssbsss";
			break;
		case 10:
			csCharPattern = "bbssbsssbss";
			break;
		case 11:
			csCharPattern = "bbsssbssbss";
			break;
		case 12:
			csCharPattern = "bsbbssbbbss";
			break;
		case 13:
			csCharPattern = "bssbbsbbbss";
			break;
		case 14:
			csCharPattern = "bssbbssbbbs";
			break;
		case 15:
			csCharPattern = "bsbbbssbbss";
			break;
		case 16:
			csCharPattern = "bssbbbsbbss";
			break;
		case 17:
			csCharPattern = "bssbbbssbbs";
			break;
		case 18:
			csCharPattern = "bbssbbbssbs";
			break;
		case 19:
			csCharPattern = "bbssbsbbbss";
			break;
		case 20:
			csCharPattern = "bbssbssbbbs";
			break;
		case 21:
			csCharPattern = "bbsbbbssbss";
			break;
		case 22:
			csCharPattern = "bbssbbbsbss";
			break;
		case 23:
			csCharPattern = "bbbsbbsbbbs";
			break;
		case 24:
			csCharPattern = "bbbsbssbbss";
			break;
		case 25:
			csCharPattern = "bbbssbsbbss";
			break;
		case 26:
			csCharPattern = "bbbssbssbbs";
			break;
		case 27:
			csCharPattern = "bbbsbbssbss";
			break;
		case 28:
			csCharPattern = "bbbssbbsbss";
			break;
		case 29:
			csCharPattern = "bbbssbbssbs";
			break;
		case 30:
			csCharPattern = "bbsbbsbbsss";
			break;
		case 31:
			csCharPattern = "bbsbbsssbbs";
			break;
		case 32:
			csCharPattern = "bbsssbbsbbs";
			break;
		case 33:
			csCharPattern = "bsbsssbbsss";
			break;
		case 34:
			csCharPattern = "bsssbsbbsss";
			break;
		case 35:
			csCharPattern = "bsssbsssbbs";
			break;
		case 36:
			csCharPattern = "bsbbsssbsss";
			break;
		case 37:
			csCharPattern = "bsssbbsbsss";
			break;
		case 38:
			csCharPattern = "bsssbbsssbs";
			break;
		case 39:
			csCharPattern = "bbsbsssbsss";
			break;
		case 40:
			csCharPattern = "bbsssbsbsss";
			break;
		case 41:
			csCharPattern = "bbsssbsssbs";
			break;
		case 42:
			csCharPattern = "bsbbsbbbsss";
			break;
		case 43:
			csCharPattern = "bsbbsssbbbs";
			break;
		case 44:
			csCharPattern = "bsssbbsbbbs";
			break;
		case 45:
			csCharPattern = "bsbbbsbbsss";
			break;
		case 46:
			csCharPattern = "bsbbbsssbbs";
			break;
		case 47:
			csCharPattern = "bsssbbbsbbs";
			break;
		case 48:
			csCharPattern = "bbbsbbbsbbs";
			break;
		case 49:
			csCharPattern = "bbsbsssbbbs";
			break;
		case 50:
			csCharPattern = "bbsssbsbbbs";
			break;
		case 51:
			csCharPattern = "bbsbbbsbsss";
			break;
		case 52:
			csCharPattern = "bbsbbbsssbs";
			break;
		case 53:
			csCharPattern = "bbsbbbsbbbs";
			break;
		case 54:
			csCharPattern = "bbbsbsbbsss";
			break;
		case 55:
			csCharPattern = "bbbsbsssbbs";
			break;
		case 56:
			csCharPattern = "bbbsssbsbbs";
			break;
		case 57:
			csCharPattern = "bbbsbbsbsss";
			break;
		case 58:
			csCharPattern = "bbbsbbsssbs";
			break;
		case 59:
			csCharPattern = "bbbsssbbsbs";
			break;
		case 60:
			csCharPattern = "bbbsbbbbsbs";
			break;
		case 61:
			csCharPattern = "bbssbssssbs";
			break;
		case 62:
			csCharPattern = "bbbbsssbsbs";
			break;
		case 63:
			csCharPattern = "bsbssbbssss";
			break;
		case 64:
			csCharPattern = "bsbssssbbss";
			break;
		case 65:
			csCharPattern = "bssbsbbssss";
			break;
		case 66:
			csCharPattern = "bssbssssbbs";
			break;
		case 67:
			csCharPattern = "bssssbsbbss";
			break;
		case 68:
			csCharPattern = "bssssbssbbs";
			break;
		case 69:
			csCharPattern = "bsbbssbssss";
			break;
		case 70:
			csCharPattern = "bsbbssssbss";
			break;
		case 71:
			csCharPattern = "bssbbsbssss";
			break;
		case 72:
			csCharPattern = "bssbbssssbs";
			break;
		case 73:
			csCharPattern = "bssssbbsbss";
			break;
		case 74:
			csCharPattern = "bssssbbssbs";
			break;
		case 75:
			csCharPattern = "bbssssbssbs";
			break;
		case 76:
			csCharPattern = "bbssbsbssss";
			break;
		case 77:
			csCharPattern = "bbbbsbbbsbs";
			break;
		case 78:
			csCharPattern = "bbssssbsbss";
			break;
		case 79:
			csCharPattern = "bsssbbbbsbs";
			break;
		case 80:
			csCharPattern = "bsbssbbbbss";
			break;
		case 81:
			csCharPattern = "bssbsbbbbss";
			break;
		case 82:
			csCharPattern = "bssbssbbbbs";
			break;
		case 83:
			csCharPattern = "bsbbbbssbss";
			break;
		case 84:
			csCharPattern = "bssbbbbsbss";
			break;
		case 85:
			csCharPattern = "bssbbbbssbs";
			break;
		case 86:
			csCharPattern = "bbbbsbssbss";
			break;
		case 87:
			csCharPattern = "bbbbssbsbss";
			break;
		case 88:
			csCharPattern = "bbbbssbssbs";
			break;
		case 89:
			csCharPattern = "bbsbbsbbbbs";
			break;
		case 90:
			csCharPattern = "bbsbbbbsbbs";
			break;
		case 91:
			csCharPattern = "bbbbsbbsbbs";
			break;
		case 92:
			csCharPattern = "bsbsbbbbsss";
			break;
		case 93:
			csCharPattern = "bsbsssbbbbs";
			break;
		case 94:
			csCharPattern = "bsssbsbbbbs";
			break;
		case 95:
			csCharPattern = "bsbbbbsbsss";
			break;
		case 96:
			csCharPattern = "bsbbbbsssbs";
			break;
		case 97:
			csCharPattern = "bbbbsbsbsss";
			break;
		case 98:
			csCharPattern = "bbbbsbsssbs";
			break;
		case 99:
			csCharPattern = "bsbbbsbbbbs";
			break;
		case 100:
			csCharPattern = "bsbbbbsbbbs";
			break;
		case 101:
			csCharPattern = "bbbsbsbbbbs";
			break;
		case 102:
			csCharPattern = "bbbbsbsbbbs";
			break;
		case 103:
			csCharPattern = "bbsbsbbbbss";
			break;
		case 104:
			csCharPattern = "bbsbssbssss";
			break;
		case 105:
			csCharPattern = "bbsbssbbbss";
			break;
		case 106:
			csCharPattern = "bbsssbbbsbsbb";
			break;
	}

	return csCharPattern;
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Name:
//		AddCheckDigitToMessage()
//
//	Description:
//		adds the code 128 check digit to the end of the message
//
//	Arguments:
//		none
//
//	Return:
//		void
//
//	Called by:
//		DrawBitmap()
//
////////////////////////////////////////////////////////////////////////////////////

long CCode128::GetCheckDigit()
{
	long	nSum=0,nCurrentSubset=0,nCode128Char,nNextChar,nWeight,nCharacterPosition;

	// start character
	if (m_nCurrentSubset==SUBSETA)
	{
		nSum = 103;
		nCurrentSubset = SUBSETA;
	}
	else if (m_nCurrentSubset==SUBSETB)
	{
		nSum = 104;
		nCurrentSubset = SUBSETB;
	}
	else if (m_nCurrentSubset==SUBSETC)
	{
		nSum = 105;
		nCurrentSubset = SUBSETC;
	}

	// intialize the values
	nCharacterPosition = 0;
	nWeight = 1;

	while (nCharacterPosition<(m_csMessage.GetLength()))
	{
		// if SUBSETC
		if (nCurrentSubset==SUBSETC)
		{
			// if it's a switch to SUBSETA - same character in all subsets
			if (g_nASCIItoCode128SubsetAB[SUBSETA][m_csMessage.GetAt(nCharacterPosition)]==101)
			{
				// we're switching to subsetA
				nCode128Char = 101;
				
				// add the change subset character to the sum
				nSum+= (nWeight*nCode128Char);
				
				// we've moved one message character
				nCharacterPosition++;

				// we've moved one weight value
				nWeight++;

				// actually change the subset
				nCurrentSubset = SUBSETA;
			}
			// if it's a switch to SUBSETB - same character in all subsets
			else if (g_nASCIItoCode128SubsetAB[SUBSETA][m_csMessage.GetAt(nCharacterPosition)]==100)
			{
				// we're switching to subset B
				nCode128Char = 100;
				
				// add the change subset character to the sum
				nSum+= (nWeight*nCode128Char);
				
				// we've moved one message character
				nCharacterPosition++;

				// we've moved one weight value
				nWeight++;

				// actually switch the subset
				nCurrentSubset = SUBSETB;
			}
			// it's FNC1 - just print it out
			else if (g_nASCIItoCode128SubsetAB[SUBSETA][m_csMessage.GetAt(nCharacterPosition)]==102)
			{
				// we're switching to subset B
				nCode128Char = 102;
				
				// add the change subset character to the sum
				nSum+= (nWeight*nCode128Char);
				
				// we've moved one message character
				nCharacterPosition++;

				// we've moved one weight value
				nWeight++;
			}
			// its a digit - process two at a time
			else
			{
				CString		csTemp;

				// get the next two characters
				csTemp = m_csMessage.Mid(nCharacterPosition,2);

				// convert them to longs
				nCode128Char = atol((const char *)csTemp);

				// add the weighted balue
				nSum += (nWeight*nCode128Char);

				// we've moved two message characters
				nCharacterPosition += 2;

				// we've moved one weight value
				nWeight++;
			}
		}
		// it's SUBSETA or SUBSETB
		else 
		{
			// handle upper ASCII characters if necessary
			long nTemp2 = m_csMessage.GetAt(nCharacterPosition);
			if (nTemp2<-1)
				nTemp2 = nTemp2&255;
			
			// retrieve the message character
			nCode128Char = g_nASCIItoCode128SubsetAB[nCurrentSubset][nTemp2];

			// add the weighted value to our sum
			nSum+= (nWeight*nCode128Char);

			// we've moved one character position
			nCharacterPosition++;

			// we've moved one weight value
			nWeight++;

			// if switch in SUBSETA
			if (nCurrentSubset==SUBSETA)
			{
				if (nCode128Char==100)
					nCurrentSubset = SUBSETB;
				else if (nCode128Char==99)
					nCurrentSubset = SUBSETC;
			}
			// if switch in SUBSETB
			else if (nCurrentSubset==SUBSETB)
			{
				if (nCode128Char==101)
					nCurrentSubset = SUBSETA;
				else if (nCode128Char==99)
					nCurrentSubset = SUBSETC;
			}
			// handle single character switch
			else if (nCode128Char==98)
			{
				// shift subsets for the next character only
				if (nCurrentSubset==SUBSETA)
					nNextChar = g_nASCIItoCode128SubsetAB[SUBSETB][m_csMessage.GetAt(nCharacterPosition)];
				else
					nNextChar = g_nASCIItoCode128SubsetAB[SUBSETA][m_csMessage.GetAt(nCharacterPosition)];

				// add weighted value to the sum
				nSum += (nWeight*nNextChar);

				// since we've handled two characters advance position and weight again
				nCharacterPosition++;
				nWeight++;
			}
		}
	}

	// return the modulus
	return (nSum%103);
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Name:
//		LoadData()
//
//	Description:
//		overridden to include starting subset
//
//	Arguments:
//		CString		csMessage
//		int			nSymbology
//		double		dNarrowBar
//		double		dFinalHeight
//		HDC			pDC
//		int			nStartingXPixel
//		int			nStartingYPixel
//		double		dRatio
//		long		nStartingSubset
//
//	Return:
//		void
//
//	Called by:
//		public class interface
//
////////////////////////////////////////////////////////////////////////////////////

void CCode128::LoadData(CString csMessage, double dNarrowBar, double dFinalHeight, HDC pDC, int nStartingXPixel, int nStartingYPixel, long nStartingSubset )
{
	// call base class version
	CBarcode::LoadData(csMessage, dNarrowBar, dFinalHeight, pDC, nStartingXPixel, nStartingYPixel);

	// set additional data
	m_nCurrentSubset = nStartingSubset;
}
