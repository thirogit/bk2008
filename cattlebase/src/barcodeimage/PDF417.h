#ifndef __PDF417_H__
#define __PDF417_H__

#include "BaseBarcode.h"
#include "pdf417lib.h"

class CPDF417 :  public CBaseBarcode
{
public:
	CPDF417();
	~CPDF417();
	CPDF417(const CString& sMessage,UINT dpi,double heightInch);
	void SetMessage(const CString& sMessage,UINT dpi,double heightInch);
	virtual void DrawBitmap(int x,int y,fipImage &image) const;
	long	GetBarcodePixelWidth() const;
	long	GetBarcodePixelHeight() const;
protected:
	double GetWidthHeightRatio() const;
	bool AccessBit(int x,int y,int scanlineW,const char* bitBuffer) const;

protected:
	CString m_sMessage;
	UINT m_DPI;
	double m_heightInch;
	pdf417param m_pdf417;
	
};



#endif