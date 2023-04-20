// Barcode.cpp: implementation of the C1DBarcode class.
//
//	Copyright 2002 Neil Van Eps
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "1DBarcode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C1DBarcode::C1DBarcode()
{
	m_COLORWHITE.rgbBlue = 0xFF;
	m_COLORWHITE.rgbGreen = 0xFF;
	m_COLORWHITE.rgbRed = 0xFF;
	m_COLORWHITE.rgbReserved = 0x00;

	m_COLORBLACK.rgbBlue = 0x00;
	m_COLORBLACK.rgbGreen = 0x00;
	m_COLORBLACK.rgbRed = 0x00;
	m_COLORBLACK.rgbReserved = 0x00;
}

C1DBarcode::~C1DBarcode()
{

}

////////////////////////////////////////////////////////////////////////////////////
//
//	Name:
//		LoadData()
//
//	Description:
//
//
//	Arguments:
//
//
//	Return:
//
//
//	Called by:
//
//
////////////////////////////////////////////////////////////////////////////////////

bool C1DBarcode::LoadData(CString csMessage, 
						double dNarrowBarWidth, 
						double dFinalHeight,
						double dNarrowToWideBarRatio,
						int DPI)
{
	int		i,nXAxisDpi = DPI,nYAxisDpi = DPI,nTemp;
	char	c;
	
	if(DPI <= 0 || dNarrowBarWidth < 0.0 ||  dFinalHeight < 0.0 || dNarrowToWideBarRatio < 0.0)
		return false;

	// values that can be saved without translation
	m_csMessage = csMessage;	

	// load the final attributes that depend on the device context
	m_nPixelHeight = (int)((nYAxisDpi*dFinalHeight)+0.5);
	m_nNarrowBarPixelWidth = (int)((nXAxisDpi*dNarrowBarWidth)+0.5);
	m_nWideBarPixelWidth = (int)(dNarrowToWideBarRatio*m_nNarrowBarPixelWidth);

	switch (m_nSymbology)
	{
		case RATIONALCODABAR:
			// initialize to zero
			m_nFinalBarcodePixelWidth = 0;

			// add the width of each character
			for (i=0;i<csMessage.GetLength();i++)
			{
				c = csMessage.GetAt(i);

				switch (c)
				{
					case '0':
					case '1':
					case '2':
					case '3':	
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
					case '-':
					case '$':
						m_nFinalBarcodePixelWidth += (6*m_nNarrowBarPixelWidth)+(2*m_nWideBarPixelWidth);
						break;
					case ':':
					case '/':
					case '.':
					case '+':
					case 'A':
					case 'B':
					case 'C':
					case 'D':
						m_nFinalBarcodePixelWidth += (5*m_nNarrowBarPixelWidth)+(3*m_nWideBarPixelWidth);
						break;
				}
			}
			break;
		case I2OF5:
			// add start code
			m_nFinalBarcodePixelWidth = 4 * m_nNarrowBarPixelWidth;

			// add message
			m_nFinalBarcodePixelWidth += ((3*m_nNarrowBarPixelWidth)+(2*m_nWideBarPixelWidth))*m_csMessage.GetLength();
			
			// add stop code
			m_nFinalBarcodePixelWidth += (2*m_nNarrowBarPixelWidth)+(m_nWideBarPixelWidth);
			
			break;
		case CODE39:
			// get final character width
			nTemp = m_csMessage.GetLength() + 2;

			// add message
			m_nFinalBarcodePixelWidth = nTemp * ((3*m_nWideBarPixelWidth) + (7*m_nNarrowBarPixelWidth));

			break;
		case COD128:
			// get final character width
			nTemp = m_csMessage.GetLength();

			m_nFinalBarcodePixelWidth = ((nTemp*11)+35)*m_nNarrowBarPixelWidth;

			break;
		case CODE93:
			// get final character width
			nTemp = m_csMessage.GetLength();

			m_nFinalBarcodePixelWidth = (((nTemp+4)*9)+1)*m_nNarrowBarPixelWidth;
		case UPCEAN:
		case TRADITIONALCODABAR:
			break;
	}

	return true;
}

long C1DBarcode::GetBarcodePixelHeight() const
{
	return m_nPixelHeight;
}

long C1DBarcode::GetBarcodePixelWidth() const
{
	return m_nFinalBarcodePixelWidth;
}