#ifndef __1DBARCODE_H__
#define __1DBARCODE_H__

#include "BaseBarcode.h"

enum Symbology
{
   RATIONALCODABAR,
   TRADITIONALCODABAR,
   I2OF5,
   CODE39,
   COD128,
   UPCEAN,
   CODE93
};

class C1DBarcode : public CBaseBarcode
{
public:
	C1DBarcode();
	bool LoadData(CString csMessage, 
			double dNarrowBarWidth, 
			double dFinalHeight,
			double dNarrowToWideBarRatio,
			int DPI);
		
	virtual ~C1DBarcode();
	long	GetBarcodePixelWidth() const;
	long	GetBarcodePixelHeight() const;
protected:
	CString m_csMessage;
	
	long	m_nFinalBarcodePixelWidth;
	long	m_nNarrowBarPixelWidth;
	long	m_nPixelHeight;
	long	m_nSymbology;
	long	m_nWideBarPixelWidth;
//	virtual int DrawPattern(const CString &csPattern,int x,int y,fipImage &image) = 0;

	RGBQUAD	m_COLORWHITE;
	RGBQUAD m_COLORBLACK;
};

#endif
