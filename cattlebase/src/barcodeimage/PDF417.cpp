#include "stdafx.h"
#include "PDF417.h"

CPDF417::CPDF417() 
{
	m_pdf417.outBits = NULL;
}

CPDF417::~CPDF417()
{
	pdf417free(&m_pdf417);
}

CPDF417::CPDF417(const CString& sMessage,UINT dpi,double heightInch)
{
	m_pdf417.outBits = NULL;
	SetMessage(sMessage,dpi,heightInch);
}
void CPDF417::SetMessage(const CString& sMessage,UINT dpi,double heightInch)
{
	if(sMessage.GetLength() == 0 || dpi == 0 || heightInch <= 0)
		return;

	pdf417free(&m_pdf417);
	pdf417init(&m_pdf417);

	m_sMessage = sMessage;
	m_DPI = dpi;
	m_heightInch = heightInch;

	m_pdf417.yHeight = 1;
    m_pdf417.text = (LPCSTR)m_sMessage;
    m_pdf417.options = PDF417_INVERT_BITMAP;
    paintCode(&m_pdf417);
    if (m_pdf417.error) 
	{
        pdf417free(&m_pdf417);
        return;
    }

}

long CPDF417::GetBarcodePixelWidth() const
{
	return GetBarcodePixelHeight()*GetWidthHeightRatio();
}
long CPDF417::GetBarcodePixelHeight() const
{
	return (m_DPI*m_heightInch);	
}

double CPDF417::GetWidthHeightRatio() const
{
	return (double)m_pdf417.bitColumns/(double) m_pdf417.codeRows;
}

bool CPDF417::AccessBit(int x,int y,int scanlineW,const char* bitBuffer) const
{
	int yByte = y*(scanlineW);
	int xByte = x/8;
	int xBit = x%8;

	//if(xBit == 0 && xByte > 0) xBit = 7;

	unsigned char byte = bitBuffer[xByte + yByte];

	char mask = 128 >> xBit;

	return ((byte & mask) != 0);
	
}

void CPDF417::DrawBitmap(int x,int y,fipImage &image) const
{
	long w = GetBarcodePixelWidth(), h = GetBarcodePixelHeight();
	double ratio = GetWidthHeightRatio();

	RGBQUAD black,white;
	
	black.rgbBlue = 0;
	black.rgbGreen = 0;
	black.rgbRed = 0;
	black.rgbReserved = 0;

	white.rgbBlue = 255;
	white.rgbGreen = 255;
	white.rgbRed = 255;
	white.rgbReserved = 0;

	int scanlineW = (m_pdf417.bitColumns/8 + ((m_pdf417.bitColumns%8) ? 1 : 0));
	
	double ratioW = (double) w/m_pdf417.bitColumns;
	double ratioH =  (double) h/m_pdf417.codeRows;

	for(int yy = 0;yy <  h;yy++)
	{
		for(int xx = 0;xx < w;xx++)
		{
			if(!AccessBit(xx/ratioW,yy/ratioH,scanlineW,m_pdf417.outBits))
			{
				image.setPixelColor(x+xx,y+yy,&black);
			}
			else
			{
				image.setPixelColor(x+xx,y+yy,&white);
			}
		}
	}

	
}
