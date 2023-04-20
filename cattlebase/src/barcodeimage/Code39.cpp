// Code39.cpp: implementation of the CCode39 class.
//
//	Copyright 2002 Neil Van Eps
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Code39.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCode39::CCode39()
{
	// code 39
	m_nSymbology = CODE39;
}

CCode39::~CCode39()
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

void CCode39::DrawBitmap(int x,int y,fipImage &image) const
{
	int		i;
	CString	csCurrentPattern;

	// draw start character, an asterisk
	int newXPos = DrawPattern(RetrievePattern('*'),x,y,image);
	
	// draw each character in the message
	for (i=0;i<m_csMessage.GetLength();i++)
		newXPos = DrawPattern(RetrievePattern(m_csMessage.GetAt(i)),newXPos,y,image);

	// draw stop character, also an asterisk
	DrawPattern(RetrievePattern('*'),newXPos,y,image);

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

int CCode39::DrawPattern( const CString &csPattern,int x,int y,fipImage &image ) const
{
	
	int nStartingYPixel = y;
	int nStartingXPixel = x;
	int			i,nXPixel,nYPixel,nTempWidth;
	
	// initialize X pixel value
	nXPixel = nStartingXPixel;
	
	for (i=0;i<csPattern.GetLength();i++)
	{
		// decide if narrow or wide bar
		if (csPattern.GetAt(i)=='n')
			nTempWidth = m_nNarrowBarPixelWidth;
		else
			nTempWidth = m_nWideBarPixelWidth;
		
		// X value for loop
		for (nXPixel=nStartingXPixel;nXPixel<nStartingXPixel+nTempWidth;nXPixel++)
		{
			// Y value for loop
			for (nYPixel=nStartingYPixel;nYPixel<nStartingYPixel+m_nPixelHeight;nYPixel++)
			{
				// if this is a bar
				if (i%2==0)
					image.setPixelColor(nXPixel,nYPixel,const_cast<RGBQUAD*>(&m_COLORBLACK));
				else
					image.setPixelColor(nXPixel,nYPixel,const_cast<RGBQUAD*>(&m_COLORWHITE));
			}
		}

		// advance the starting position
		nStartingXPixel+= nTempWidth;
	}

	
	return nStartingXPixel;
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

CString CCode39::RetrievePattern(char c) const
{
	CString	csCharPattern;

	switch (c)
	{
		case '1':
			csCharPattern = "wnnwnnnnwn";
			break;
		case '2':
			csCharPattern = "nnwwnnnnwn";
			break;
		case '3':
			csCharPattern = "wnwwnnnnnn";
			break;
		case '4':
			csCharPattern = "nnnwwnnnwn";
			break;
		case '5':
			csCharPattern = "wnnwwnnnnn";
			break;
		case '6':
			csCharPattern = "nnwwwnnnnn";
			break;
		case '7':
			csCharPattern = "nnnwnnwnwn";
			break;
		case '8':
			csCharPattern = "wnnwnnwnnn";
			break;
		case '9':
			csCharPattern = "nnwwnnwnnn";
			break;
		case '0':
			csCharPattern = "nnnwwnwnnn";
			break;
		case 'A':
			csCharPattern = "wnnnnwnnwn";
			break;
		case 'B':
			csCharPattern = "nnwnnwnnwn";
			break;
		case 'C':
			csCharPattern = "wnwnnwnnnn";
			break;
		case 'D':
			csCharPattern = "nnnnwwnnwn";
			break;
		case 'E':
			csCharPattern = "wnnnwwnnnn";
			break;
		case 'F':
			csCharPattern = "nnwnwwnnnn";
			break;
		case 'G':
			csCharPattern = "nnnnnwwnwn";
			break;
		case 'H':
			csCharPattern = "wnnnnwwnnn";
			break;
		case 'I':
			csCharPattern = "nnwnnwwnnn";
			break;
		case 'J':
			csCharPattern = "nnnnwwwnnn";
			break;
		case 'K':
			csCharPattern = "wnnnnnnwwn";
			break;
		case 'L':
			csCharPattern = "nnwnnnnwwn";
			break;
		case 'M':
			csCharPattern = "wnwnnnnwnn";
			break;
		case 'N':
			csCharPattern = "nnnnwnnwwn";
			break;
		case 'O':
			csCharPattern = "wnnnwnnwnn";
			break;
		case 'P':
			csCharPattern = "nnwnwnnwnn";
			break;
		case 'Q':
			csCharPattern = "nnnnnnwwwn";
			break;
		case 'R':
			csCharPattern = "wnnnnnwwnn";
			break;
		case 'S':
			csCharPattern = "nnwnnnwwnn";
			break;
		case 'T':
			csCharPattern = "nnnnwnwwnn";
			break;
		case 'U':
			csCharPattern = "wwnnnnnnwn";
			break;
		case 'V':
			csCharPattern = "nwwnnnnnwn";
			break;
		case 'W':
			csCharPattern = "wwwnnnnnnn";
			break;
		case 'X':
			csCharPattern = "nwnnwnnnwn";
			break;
		case 'Y':
			csCharPattern = "wwnnwnnnnn";
			break;
		case 'Z':
			csCharPattern = "nwwnwnnnnn";
			break;
		case '-':
			csCharPattern = "nwnnnnwnwn";
			break;
		case '.':
			csCharPattern = "wwnnnnwnnn";
			break;
		case ' ':
			csCharPattern = "nwwnnnwnnn";
			break;
		case '*':
			csCharPattern = "nwnnwnwnnn";
			break;
		case '$':
			csCharPattern = "nwnwnwnnnn";
			break;
		case '/':
			csCharPattern = "nwnwnnnwnn";
			break;
		case '+':
			csCharPattern = "nwnnnwnwnn";
			break;
		case '%':
			csCharPattern = "nnnwnwnwnn";
			break;
	}

	return csCharPattern;
}
